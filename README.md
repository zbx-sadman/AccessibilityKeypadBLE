# AccessibilityKeypadBLE

This project helps to use physical buttons or keypad with Android TalkBack or Android Switch Access technologies via BLE.

### Features
- Acts as BLE Keyboard HID device;
- Can send complex shortcuts (up to 6 keys) with key modificators (Alt, Ctrl, Shift);
- Pairing feature is available.

### You need
- ESP32 board;
- A number of buttons;
- A few wires;
- AccessibilityKeypadBLE firmware.

### How to make device
- Connect buttons between ESP32 GPIO's and GND;
- Configure keyboard shortcuts in the sketch as you wish;
- Compile firmware with Arduino IDE (1.8.9 is ok) + Espressif Arduino core for the ESP32 1.0.4;
- Upload firmware to the ESP32 board; 
- Connect BLE device "AccessPad BLE" to the Android device;
- Make settings for Android Switch Access feature, if need; 
- Use it.

### Device schematic

![Schematic](https://user-images.githubusercontent.com/12827470/111063811-c4308200-84c1-11eb-9621-f7102ec3c3a7.png)

### Device prototype (another dev-board used)

![Prototype](https://user-images.githubusercontent.com/12827470/111041727-b68be580-844a-11eb-8dbc-0f94b9277f78.png)

### Bluetooth/BLE device connection dialog

![Connect device to Android](https://user-images.githubusercontent.com/12827470/111041734-c1467a80-844a-11eb-8451-ab689d014fc5.png)
