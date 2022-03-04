//основная страница сервера с формой для заполнения wifi и токена

void mainForm() {

  String Page;
  Page += F(
            "<!DOCTYPE html><html lang='ru'>"

            "<head><meta name='viewport' content='width=device-width'>"
            "<title>MYEWWT</title><meta http-equiv='Content-type' content='text/html; charset=utf-8'>"
            "<style type='text/css'>"
            "body {"
            "margin: 0;"
            "width: 100vw;"
            "height: 100vh;"
            "background: #f3ecf3;"
            "display: flex;"
            "align-items: center;"
            "text-align: center;"
            "justify-content: center;"
            "place-items: center;"
            "overflow: hidden;"
            "}"

            ".container {"
            "position: relative;"
            "width: auto;"
            "height: auto;"
            "border-radius: 20px;"
            "padding: 40px;"
            "box-sizing: border-box;"
            "background: #f1e9e9;"
            "box-shadow: 14px 14px 20px #cbced1, -14px -14px 20px white;"
            "}"

            ".brand-logo {"
            "height: 100px;"
            "width: 100px;"
            "background: url(data:image/jpeg;base64,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);"
            "background-repeat: no-repeat;"
            "background-size: contain;"
            "margin: auto;"
            "border-radius: 50%;"
            "box-sizing: border-box;"
            "box-shadow: 7px 7px 10px #cbced1, -7px -7px 10px white;"
            "}"

            ".brand-title {"
            "margin-top: 15px;"
            "font-weight: 900;"
            "font-size: 1.8rem;"
            "color: #bfa5bf;"
            "letter-spacing: 1px;"
            "}"

            ".inputs {"
            "text-align: left;"
            "margin-top: 15px;"
            "}"

            "label, input, button {"
            "display: block;"
            "width: 100%;"
            "padding: 0;"
            "border: none;"
            "outline: none;"
            "box-sizing: border-box;"
            "}"

            "label {"
            "margin-bottom: 4px;"
            "margin-top: 12px;"
            "}"


            "input::placeholder {"
            "color: gray;"
            "}"

            "input {"
            "background: #f3ecf3;"
            "padding: 10px;"
            "padding-left: 20px;"
            "height: 50px;"
            "font-size: 14px;"
            "border-radius: 50px;"
            "box-shadow: inset 6px 6px 6px #d0cbd1, inset -6px -6px 6px white;"
            "}"

            "button {"
            "margin-top: 40px;"
            "background: #f1e9e9;"
            "color: #6f6cde;"
            "height: 40px;"
            "border-radius: 20px;"
            "cursor: pointer;"
            "font-weight: 750;"
            "box-shadow: 6px 6px 6px #d1cbd1, -6px -6px 6px white;"
            "transition: 0.5s;"
            "}"

            "button:hover {"
            "opacity: 0.3;"
            "box-shadow: -6px -6px 10px rgba(255, 255, 255, 0.8),"
            "6px 6px 10px rgba(0, 0, 0, 0.2);"
            "}"


            "button:active {"
            "opacity: 1;"
            "box-shadow: inset -4px -4px 8px rgba(255, 255, 255, 0.5),"
            "inset 8px 8px 16px rgba(0, 0, 0, 0.1);"
            "color: #79e3b6;"
            "}"

            "a {"
            "position: absolute;"
            "font-size: 8px;"
            "bottom: 4px;"
            "right: 4px;"
            "text-decoration: none;"
            "color: black;"
            "background: yellow;"
            "border-radius: 10px;"
            "padding: 2px;"
            "}"

            "h1 {"
            "position: absolute;"
            "top: 0;"
            "left: 0;"
            "}"

            "</style>"
            "</head>"
            "<div class='container'>"
            "<div class='brand-logo'></div>"
            "<div class='brand-title'>MYEWWT</div>"
            "<div class='inputs'>"
            "<form method='post' action='/save'>"
            "<label>Wi-Fi</label>"
            "<input type='text' name='ssid' required placeholder='TP-Link_06E0' />"
            "<label>Пароль</label>"
            "<input type='password' name='pass' required placeholder='12345678'/>"
            "<label>Токен бота</label>"
            "<input type='text' name='token' required placeholder='1780888739:AAE6yWzPT3psfjjR7DxXSZsc3cW5cnleArg' />"
            "<button type='submit'>Сохранить</button>"
            "</form>"
            "</div>"
            "</div>");

  webServer.send(200, "text/html", Page);
}

void confirmPage() {

  String Page;
  Page += F(
            "<!DOCTYPE html><html lang='ru'>"

            "<head><meta name='viewport' content='width=device-width'>"
            "<title>MYEWWT</title><meta http-equiv='Content-type' content='text/html; charset=utf-8'>"
            "<style type='text/css'>"
            "body {"
            "margin: 0;"
            "width: 100vw;"
            "height: 100vh;"
            "background: #f3ecf3;"
            "display: flex;"
            "align-items: center;"
            "text-align: center;"
            "justify-content: center;"
            "place-items: center;"
            "overflow: hidden;"
            "}"

            ".container {"
            "position: relative;"
            "width: auto;"
            "height: auto;"
            "border-radius: 20px;"
            "padding: 40px;"
            "box-sizing: border-box;"
            "background: #f1e9e9;"
            "box-shadow: 14px 14px 20px #cbced1, -14px -14px 20px white;"
            "}"

            ".brand-logo {"
            "height: 100px;"
            "width: 100px;"
            "background: url(data:image/jpeg;base64,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);"
            "background-repeat: no-repeat;"
            "background-size: contain;"
            "margin: auto;"
            "border-radius: 50%;"
            "box-sizing: border-box;"
            "box-shadow: 7px 7px 10px #cbced1, -7px -7px 10px white;"
            "}"

            ".brand-title {"
            "margin-top: 15px;"
            "font-weight: 900;"
            "font-size: 1.8rem;"
            "color: #bfa5bf;"
            "letter-spacing: 1px;"
            "}"

            "</style>"
            "</head>"
            "<div class='container'>"
            "<div class='brand-logo'></div>"
            "<div class='brand-title'>Сохранено. Перезагрузка...</div>"
            "</div>");

  webServer.send(200, "text/html", Page);

}

//функция при нажатии кнопки "сохранить" на сервере
void handleForm() {
  char token[60];  //Telegram token
  char ssid[35];
  char pass[65];

  //записываем данные из формы в глобальные переменные
  webServer.arg("ssid").toCharArray(ssid, sizeof(ssid) - 1);
  webServer.arg("pass").toCharArray(pass, sizeof(pass) - 1);
  webServer.arg("token").toCharArray(token, sizeof(token) - 1);

  bool mode_flag_eeprom = 0; //включаем обычный режим (выключаем режим сервера)

  //записываем переменные в EEPROM

  EEPROM.put(1040, mode_flag_eeprom);
  EEPROM.put(1045, ssid);
  EEPROM.put(1085, pass);
  EEPROM.put(1160, token);
  EEPROM.put(1460, first_user_flag);
  EEPROM.commit();

  webServer.sendHeader("Location", "/confirm", true); //Redirect to our html web page
  webServer.send(302, "text/plane", "");
reset_tmr.start();
}
