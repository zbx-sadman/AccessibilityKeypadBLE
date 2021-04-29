# Accessibility Keypad BLE / OTA Ready release

Вы можете испытать работу этой прошивки загрузив ее в ESP32 через стандартный скетч OTAWebUpdater, включенный в примеры библиотеки ArduinoOTA, поставляемой с фреймворком [arduino-esp32](https://github.com/espressif/arduino-esp32).

Участники программы тестирования получают полный доступ к функциональности устройства.

### Возможности
В прошивке доступны:
- Звуковое сопровождение действий системы (загрузка, активация Web-конфигуратора, сброс настроек, сопряжение по Bluetooth);
- Представление физических кнопок как клавиш HID-клавиатуры (с модификаторами);
- Поддержка PS/2 клавиатуры и представление её клавиш как клавиш HID-клавиатуры (с модификаторами);
- Ускоренная перепосылка кодов HID-клавиатуры при удержании физической кнопки или клавиши PS/2;
- Переход в режим энергосбережения и пробуждение по нажатию управляющей кнопки;
- Сопряжение Bluetooth по запросу;
- Web-интерфейс для конфигурирования системы, созданный с учетом требований доступности (Web accessibility);
- Парольная защита подключения в режиме точки доступа Wi-Fi (для конфигурирования);
- Возможность подключения ко внешней точке доступа Wi-Fi (для конфигурирования);
- OTA-обновление прошивки;
- Импорт/экспорт конфигурации.

### Схема соединений

...
- Buzzer - GPIO26
- Control Button - GPIO13
- PS/2 Keyboard data - GPIO5
- PS/2 Keyboard clock - GPIO4
...

### Снимки интерфейса
AccessKeypad BLE присоединен к Android как BLE-клавиатура

![AccessKeypad BLE connected as bluetooth keyboard](https://user-images.githubusercontent.com/12827470/116542647-8ebae900-a8f5-11eb-9237-d2da0069326f.jpg)

AccessKeypad BLE доступен как Wi-Fi точка доступа для изменения настроек системы

![AccessKeypad BLE acts as access point](https://user-images.githubusercontent.com/12827470/116542781-b8741000-a8f5-11eb-840f-3ebbb12d8ce2.jpg)

Настройки системы, доступные в Web-конфигураторе.

![AccessKeypad BLE Web Configurator screenshoot 03](https://user-images.githubusercontent.com/12827470/116542872-d80b3880-a8f5-11eb-8c27-2a707251d780.jpg)
![AccessKeypad BLE Web Configurator screenshoot 02](https://user-images.githubusercontent.com/12827470/116542874-d8a3cf00-a8f5-11eb-95ac-ddb6f25b9f7b.jpg)


![AccessKeypad BLE Web Configurator screenshoot 01](https://user-images.githubusercontent.com/12827470/116542877-d93c6580-a8f5-11eb-9367-ce0151d450b9.jpg)
![AccessKeypad BLE Web Configurator screenshoot 04](https://user-images.githubusercontent.com/12827470/116542869-d772a200-a8f5-11eb-8251-fe0b20cda9bd.jpg)




