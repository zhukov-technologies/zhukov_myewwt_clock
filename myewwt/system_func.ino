void Ticker_Circular_Callback(void) {

  struct {
    //цвета в ргб диапазоне для передачи на светодиоды
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
    uint8_t userBright_clock; //яркость (0-255)
  } forEEPROM;
  EEPROM.get(1030, forEEPROM);

  second++;


  if (forEEPROM.autoBright_clock) //если включена автояркость
  {
    SetBrightness(); //функция обработки автояркости
    showLed(); //показываем цифры
  }

  LED = !LED;
  if (LED) {
    pixels.setPixelColor(14, pixels.Color(forEEPROM.R, forEEPROM.G, forEEPROM.B));
    pixels.setPixelColor(15, pixels.Color(forEEPROM.R, forEEPROM.G, forEEPROM.B));
    pixels.show();

  }
  else {
    pixels.setPixelColor(14, pixels.Color(0, 0, 0));
    pixels.setPixelColor(15, pixels.Color(0, 0, 0));
    pixels.show();

  }

  if (second == 60) {
    second = 0;
    minute++;
    if (!forEEPROM.autoBright_clock) //если отключена автояркость (иначе будет вызываться два раза)
      showLed();
  }
  if (minute == 60) {
    minute = 0;
    hour++;
    showLed();
  }
  if (hour == 24)  {
    hour = 0;
    if (!forEEPROM.autoBright_clock)
      showLed();
  }
}

void first_start() {
  EEPROM.write(1, 4); //если запуск первый, записываем ключ

  uint32_t userid[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //для хранения ID пользователей

  mode_flag = 1; //включаем режим сервера

  bool light[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //флаг включения/отключения пункта меню в клавиатуре устройства освещения
  char light_name[10][41]; //название устройства освещения
  uint8_t lightAddress[10][6]; //mac-адрес устройства освещения
  bool sock[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //флаг включения/отключения пункта меню в клавиатуре розеток
  char sock_name[10][41]; //название розетки
  uint8_t socketAddress[10][6]; //mac-адрес розетки
  char token[60];  //Telegram token
  char ssid[35];
  char pass[65];

  //записываем стандартные значения всех данных в EEPROM
  EEPROM.put(2, userid);
  EEPROM.put(44, lightAddress);
  EEPROM.put(106, light_name);
  EEPROM.put(520, light);
  EEPROM.put(532, socketAddress);
  EEPROM.put(595, sock_name);
  EEPROM.put(1010, sock);
  EEPROM.put(1040, mode_flag);
  EEPROM.put(1045, ssid);
  EEPROM.put(1085, pass);
  EEPROM.put(1160, token);
  EEPROM.put(1460, first_user_flag);

  EEPROM.commit(); //для окончания записи
}


void reset_settings() {
  EEPROM.write(1, 0); //записываем ключ для сброса всех данных в EEPROM
  EEPROM.end();
  delay(100);
  ESP.restart();
}

void update_firm() {
  Serial.println("Пробуем обновиться");
  WiFiClient client;

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, "http://www.cg10019.tmweb.ru/myewwt_v10.bin");
  client.stop();

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());

      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");

      break;

    case HTTP_UPDATE_OK:
      Serial.println("UPDATE_OK");
      break;
  }
}


String light_menu() {
  String menu;
  bool light[10];
  char light_name[10][41];
  EEPROM.get(106, light_name);
  EEPROM.get(520, light);

  if (light[0]) {
    menu += light_name[0];
    menu += " \n";
  }
  if (light[1]) {
    menu += light_name[1];
    menu += " \n";
  }
  if (light[2]) {
    menu += light_name[2];
    menu += " \n";
  }
  if (light[3]) {
    menu += light_name[3];
    menu += " \n";
  }
  if (light[4]) {
    menu += light_name[4];
    menu += " \n";
  }
  if (light[5]) {
    menu += light_name[5];
    menu += " \n";
  }
  if (light[6]) {
    menu += light_name[6];
    menu += " \n";
  }
  if (light[7]) {
    menu += light_name[7];
    menu += " \n";
  }
  if (light[8]) {
    menu += light_name[8];
    menu += " \n";
  }
  if (light[9]) {
    menu += light_name[9];
    menu += " \n";
  }
  menu += "Назад";
  return (menu);
}

String sock_menu() {

  String menu;

  bool sock[10];
  char sock_name[10][41];
  EEPROM.get(595, sock_name);
  EEPROM.get(1010, sock);

  if (sock[0]) {
    menu += sock_name[0];
    menu += " \n";
  }
  if (sock[1]) {
    menu += sock_name[1];
    menu += " \n";
  }
  if (sock[2]) {
    menu += sock_name[2];
    menu += " \n";
  }
  if (sock[3]) {
    menu += sock_name[3];
    menu += " \n";
  }
  if (sock[4]) {
    menu += sock_name[4];
    menu += " \n";
  }
  if (sock[5]) {
    menu += sock_name[5];
    menu += " \n";
  }
  if (sock[6]) {
    menu += sock_name[6];
    menu += " \n";
  }
  if (sock[7]) {
    menu += sock_name[7];
    menu += " \n";
  }
  if (sock[8]) {
    menu += sock_name[8];
    menu += " \n";
  }
  if (sock[9]) {
    menu += sock_name[9];
    menu += " \n";
  }
  menu += "Назад";
  return (menu);
}


void ConnectWIFI() {
  char ssid[35];
  char pass[65];
  EEPROM.get(1045, ssid);
  EEPROM.get(1085, pass);
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.begin(ssid, pass); //подключаемся к точке досупа
  byte count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    count ++;
    if (count > 20) {
      break;
    }
  }
}

//функция синхронизации времени по ntp
void UpdateTime() {

  time_t now;
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  year = timeinfo->tm_year;
  hour = timeinfo->tm_hour;
  minute = timeinfo->tm_min;
  second = timeinfo->tm_sec;
}
