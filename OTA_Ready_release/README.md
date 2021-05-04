# Accessibility Keypad BLE / OTA Ready release

Вы можете испытать работу этой прошивки загрузив ее в ESP32 через стандартный скетч OTAWebUpdater, включенный в примеры библиотеки ArduinoOTA, поставляемой с фреймворком [arduino-esp32](https://github.com/espressif/arduino-esp32).

Участники программы тестирования получают полный доступ к функциональности устройства. Пожалуйста, заявите о своем участии на e-mail zbx.sadman@gmail.com

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
- Status led - GPIO2
- Control Button - GPIO13
- Passive buzzer module- GPIO26
- PS/2 Keyboard clock - GPIO4
- PS/2 Keyboard data - GPIO5

Клавиатура PS/2 соединяется через конвертор логических уровней 3.3V <=> 5V.

![AccessKeypad BLE wiring](https://user-images.githubusercontent.com/12827470/116598033-078a6700-a92f-11eb-9609-62784febb2f1.png)

### Управление
1) Включение устройства: выключить напряжение, дождаться однократного звукового сигнала.
2) Переключение в режим Wb-конфигурирования: удерживать при включении кнопку "Управление" (Control button) до двойного звукового сигнала, затем сразу отпустить. 
3) Сброс конфигурации на значения по умолчанию: удерживать при включении кнопку "Управление" (Control button) до тройного звукового сигнала, затем отпустить.
4) Сопряжение AccessKeypad BLE с Android-устройством: во время работы нажать и удерживать кнопку "Управление" (Control button) не менее 2 сек. После начала звуковой и световой сигнализации произвести сопряжение в течении 15 сек.
5) Выключение устройства: просто выключить напряжение.

### Режимы "On hold" клавиш/кнопок-источников
Для даннного режима доступны следующие действия:
- "Do nothing on hold". События нажатия и отпускания клавиши- или кнопки-источника передаются напрямую устройству, к которому подключен AccessKeypad BLE. Операционная система самостоятельно определяет действие (повтор/однократное выполнение) связанной клавиши-цели;
- "Repeat on hold". При удержании клавиши- или кнопки-источника AccessKeypad BLE имитирует для подключенного устройства нажатия и отпускания связанной клавиши-цели через заданный интервал;
- "One shoot on hold". При удержании клавиши- или кнопки-источника AccessKeypad BLE производит имитацию однократного быстрого нажатия и отпускания связанной клавиши-цели. Повтор имитации доступен после отпускания и последующего нажатия клавиши-источника.

### Полезные советы
- При использовании клавиши/кнопки-источника, на которую назначена целевая функция "Play/Pause", в приложениях Android может быть доступна следующая функциональность: 
  - Короткое нажатие на заблокированном/разблокированном экране останавливает или продолжает воспроизведение медиа-файла;
  - Продолжительное нажатие разблокированном экране вызывает Google Assistant (голосовой помощник).
- Воспользуйтесь режимом "One shoot on hold" для однократного действия, связанного с клавишей-источником, если Вы испытываете проблему со своевременным отпусканием клавиши/кнопки-источника. 

### Прототип (PS/2 model)
![AccessKeypad BLE prototype PS/2 keypad connected](https://user-images.githubusercontent.com/12827470/116737006-57356500-a9f9-11eb-8157-6ea62c39171c.jpg)

Раскладка Talkback для данного блока клавиш:
  - "ESC" - TalkBack start/stop
  - "F1" - Browser 
  - "F2" - Assistent 
  - "F3" - Notifications
  - "7"  - First item
  - "8" - Home screen
  - "9" - Read from top item
  - "<-" - Tab
  - "4" - Previous item
  - "5" - Click on focused
  - "6" - Next item
  - "F4" - Previous heading
  - "Enter" - Next heading
  - "1" - Back
  - "2" - Previous window
  - "9" - Read from next item
  - "0" - Stop TalkBack speech
  - "Dot" - TalkBack menu

### Прототип (GPIO model)
Однокнопочный орган управления для Android Switch Access. Использование USB-разъёма обусловлено конструктивной особенностью корпуса для Raspberry Pi - в нем уже существует соответствующее отверстие. Внутри "большой кнопки" D+ замыкается на GND, а VCC не используется.
![AccessKeypad BLE prototype GPIO button connected](https://user-images.githubusercontent.com/12827470/116976162-766a1600-acc9-11eb-8251-cef65a3f9312.jpg)

![Using AccessKeypad BLE with SwitchAccess technology](https://user-images.githubusercontent.com/12827470/116979006-20976d00-accd-11eb-901d-80b6cc14dc68.jpg)

### Снимки интерфейса
AccessKeypad BLE присоединен к Android как BLE-клавиатура

![AccessKeypad BLE connected as bluetooth keyboard](https://user-images.githubusercontent.com/12827470/116915711-120d6f00-ac55-11eb-80b9-f99faab173e7.png)

AccessKeypad BLE доступен как Wi-Fi точка доступа для изменения настроек системы

![AccessKeypad BLE acts as access point](https://user-images.githubusercontent.com/12827470/116915724-13d73280-ac55-11eb-9d52-8600a591cac8.png)

Настройки системы, доступные в Web-конфигураторе.

![AccessKeypad BLE Web Configurator screenshoot 01](https://user-images.githubusercontent.com/12827470/116915729-16398c80-ac55-11eb-968d-3cab4fc4a234.png)
![AccessKeypad BLE Web Configurator screenshoot 02](https://user-images.githubusercontent.com/12827470/116915734-18035000-ac55-11eb-92ba-cd77d581295c.png)


![AccessKeypad BLE Web Configurator screenshoot 03](https://user-images.githubusercontent.com/12827470/116915741-19cd1380-ac55-11eb-8504-6f99880d43fb.png)
![AccessKeypad BLE Web Configurator screenshoot 04](https://user-images.githubusercontent.com/12827470/116915746-1c2f6d80-ac55-11eb-93ba-f13a5e52eb9a.png)

### Изменения в версиях
#### 0.2
- Убрана глобальная опция "Enable repeat on keypress", вместо нее введены индивидуальные режимы режимы "... on hold" для клавиш/кнопок-источников;
- В список клавиш-целей добавлены мультимедийные кнопки Volume+, Volume-, Mute, Play/Pause, Next Track, Previous Track, Fast Forward, Rewind.
