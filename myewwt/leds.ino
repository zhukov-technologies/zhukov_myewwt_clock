
void SetBrightness() {

  
int bfac = 0; //отфильтрованное значение яркости
int readout = 512; //для фильтра
int analogPhoto = 0; //показания с датчика



  analogPhoto = analogRead(A0); //читаем аналоговый вход

  if (abs(analogPhoto - readout) > 90) {
    readout = analogPhoto; //lowpass filtering
  } else {
    readout = (readout * 7 + analogPhoto) >> 3;
  }

  bfac = map(readout, BRIGHTNESS_ADC_LOW, BRIGHTNESS_ADC_HIGH, BRIGHTNESS_LOW, BRIGTHNESS_HIGH); //конвертируем
  if (bfac < BRIGHTNESS_LOW) bfac = BRIGHTNESS_LOW;
  if (bfac > BRIGTHNESS_HIGH) bfac = BRIGTHNESS_HIGH;
  pixels.setBrightness(bfac); //устанавливаем яркость по датчику с учетом указанного min & max
  
}

//для режима радуги
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//функция вывода радуги
void rainbow() {

static int pixelCycle = 0;


  for (uint16_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, Wheel((i + pixelCycle) & 255)); //  Update delay time
  }
  pixels.show();                             //  Update strip to match
  pixelCycle++;                             //  Advance current cycle
  if (pixelCycle >= 256)
    pixelCycle = 0;                         //  Loop the cycle back to the begining
}

//функция вывода цифр
void showLed() {


  //разделяем время по одной цифре
  byte digit1 = hour / 10;
  byte digit2 = hour % 10;
  byte digit3 = minute / 10;
  byte digit4 = minute % 10;

  struct {
  //цвета в ргб диапазоне для передачи на светодиоды
  uint8_t R;
  uint8_t G;
  uint8_t B;
  uint8_t autoBright_clock; //флаг для подстройки яркости по фоторезистору
  uint8_t userBright_clock; //яркость (0-255)
} forEEPROM;
  EEPROM.get(1030, forEEPROM);



  //слева направо
  //первая цифра
  for (int k = 0; k < 7; k++) {
    if (pgm_read_byte(&num[digit1][k]) == 1) {
      pixels.setPixelColor(k, pixels.Color(forEEPROM.R, forEEPROM.G, forEEPROM.B));
    }
    else if (pgm_read_byte(&num[digit1][k]) == 0) {
      pixels.setPixelColor(k, pixels.Color(0, 0, 0));
    }
  }

  //вторая цифра
  for (int k = 0; k < 7; k++) {
    if (pgm_read_byte(&num[digit2][k]) == 1) {
      pixels.setPixelColor(k + 7, pixels.Color(forEEPROM.R, forEEPROM.G, forEEPROM.B));
    }
    else if (pgm_read_byte(&num[digit2][k]) == 0) {
      pixels.setPixelColor(k + 7, pixels.Color(0, 0, 0));
    }
  }

  //третья цифра
  for (int k = 0; k < 7; k++) {
    if (pgm_read_byte(&num[digit3][k]) == 1) {
      pixels.setPixelColor(k + 16, pixels.Color(forEEPROM.R, forEEPROM.G, forEEPROM.B));
    }
    else if (pgm_read_byte(&num[digit3][k]) == 0) {
      pixels.setPixelColor(k + 16, pixels.Color(0, 0, 0));
    }
  }

  //четвёртая цифра
  for (int k = 0; k < 7; k++) {
    if (pgm_read_byte(&num[digit4][k]) == 1) {
      pixels.setPixelColor(k + 23, pixels.Color(forEEPROM.R, forEEPROM.G, forEEPROM.B));
    }
    else if (pgm_read_byte(&num[digit4][k]) == 0) {
      pixels.setPixelColor(k + 23, pixels.Color(0, 0, 0));
    }
  }
  
  //зажигаем светодиоды
  pixels.show();
}
