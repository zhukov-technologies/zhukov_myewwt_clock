void newMsg(FB_msg& msg) {

  char main_menu[] = "Освещение \xF0\x9F\x92\xA1 \n"
                     "Розетки \xF0\x9F\x94\x8C \n"
                     "Температура \xF0\x9F\x8C\xA1 \n"
                     "Влажность \xF0\x9F\x92\xA7 \n"
                     "Цвет часов \xF0\x9F\x8C\x88 \n"
                     "Яркость часов \xF0\x9F\x94\x86 \n"
                     "Настройки \xE2\x9A\x99 \n"
                     "У меня проблема \xF0\x9F\x91\x80";

  if (first_user_flag) { //если добавляем первого пользователя
    int32_t userid[10]; //объявляем массив с ID добавленных пользователей
    EEPROM.get(2, userid); //считываем из EEPROM

    first_user_flag = 0; //сбрасываем флаг
    userid[0] = strtol(msg.chatID.c_str(), NULL, 10); //добавляем первого пользователя в массив

    //записываем в EEPROM
    EEPROM.put(2, userid);
    EEPROM.put(1460, first_user_flag);
    EEPROM.commit();

    bot.showMenuText("Добро пожаловать в систему управления умным домом! %0A%0AСохраняем ваш ID. Перезагрузка...", main_menu, msg.chatID);
    delay(1000);
    ESP.restart();
    goto bailout; //для выхода из ветки if else
  }


  if (del_user_flag) {
    if (strcmp(msg.text.c_str(), "Назад") == 0) {
      del_user_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);

      goto bailout;
    }

    else {
      int32_t userid[10];
      uint32_t x = 0;
      uint8_t index;
      x = strtol(msg.text.c_str(), NULL, 10);
      EEPROM.get(2, userid);
      for (int i = 0; i < 10; i++) {
        if ((x == userid[i]) && (x != 0)) {
          index = i;
          break;
        }
        else {
          index = 11;
        }
      }
      if (index != 11) {
        userid[index] = 0;
        EEPROM.put(2, userid);
        EEPROM.commit();
        bot.showMenuText("Пользователь успешно удалён. Перезагрузка...", main_menu, msg.chatID);
        del_user_flag = false;
        delay(1000);
        ESP.restart();
        goto bailout;
      }
      else {
        index = 11;
        bot.sendMessage("Пользователь не найден. Проверьте ID.", msg.chatID);

        goto bailout;
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////УДАЛЕНИЕ РОЗЕТКИ/////////////////////////////////////////////////////////////////////////
  else if (del_sock_flag) {
    if  (strcmp(msg.text.c_str(), "Назад") == 0) {
      del_sock_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);

      goto bailout;
    }
    else {
      bool sock[10]; //флаг включения/отключения пункта меню в клавиатуре розеток
      char sock_name[10][41]; //название розетки
      uint8_t socketAddress[10][6]; //mac-адрес розетки
      EEPROM.get(532, socketAddress);
      EEPROM.get(595, sock_name);
      EEPROM.get(1010, sock);
      uint8_t index;
      for (int i = 0; i < 10; i++) {
        if (strcmp(msg.text.c_str(), sock_name[i]) == 0) {
          index = i;
          break;
        }
        else {
          index = 11;
        }
      }
      if (index != 11) {

        sock[index] = 0;
        strcpy(sock_name[index], "");
        for (int i = 0; i < 6; i++) {
          socketAddress[index][i] = 0;
        }

        EEPROM.put(532, socketAddress);
        EEPROM.put(595, sock_name);
        EEPROM.put(1010, sock);
        EEPROM.commit();
        bot.showMenuText("Устройство успешно удалено", main_menu, msg.chatID);
        del_sock_flag = false;

        goto bailout;
      }
      else {
        bot.sendMessage("Устройство не найдено. Проверьте название.", msg.chatID);

        goto bailout;
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////УДАЛЕНИЕ УСТРОЙСТВА ОСВЕЩЕНИЯ/////////////////////////////////////////////////////////////////////////
  else if (del_light_flag) {
    if  (strcmp(msg.text.c_str(), "Назад") == 0) {
      del_light_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);

      goto bailout;
    }
    else {
      uint8_t index;
      bool light[10]; //флаг включения/отключения пункта меню в клавиатуре устройства освещения
      char light_name[10][41]; //название устройства освещения
      uint8_t lightAddress[10][6]; //mac-адрес устройства освещения
      EEPROM.get(44, lightAddress);
      EEPROM.get(106, light_name);
      EEPROM.get(520, light);
      for (int i = 0; i < 10; i++) {

        if (strcmp(msg.text.c_str(), light_name[i]) == 0) {
          index = i;
          break;
        }
        else {
          index = 11;
        }
      }
      if (index != 11) {

        light[index] = 0;
        strcpy(light_name[index], "");
        for (int i = 0; i < 6; i++) {
          lightAddress[index][i] = 0;
        }

        EEPROM.put(44, lightAddress);
        EEPROM.put(106, light_name);
        EEPROM.put(520, light);
        EEPROM.commit();

        bot.showMenuText("Устройство успешно удалено", main_menu, msg.chatID);
        del_light_flag = false;

        goto bailout;
      }
      else {
        bot.sendMessage("Устройство не найдено. Проверьте название.", msg.chatID);

        goto bailout;
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////ДОБАВЛЕНИЕ ВЫКЛЮЧАТЕЛЯ/////////////////////////////////////////////////////////////////////////
  else if (new_switch_flag) {
    if (strcmp(msg.text.c_str(), "Назад") == 0) {
      new_switch_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);

      goto bailout;
    }
    else {
      uint8_t index;
      char light_name[10][41]; //название устройства освещения
      uint8_t lightAddress[10][6]; //mac-адрес устройства освещения
      EEPROM.get(44, lightAddress);
      EEPROM.get(106, light_name);

      for (int i = 0; i < 10; i++) {

        if (strcmp(msg.text.c_str(), light_name[i]) == 0) {
          index = i;
          break;
        }
        else {
          index = 11;
        }
      }
      if (index != 11) {
        for (int i = 0; i < 6; i++) {
          to_switch.ID = 5;
          to_switch.Address[i] = lightAddress[index][i];
        }
        switch_flag = true;
        bot.sendMessage("Зажмите и удерживайте кнопку на выключателе в течение 7 секунд", msg.chatID);

        goto bailout;
      }
      else {
        bot.sendMessage("Устройство не найдено. Проверьте название.", msg.chatID);

        goto bailout;
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////УПРАВЛЕНИЕ РОЗЕТКАМИ/////////////////////////////////////////////////////////////////////////
  char sock_name[10][41]; //название розетки
  EEPROM.get(595, sock_name);
  for (byte i = 0; i < 10; i++) {
    if (strcmp(msg.text.c_str(), sock_name[i]) == 0) {
      struct {
        byte ID = 1;
        byte value = 2;
        char WIFI_ROUTER_SSID[35] = "";
        char WIFI_ROUTER_PASS[65] = "";
      } myData_send;

      EEPROM.get(1045, myData_send.WIFI_ROUTER_SSID);
      EEPROM.get(1085, myData_send.WIFI_ROUTER_PASS);
      answer_flag = true;
      answer_ID = msg.chatID;
      uint8_t socketAddress[10][6]; //mac-адрес розетки
      EEPROM.get(532, socketAddress);
      esp_now_send(socketAddress[i], (uint8_t *) &myData_send, sizeof(myData_send));
      yield();
      goto bailout;
    }
  }
  /////////////////////////////////////////////////////////////////////////УПРАВЛЕНИЕ ОСВЕЩЕНИЕМ/////////////////////////////////////////////////////////////////////////
  char light_name[10][41]; //название устройства освещения
  uint8_t lightAddress[10][6]; //mac-адрес устройства освещения
  EEPROM.get(44, lightAddress);
  EEPROM.get(106, light_name);
  for (byte i = 0; i < 10; i++) {

    if (strcmp(msg.text.c_str(), light_name[i]) == 0) {
      struct {
        byte ID = 1;
        byte value = 2;
        char WIFI_ROUTER_SSID[35] = "";
        char WIFI_ROUTER_PASS[65] = "";
      } myData_send;
      EEPROM.get(1045, myData_send.WIFI_ROUTER_SSID);
      EEPROM.get(1085, myData_send.WIFI_ROUTER_PASS);
      answer_flag = true;
      answer_ID = msg.chatID;
      esp_now_send(lightAddress[i], (uint8_t *) &myData_send, sizeof(myData_send));
      yield();
      goto bailout;
    }
  }
  /////////////////////////////////////////////////////////////////////////ДОБАВЛЕНИЕ НОВОГО ПОЛЬЗОВАТЕЛЯ/////////////////////////////////////////////////////////////////////////
  if (new_id_flag) {
    uint32_t x = 0;
    x = strtol(msg.text.c_str(), NULL, 10);
    if  (strcmp(msg.text.c_str(), "Назад") == 0) {
      new_id_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);
    }

    else if ((x > 3999999999) || (x < 999)) {
      bot.sendMessage("Неправильный ID.", msg.chatID);
    }
    else {

      int32_t userid[10];
      uint8_t index;
      EEPROM.get(2, userid);
      for (int i = 1; i < 10; i++) {

        if (userid[i] == 0) {
          index = i;
          break;
        }
        else if (userid[i] != 0) {
          index = 11;
        }
      }
      if (index != 11) {
        userid[index] = x;
        EEPROM.put(2, userid);
        EEPROM.commit();

        bot.showMenuText("Пользователь успешно добавлен! Перезагрузка...", main_menu, msg.chatID);
        new_id_flag = false;
        String newUser = "";
        newUser += x;
        bot.sendMessage("Поздравляем! Теперь вы можете управлять системой умного дома.%0A%0AПодождите, пока MYEWWT перезагрузится, затем напишите любое сообщение и мы начнём...", newUser);
        delay(1000);
        ESP.restart();
      }
      else if (index == 11)
      {
        bot.sendMessage("Все места заняты.", msg.chatID);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////СБРОС НАСТРОЕК/////////////////////////////////////////////////////////////////////////
  else if (reset_settings_flag) {
    if (strcmp(msg.text.c_str(), "Назад") == 0) {
      new_id_flag = false;
      new_sock_flag = false;
      new_light_flag = false;
      reset_settings_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);
    }

    else if (strcmp(msg.text.c_str(), "Да") == 0) {

      EEPROM.write(1, 0); // key for writing start data
      EEPROM.commit();
      bot.showMenuText("Все настройки сброшены. Перезагрузка...", main_menu, msg.chatID);

      delay(100);

      ESP.restart();

    }
  }
  /////////////////////////////////////////////////////////////////////////ДОБАВЛЕНИЕ РОЗЕТКИ И УСТРОЙСТВА ОСВЕЩЕНИЯ/////////////////////////////////////////////////////////////////////////
  else if ((new_sock_flag) || (new_light_flag)) {

    if  (strcmp(msg.text.c_str(), "Назад") == 0) {
      new_id_flag = false;
      new_sock_flag = false;
      new_light_flag = false;
      bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);
    }

    else {

      if (strlen(msg.text.c_str()) < 42)
      {
        if (new_sock_flag) {
          uint8_t index;
          bool sock[10]; //флаг включения/отключения пункта меню в клавиатуре розеток
          char sock_name[10][41]; //название розетки
          uint8_t socketAddress[10][6]; //mac-адрес розетки
          EEPROM.get(532, socketAddress);
          EEPROM.get(595, sock_name);
          EEPROM.get(1010, sock);
          for (int i = 0; i < 10; i++) {

            if (sock[i] == 0) {
              index = i;
              break;
            }
            else {
              index = 11;
            }
          }
          if (index != 11) {
            sock[index] = 1;

            strcpy(sock_name[index], msg.text.c_str());

            for (int i = 0; i < 6; i++) {
              socketAddress[index][i] = broadcastAddress[i];
            }
            bot.showMenuText("Розетка успешно добавлена", main_menu, msg.chatID);
            EEPROM.put(532, socketAddress);
            EEPROM.put(595, sock_name);
            EEPROM.put(1010, sock);
            EEPROM.commit();
            new_sock_flag = false;
          }
          else if (index == 11)
          {
            new_sock_flag = false;
            bot.showMenuText("Всё место занято. Если хотите добавить ещё, воспользуйтесь кнопкой 'У меня проблема' в главном меню.", main_menu, msg.chatID);
          }
        }

        if (new_light_flag) {
          uint8_t index;
          bool light[10]; //флаг включения/отключения пункта меню в клавиатуре устройства освещения
          char light_name[10][41]; //название устройства освещения
          uint8_t lightAddress[10][6]; //mac-адрес устройства освещения
          EEPROM.get(44, lightAddress);
          EEPROM.get(106, light_name);
          EEPROM.get(520, light);
          for (int i = 0; i < 10; i++) {

            if (light[i] == 0) {
              index = i;
              break;
            }
            else {
              index = 11;
            }
          }
          if (index != 11) {
            light[index] = 1;

            strcpy(light_name[index], msg.text.c_str());
            for (int i = 0; i < 6; i++) {

              lightAddress[index][i] = broadcastAddress[i];
            }
            bot.showMenuText("Устройство освещения успешно добавлено", main_menu, msg.chatID);

            EEPROM.put(44, lightAddress);
            EEPROM.put(106, light_name);
            EEPROM.put(520, light);
            EEPROM.commit();
            new_light_flag = false;
          }
          else if (index == 11)
          {
            new_light_flag = false;
            bot.showMenuText("Всё место занято. Если хотите добавить ещё, воспользуйтесь кнопкой 'У меня проблема' в главном меню.", main_menu, msg.chatID);
          }
        }
      }

      else {
        bot.sendMessage("Слишком длинное название. Введите менее 20 символов.", msg.chatID);
      }
    }
  }
  /////////////////////////////////////////////////////////////////////////ОСНОВНОЕ МЕНЮ/////////////////////////////////////////////////////////////////////////
  else if  (strcmp(msg.text.c_str(), "Назад") == 0) {

    new_id_flag = false;
    bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Влажность \xF0\x9F\x92\xA7") == 0) {

    String message;
    byte humidity = dht.readHumidity();
    message += "Влажность ";
    message += humidity;
    message += "%";
    bot.sendMessage(message.c_str(), msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Температура \xF0\x9F\x8C\xA1") == 0) {
    String message;
    byte temp = dht.readTemperature();
    message += "Температура ";
    message += temp;
    message += "°С";
    bot.sendMessage(message.c_str(), msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Цвет часов \xF0\x9F\x8C\x88") == 0) {
    char color_menu[] =  "Зелёный \t Синий \n"
                         "Красный \t Жёлтый \n"
                         "Оранжевый \t Голубой \n"
                         "Фиолетовый \t Белый \n"
                         "Назад";
    bot.showMenuText("\xF0\x9F\x8C\x88", color_menu, msg.chatID);
  }
  else if (strcmp(msg.text.c_str(), "Розетки \xF0\x9F\x94\x8C") == 0) {
    bot.showMenuText("\xF0\x9F\x94\x8C", sock_menu(), msg.chatID);


  }
  else if (strcmp(msg.text.c_str(), "Освещение \xF0\x9F\x92\xA1") == 0) {
    bot.showMenuText("\xF0\x9F\x92\xA1", light_menu(), msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Яркость часов \xF0\x9F\x94\x86") == 0) {
    char bright_menu[] =  "Минимальная \n"
                          "Средняя \n"
                          "Максимальная \n"
                          "Автоматическая \n"
                          "Назад";
    bot.showMenuText("\xF0\x9F\x94\x86", bright_menu, msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Настройки \xE2\x9A\x99") == 0) {
    char settings_menu[] =  "Настроить розетку \n"
                            "Удалить розетку \n"
                            "Настроить устройство освещения \n"
                            "Удалить устройство освещения \n"
                            "Настроить выключатель \n"
                            "Добавить пользователя \n"
                            "Удалить пользователя \n"
                            "Сбросить все настройки \n"
                            "Назад";
    bot.showMenuText("\xE2\x9A\x99", settings_menu, msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "У меня проблема \xF0\x9F\x91\x80") == 0) {
    bot.sendMessage("Здесь будет ссылка на бота-помощника или связь с поддержкой", msg.chatID);

  }
  /////////////////////////////////////////////////////////////////////////ЯРКОСТЬ ЧАСОВ/////////////////////////////////////////////////////////////////////////
  else if (strcmp(msg.text.c_str(), "Минимальная") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.userBright_clock = 10;
    forEEPROM.autoBright_clock = 0;
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();
    pixels.setBrightness(forEEPROM.userBright_clock);
    showLed();
    bot.sendMessage("Теперь яркость на минимуме", msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Средняя") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.userBright_clock = 80;
    forEEPROM.autoBright_clock = 0;
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();
    pixels.setBrightness(forEEPROM.userBright_clock);
    showLed();
    bot.sendMessage("Теперь яркость средняя", msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Максимальная") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.userBright_clock = 170;
    forEEPROM.autoBright_clock = 0;
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();
    pixels.setBrightness(forEEPROM.userBright_clock);
    showLed();
    bot.sendMessage("Теперь яркость на максимуме", msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Автоматическая") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.autoBright_clock = 1;
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();
    bot.sendMessage("Теперь яркость устанавливается автоматически", msg.chatID);

  }
  /////////////////////////////////////////////////////////////////////////ЦВЕТ ЧАСОВ/////////////////////////////////////////////////////////////////////////
  else if (strcmp(msg.text.c_str(), "Зелёный") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 0;
    forEEPROM.G = 255;
    forEEPROM.B = 0;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся зелёным!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Синий") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 0;
    forEEPROM.G = 0;
    forEEPROM.B = 255;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся синим!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Красный") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 255;
    forEEPROM.G = 0;
    forEEPROM.B = 0;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся красным!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Жёлтый") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 255;
    forEEPROM.G = 255;
    forEEPROM.B = 0;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся жёлтым!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Оранжевый") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 255;
    forEEPROM.G = 140;
    forEEPROM.B = 0;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся оранжевым!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Голубой") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 0;
    forEEPROM.G = 191;
    forEEPROM.B = 255;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся голубым!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Фиолетовый") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 128;
    forEEPROM.G = 0;
    forEEPROM.B = 128;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся фиолетовым!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  else if (strcmp(msg.text.c_str(), "Белый") == 0) {
    struct {
      //цвета в ргб диапазоне для передачи на светодиоды
      uint8_t R;
      uint8_t G;
      uint8_t B;
      uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
      uint8_t userBright_clock; //яркость (0-255)
    } forEEPROM;
    EEPROM.get(1030, forEEPROM);

    forEEPROM.R = 255;
    forEEPROM.G = 255;
    forEEPROM.B = 255;
    showLed();
    bot.sendMessage("Отлично, теперь часы светятся белым!", msg.chatID);
    EEPROM.put(1030, forEEPROM);           EEPROM.commit();

  }
  /////////////////////////////////////////////////////////////////////////НАСТРОЙКИ/////////////////////////////////////////////////////////////////////////
  else if (strcmp(msg.text.c_str(), "Настроить устройство освещения") == 0)  {
    bot.sendMessage("Включите устройство зажмите кнопку на нём на 7 секунд.", msg.chatID);
    new_device_ID = msg.chatID;
    light_flag = true;

  }
  else if (strcmp(msg.text.c_str(), "Удалить устройство освещения") == 0) {
    bot.showMenuText("Выберите устройство, которое хотите удалить: ", light_menu(), msg.chatID);
    del_light_flag = 1;

  }
  else if (strcmp(msg.text.c_str(), "Удалить розетку") == 0) {
    bot.showMenuText("Выберите розетку, которую хотите удалить: ", sock_menu(), msg.chatID);
    del_sock_flag = 1;

  }
  else if (strcmp(msg.text.c_str(), "Настроить розетку") == 0)  {
    bot.sendMessage("Включите розетку и зажмите кнопку на ней на 7 секунд.", msg.chatID);
    new_device_ID = msg.chatID;
    sock_flag = true;

  }
  else if (strcmp(msg.text.c_str(), "Настроить выключатель") == 0)  {
    new_switch_flag = true;
    new_device_ID = msg.chatID;
    bot.showMenuText("С каким устройством освещения связать выключатель?", light_menu(), msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Добавить пользователя") == 0) {
    new_id_flag = true;
    bot.sendMessage("Введите ID пользователя:", msg.chatID);

  }
  else if (strcmp(msg.text.c_str(), "Удалить пользователя") == 0) {

    del_user_flag = true;
    String message;
    message += "Введите ID пользователя, которого хотите удалить. %0A%0AДобавленные ID:%0A";
    int32_t userid[10];
    EEPROM.get(2, userid);
    for (int i = 1; i < 10; i++) {
      if (userid[i] != 0) {
        message += userid [i];
        message += "%0A";
      }
    }
    bot.sendMessage(message.c_str(), msg.chatID);
  }
  else if (strcmp(msg.text.c_str(), "Сбросить все настройки") == 0) {
    char yes_menu[] = "Да \n Назад";
    bot.showMenuText("ВНИМАНИЕ! Все настройки будут сброшены, информация о токене бота, логине и пароле Wi-Fi и добавленных устройствах будет удалена.%0AСбросить все настройки?", yes_menu, msg.chatID);
    reset_settings_flag = 1;

  }
  else  {
    bot.showMenuText("\xF0\x9F\x8C\xBE", main_menu, msg.chatID);

  }
bailout:
  {

  }
}
