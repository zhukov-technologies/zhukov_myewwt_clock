void send_answer(byte answer) {

  switch (answer) {

    case 0:
      bot.sendMessage("Выключено! \xF0\x9F\x8C\x91", answer_ID);
      break;
    case 1:
      bot.sendMessage("Включено! \xE2\x9A\xA1", answer_ID);
      break;
    case 2:
      bot.closeMenuText("Обнаружено новое устройство. Придумайте ему название: ", new_device_ID);
      break;
    case 3:
      bot.showMenuText("Выключатель успешно настроен",
                       "Освещение \xF0\x9F\x92\xA1 \n"
                       "Розетки \xF0\x9F\x94\x8C \n"
                       "Температура \xF0\x9F\x8C\xA1 \n"
                       "Влажность \xF0\x9F\x92\xA7 \n"
                       "Цвет часов \xF0\x9F\x8C\x88 \n"
                       "Яркость часов \xF0\x9F\x94\x86 \n"
                       "Настройки \xE2\x9A\x99 \n"
                       "У меня проблема \xF0\x9F\x91\x80", new_device_ID);
      break;
    case 4:
      bot.sendMessage("Ошибка. Попробуйте ещё раз.", answer_ID);
      break;
    default:
      break;
  }
}


void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if (answer_flag) { //если был поднят флаг запроса состояния розетки

    switch (myData.value) {
      case 0:
        esp_now_mode = 0;
        esp_now_flag = true;
        answer_flag = false;
        break;
      case 1:
        esp_now_mode = 1;
        esp_now_flag = true;
        answer_flag = false;
        break;
      default:
        break;
    }
  }
  else if (sock_flag && (myData.ID == 0)) { //если добавлем розетку

    esp_now_mode = 2;
    esp_now_flag = true;

    //получаем в массив broadcastAddress mac отправителя
    for (int i = 0; i < 6; i++) {
      broadcastAddress[i] = mac_addr[i];
    }

    new_sock_flag = true; //подтверждаем поднятием флага
    sock_flag = false; //сбрасываем текущий

  }



  else if (light_flag && (myData.ID == 0)) { //если добавлем устройство освещения

    esp_now_mode = 2;
    esp_now_flag = true;

    //получаем в массив broadcastAddress mac отправителя
    for (int i = 0; i < 6; i++) {
      broadcastAddress[i] = mac_addr[i];
    }
    new_light_flag = true;
    light_flag = false;
  }
  else if (switch_flag && (myData.ID == 2)) {

    esp_now_send(mac_addr, (uint8_t *) &to_switch, sizeof(to_switch));
    new_switch_flag = false;
  }
  else if (switch_flag && (myData.ID == 3)) {
    esp_now_mode = 3;
    esp_now_flag = true;
    switch_flag = false;
  }
}

//функция при отправке данных
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if (sendStatus != 0) {
    esp_now_mode = 4;
    esp_now_flag = true;
  }
}
