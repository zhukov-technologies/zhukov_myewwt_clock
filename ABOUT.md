- [Умный дом на основе API Telegram](#smart_home)
- [Прошивка](#firmware)
- - [Безопасность соединений](#secure)
- - - [Соединение сервер-MYEWWT](#server-myewwt)
- - - [Соединение MYEWWT-устройства](#myewwt-device)
- - [Конвертер из Юникода](#unicode)

<a id="smart_home"></a>
# Умный дом на основе API Telegram

Принцип работы системы умного дома grib tehcnology на основе API Telegram.

1. Пользователь создаёт своего бота в Telegram при помощи BotFather ([Создание бота в Telegram](https://github.com/grib-technology/grib_myewwt_clock#bot).
2. Получив токен созданного бота, пользователь сообщает устройству ([Miss You Even When We're Together](https://github.com/grib-technology/grib_myewwt_clock)) (MYEWWT) токен бота а также SSID и пароль Wi-Fi сети ([Настройка MYEWWT](https://github.com/zbltrz/MYEWWT#myewwt_setup)).
3. Отправляя сообщение боту, оно обрабатывается устройством MYEWWT.
4. Если в систему добавлены дополнительные устройства — умные розетки, реле и т.д., команда от бота обрабатывается сначала устройством MYEWWT, затем передаётся подключенным в систему устройствам с применением технологии [ESP-NOW](https://www.espressif.com/en/products/software/esp-now/overview).

![Frame 44](https://user-images.githubusercontent.com/84660518/156764174-f941a3c9-73b9-44ff-b16c-c1ca3817e657.png)

<a id="firmware"></a>
# Прошивка

Проект начал свое существование в среде Arduino IDE с применением SDK от компании Espressif для микроконтроллера ESP-12F, затем получил развитие в среде VS Code с применением набора инструментов PlatformIO.

Большая часть кода проекта проккоментирована и разбита на удобные части в виде нескольких файлов: 
* основная, с общим циклом и объявлением глобальных переменных и некоторых функций,
* для функций, вызываемых при отправке и приеме сообщений через [ESP-NOW](https://www.espressif.com/en/products/software/esp-now/overview),
* функции, используемые для работы адресных светодиодов,
* блок с функциями для работы сервера ([captive portal](https://ru.wikipedia.org/wiki/Captive_portal)),
* функция-обработчик сообщений с бота Telegram,
* блок с прочими системнымми функциями: отсчёт времени, обвновление прошивки, синхронизация времени и т.д.


<a id="secure"></a>
## Безопасность соединений

В системе умного дома grib technology используются два типа соединений:

1. От сервера Telegram к шлюзу MYEWWT.
2. От шлюза MYEWWT к прочим устройствам системы (розетки, реле, выключатели и т.д.).

<a id="server-myewwt"></a>
### Соединение сервер-MYEWWT

Все аспекты безопасности самого API Telegram, а конкретно защита профиля, защита чатов и т.д. описаны [здесь](https://core.telegram.org/#security).

Между серверами Telegram и MYEWWT устанавливается соединение по защищённому каналу связи, это гарантирует протокол SSL, т.е. сами сервера Telegram не дают возможности подключиться и получить запрос от бота, без использования SSL шифрования. 

В коде прошивки благодаря [BearSSL](https://bearssl.org/) (реализация протокола SSL на языке C) создаётся клиент, с помощью которого мы имеем возможность получать и отправлять запросы к API Telegram, конкретно к указанному в прошивке (при настройке MYEWWT) боту. 

Чтобы другой пользователь, получивший ссылку на бота под управлением MYEWWT или обнаружив его в поиске не смог получить доступ к нему, в коде прошивки создаётся white list, где находятся ID пользователей. ID — уникальный номер аккаунта в Telegram, его невозможно подделать и изменить. Таким образом, при настройке MYEWWT, доступ получает первый человек, обратившийся к боту, его ID сохраняется в энергонезависимой памяти, и возможность обмена сообщений закрепляется за данным пользователем, сообщения от других пользователей просто отсеиваются. 

Также в коде прошивки есть возможность выдать правда доступа к MYEWWT другим пользователям — достаточно в настройках выбрать меню "Добавить пользователя" и указать его ID.

Подводя итог, можно сделать вывод о безопасности подобного соединения. Чтобы иметь несанкционированный доступ к боту — придется получить доступ к аккаунту владельца MYEWWT, что сделать довольно сложно, а при должных настройках безопасности Telegram профиля — практически невозможно.

<a id="myewwt-device"></a>
### Соединение MYEWWT-устройства

Внутри дома прочие устройства, такие как розетки, реле, выключатели и т.д. не подключены к Wi-Fi и не связаны с интернетом (за исключеним попыток связи с сервером для обновления прошивки раз в n часов)

Связь между MYEWWT и другими устройствами передаётся с помощью [ESP-NOW](https://www.espressif.com/en/products/software/esp-now/overview) — встроенного в SDK от Espressif метода передачи данных. Данное соединение одноранговое, не требует времязатратного "рукопожатия" а также обеспечено возможностью применения протокола [CCMP](https://ru.wikipedia.org/wiki/CCMP), алгоритма [AES-128](https://ru.wikipedia.org/wiki/AES_(%D1%81%D1%82%D0%B0%D0%BD%D0%B4%D0%B0%D1%80%D1%82_%D1%88%D0%B8%D1%84%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)).
Связь между устройствами устанавливается посредством отправки пакета на MAC-адрес устройства, либо с использованием широковещательной передачи. 

В документации указано ограниченное число соединений:

Максимум 20 пар, включая зашифрованные, поддерживаются на одном устройстве, включая зашифрованные пары.
Максимум 10 зашифрованных пар поддерживаются в режиме Station.
Максимум 6 в режиме SoftAP или SoftAP + Station.

А также указано о необходимости регистрации устройств на этапе компиляции прошивки:
```cpp
 esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  ```

Несмотря на это, было установлено, что без выполнения подобных рекомендаций, можно ограничиться лишь объявленим функций, вызываемых при принятии и отправке пакета:
```cpp
esp_now_register_recv_cb(OnDataRecv);
esp_now_register_send_cb(OnDataSent);
```  
и непосредственно функцией отправки пакета:
```cpp
esp_now_send(mac_addr, (uint8_t *) &to_switch, sizeof(to_switch));
  ```
 ограничиваясь, таким образом, лишь памятью ESP-12f, где хранятся MAC адреса добавленных устройств.
 
Также стоит отметить, чтобы функции ESP-NOW отрабатывались без ошибок, оба устройства должны быть установлены на один и тот же канал Wi-Fi. Устройства время от времени сканируют сеть, выбирают точку доступа MYEWWT, или точку доступа домашней сети Wi-Fi при недоступности сети MYEWWT, и настраиваются на её канал. При этом подключение к самой точке доступа не требуется. 


<a id="unicode"></a>
## Конвертер из Юникода

Помимо прочих, в проекте используются библиотека FastBot от AlexGyver, для удобного парсинга сообщений от бота. Функционал MYEWWT предполагает использование смайликов и киррилических символов, но при обращении с API Telegram обработчик получает Юникод, поэтому библиотека FastBot была доработана — дописана функция конвертации символов Юникода в UTF-8.

В википедии найдены [алгоритмы преобразования юникода в utf-8](https://ru.wikipedia.org/wiki/UTF-8#%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%BA%D0%BE%D0%B4%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F). Нужно определить, сколько октетов используется, в соответствии с этим создать шаблоны в битовом выражении, далее побитно включить или выключить нужные биты в шаблоне, затем с помощью логического умножения взять маской биты и включить, после этого преобразовать в последовательность символов HEX системы. В результате успешно получаем русский, индийский и прочие алфавиты.

С другими символами и смайлами ситуация обстоит немного иначе: UTF-8 может занимать и 4 бита, кодируя в принципе весь диапазон юникода, но во входящем сообщении от Telegram мы видим [суррогатные пары UTF-16](https://ru.wikipedia.org/wiki/UTF-16). Если коротко, цельные байты UTF-8 разбиваются на две пары — верхняя и нижняя часть суррогатной пары. Её нам нужно восстановить в цельный юникод по формуле, чтобы затем преобразовать в уже знакомую нам последовательность HEX в utf-8.

```cpp
String convertUnicode(String unicodeStr) {
  String out = "";
  int len = unicodeStr.length();
  char iChar;
  char* error;
  int32_t unicode_bytes; //переменная для числовой записи байтов юникода
  int32_t surr_bytes1; // верхняя часть суррогатной пары
  int32_t surr_bytes2; // нижняя часть суррогатной пары
  for (int i = 0; i < len; i++) {
    iChar = unicodeStr[i];
    if (iChar == '\\') {
      iChar = unicodeStr[++i];
      if (iChar == 'u') {
        char unicode[6];
        unicode[0] = '0';
        unicode[1] = 'x';
        for (int j = 0; j < 4; j++) {
          iChar = unicodeStr[++i];
          unicode[j + 2] = iChar;
        }

        unicode_bytes = strtol(unicode, &error, HEX); //получаем число - юникод в hex

        if ((unicode_bytes >= 0xD800) && (unicode_bytes <= 0xDBFF)) { //если находим первую кодовую единицу суррогатной пары
          surr_bytes1 = unicode_bytes;
        }
        
        else if ((unicode_bytes >= 0xDC00) && (unicode_bytes <= 0xDFFF)) { //если находим вторую кодовую единицу суррогатной пары

          surr_bytes2 = unicode_bytes;
          unicode_bytes = (0x10000 + ((surr_bytes1 - 0xD800) * 0x0400) + (surr_bytes2 - 0xDC00)); //собираем из суррогатной пары полный юникод

          //собираем по алгоритму для 4 октетов
          byte myByte1 = unicode_bytes & 0b111111;
          bitSet(myByte1, 7);
          bitClear(myByte1, 6);

          unicode_bytes >>= 6;
          byte myByte2 = unicode_bytes & 0b111111;
          bitSet(myByte2, 7);
          bitClear(myByte2, 6);

          unicode_bytes >>= 6;
          byte myByte3 = unicode_bytes & 0b111111;
          bitSet(myByte3, 7);
          bitClear(myByte3, 6);

          unicode_bytes >>= 6;
          byte myByte4 = unicode_bytes & 0b111;
          bitSet(myByte4, 7);
          bitSet(myByte4, 6);
          bitSet(myByte4, 5);
          bitSet(myByte4, 4);
          bitClear(myByte4, 3);

          out += (char)myByte4;
          out += "\0x";
          out += (char)myByte3;
          out += "\0x";
          out += (char)myByte2;
          out += "\0x";
          out += (char)myByte1;
        }

        else if (unicode_bytes < 0x800) { //если требуется 2 октета
          byte myByte1 = unicode_bytes & 0b111111;
          bitSet(myByte1, 7);
          bitClear(myByte1, 6);

          unicode_bytes >>= 6;
          byte myByte2 = unicode_bytes & 0b11111;
          bitSet(myByte2, 7);
          bitSet(myByte2, 6);
          bitClear(myByte2, 5);

          out += (char)myByte2;
          out += "\0x";
          out += (char)myByte1;
        }
        else if (unicode_bytes >= 0x800) { //если требуется три октета

          byte myByte1 = unicode_bytes & 0b111111;
          bitSet(myByte1, 7);
          bitClear(myByte1, 6);

          unicode_bytes >>= 6;
          byte myByte2 = unicode_bytes & 0b111111;
          bitSet(myByte2, 7);
          bitClear(myByte2, 6);
          
          unicode_bytes >>= 6;
          byte myByte3 = unicode_bytes & 0b1111;
          bitSet(myByte3, 7);
          bitSet(myByte3, 6);
          bitSet(myByte3, 5);
          bitClear(myByte3, 4);

          out += (char)myByte3;
          out += "\0x";
          out += (char)myByte2;
          out += "\0x";
          out += (char)myByte1;
        }
      } else if (iChar == '/') {
        out += iChar;
      } else if (iChar == 'n') {
        out += '\n';
      }
    } else {
      out += iChar;
    }
  }
  return out;
}
```