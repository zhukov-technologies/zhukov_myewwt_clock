/*
  Name:        MYEWWT.ino
  Created:     22.09.2021
  Author:      Zhukov Gleb (https://github.com/zbltrz)
  Description:

  This project was created for PCB board of main device, which is called "MYEWWT".
  This is a clock with DHT22 on a board.
  MYEWWT clock is the main part of the smart home system.
  This clock works with WS2812B LEDs.
  This board with ESP12f connects to the Internet via WIFI and communicates with the telegram bot,
  Receiving commands from the bot, controls peripheral devices: light switches, smart sockets, etc. through ESP-NOW technology
*/
#include <Arduino.h>
#include <Ticker.h>
#include "FastBot.h"
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <espnow.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <TimerMs.h>

////////////////////SERVER////////////////////

IPAddress local_IP(10, 10, 10, 10);
IPAddress gateway(10, 10, 10, 10);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer webServer(80);

////////////////////BRIGHTNESS////////////////////
//минимальные и максимальные показания детчика
#define BRIGHTNESS_ADC_LOW  15
#define BRIGHTNESS_ADC_HIGH 300
//минимальная и максимальная освещенность
#define BRIGTHNESS_HIGH 200
#define BRIGHTNESS_LOW  6


////////////////////TELEGRAM BOT////////////////////

FastBot bot;



////////////////////FLAGS////////////////////


bool LED; //для мигания секундных точек
bool new_id_flag; //для добавления нового пользователя
bool light_flag; //для добавления устройства освещения (не подтвержденное)
bool sock_flag; //для добавления розетки (не подтвержденное)
bool new_light_flag; //для добавления устройства освещения (подтвержденное)
bool new_sock_flag; //для добавления розетки (подтвержденное)
bool switch_flag; //для добавления выключателя (не подтвержденное)
bool new_switch_flag; //для добавления выключателя (подтвержденное)
bool del_light_flag; //для удаления устройства освещения
bool del_sock_flag; //для удаления розетки
bool reset_settings_flag; //для сброса настроек
bool del_user_flag; //для удаления пользователя
bool first_user_flag; //для добавления первого пользователя при старте
bool mode_flag; //1 - точка доступа с сервером для заполнения формы, 0 - обычная работа
bool answer_flag; //для ответа при запросе розетки/устройства освещения
bool esp_now_flag; //для отправки данных по esp-now
bool auto_bright; //для автояркости



byte esp_now_mode; //коды ответа по esp-now

String new_device_ID; //ID пользователя, запросившего добавления розетки/устройства освещения для ответа ему
String answer_ID; //ID пользователя, запросившего изменение состояния розетки/устройства освещения для ответа ему


////////////////////DHT-22////////////////////
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


////////////////////WIFI////////////////////
#define AP_SSID "MYEWWT"
#define AP_PASS "iloveyou"


////////////////////TIME////////////////////

TimerMs rainbow_tmr(50, 1, 0); //для управления "радугой" в режиме сервера
TimerMs sync_tmr(100, 1, 0); //для синхронизации времени по ntp
TimerMs firmware_tmr(1000 * 60 * 2, 1, 0); //для обновления прошивки
TimerMs reset_tmr(5000, 0, 1); //для перезагрузки при сохранении параметров


int year;
byte second, minute, hour;

Ticker TIM_Test_0; //Таймер_0


void send_answer(byte answer);
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len);
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void SetBrightness();
uint32_t Wheel(byte WheelPos);
void rainbow();
void showLed();
void mainForm();
void confirmPage();
void handleForm();
void Ticker_Circular_Callback(void);
void first_start();
void reset_settings();
void update_firm();
String light_menu();
String sock_menu();
void ConnectWIFI();
void UpdateTime();
void newMsg(FB_msg& msg);


////////////////////ESP-NOW////////////////////

uint8_t broadcastAddress[6]; //для хранения mac-адреса

struct {
  byte ID;
  char WIFI_ROUTER_SSID[35];
  uint8_t Address[6];
} to_switch;

struct {
  byte ID;
  byte value;
} myData;

////////////////////LED////////////////////
#define PIN        12
#define NUMPIXELS 30

//массив для управления светодиодами как цифрами и знаками
const bool num[][7] PROGMEM = {
  {0, 1, 1, 1, 1, 1, 1}, //цифра 0
  {0, 1, 0, 0, 0, 0, 1}, //цифра 1
  {1, 0, 1, 1, 0, 1, 1}, //цифра 2
  {1, 1, 1, 0, 0, 1, 1}, //цифра 3
  {1, 1, 0, 0, 1, 0, 1}, //цифра 4
  {1, 1, 1, 0, 1, 1, 0}, //цифра 5
  {1, 1, 1, 1, 1, 1, 0}, //цифра 6
  {0, 1, 0, 0, 0, 1, 1}, //цифра 7
  {1, 1, 1, 1, 1, 1, 1}, //цифра 8
  {1, 1, 1, 0, 1, 1, 1}, //цифра 9
  {1, 0, 0, 0, 1, 1, 1}, //знак градуса
  {0, 0, 1, 1, 1, 1, 0}, //знак С
  {1, 0, 0, 1, 1, 1, 1}, //знак Р
};


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);



void setup() {

  pixels.begin();
  pixels.clear();



  Serial.begin(115200);

  EEPROM.begin(4096);
  if (EEPROM.read(1) != 4) //проверка на первый запуск
  {
   first_start();
  }


  EEPROM.get(1040, mode_flag);
  EEPROM.get(1460, first_user_flag);

  //если режим сервера
  if (mode_flag) {

    WiFi.softAPConfig(local_IP, gateway, subnet); //настраиваем свою точку доступа
    WiFi.softAP(AP_SSID, AP_PASS); //создаём свою точку доступа

    webServer.on("/save", handleForm);
    webServer.on("/confirm", confirmPage);
    webServer.on("/", mainForm);
    webServer.onNotFound(mainForm);
    webServer.begin();
  }

  else {
    ConnectWIFI(); //функция соединения с Wi-Fi

    dht.begin();

    pinMode(4, INPUT_PULLUP);


    configTime("MSK - 3MSD - 3", "time.google.com", "time.windows.com", "pool.ntp.org"); //сервера для синхронизации ntp


    int32_t userid[10];
    EEPROM.get(2, userid);
    String massive = "0,";
    for (int i = 0; i < 10; i++) {
      if (userid[i] != 0) {
        massive += userid[i];
        massive += ",";
      }
    }
    if (massive == "0,") {
      massive = "";
      first_user_flag = 1;
    }
    bot.setChatID(massive);

    char token[60];  //Telegram token
    EEPROM.get(1160, token); //получаем в глобальную переменную значения токена с EEPROM
    bot.setToken(token);
    bot.attach(newMsg);

    //запускаем esp-now (для связи с периферийными устройствами)
    if (esp_now_init() != 0) {
      return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO); //роль контроллера. комбо для двустороннего общения
    esp_now_register_send_cb(OnDataSent); //регистрируем функцию вызываемую при передаче данных
    esp_now_register_recv_cb(OnDataRecv); //регистрируем функцию вызываемую при приёме данных

    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    pixels.setBrightness(forEEPROM.userBright_clock); //настраиваем яркость светодиодов
    showLed(); //функция вывода цифр на светодиоды

    EEPROM.get(1045, to_switch.WIFI_ROUTER_SSID);

    TIM_Test_0.attach_ms(1000, Ticker_Circular_Callback);      //Запуск таймера в циклическом режиме


  }
}




void loop() {

  switch (mode_flag) {
    case 0:
      {
        if (esp_now_flag)
        {
          esp_now_flag = false;
          send_answer(esp_now_mode);
        }

        //управление кнопкой (для сброса всех настроек)
        if (!digitalRead(4)) {
          reset_settings();
        }

        if (sync_tmr.tick() && (WiFi.status() == WL_CONNECTED)) {
          sync_tmr.setTime(300000);
          do {
            UpdateTime();
            yield();
          } while (year == 70);
          showLed();
          sync_tmr.setTime(300000);
        }

        if (firmware_tmr.tick() && (WiFi.status() == WL_CONNECTED)) {
          update_firm();
        }
        
        bot.tick();
      }
      break;
    case 1:
      {
        webServer.handleClient(); //обработка сервера
        if (rainbow_tmr.tick()) {
          rainbow(); //режим радуги
        }
        if (reset_tmr.tick()) {
          delay(1000);
          ESP.restart();
        }
      }
      break;
  }
}
