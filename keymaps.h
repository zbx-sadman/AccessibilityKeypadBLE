#if defined(KEYMAP_SWITCH_ACCESS_ANY_BOARD)
// Switch Access controls. You can use just one button
keyMap_t keyMap[] = {
  12, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Enter
};
#endif

/*
  TalkBack keymap
  https://support.google.com/accessibility/android/answer/6110948?hl=en#zippy=%2Cdefault-keymap
*/

#if defined(KEYMAP_TALKBACK_MH_ET_LIVE_MINIKIT_ESP32)
  // For MH-ET LIVE D1 mini ESP32
  /*
  https://i0.wp.com/www.bizkit.ru/wp-content/uploads/2019/12/MH-ET_LIVE_D1_mini_ESP32_pinout.png
  GPIO # | Board mark | Description               | Control         | Shortcut / gesture
  2      | IO2        | State                     | LED (Onboard)   |
  27     | IO27       | Home                      | Button (White)  | Alt+Crl+h
  32     | IO32       | Prev object               | Button (Green)  | Alt+Left  / swipe left
  12     | TDI        | Enter                     | Button (Yellow) | Alt+Enter / double tap
  17     | IO17       | Next Object               | Button (Red)    | Alt+Right / swipe right
  16     | IO16       | Read from the next object | Button (Blue)   | Alt+Ctrl+Shift+Enter
  15     | TD0        |
  13     | TCK        |
  14     | TMS        |
  33     | IO33       |
  26     | IO26       |
  */
keyMap_t keyMap[] = {
  27, (KEY_ALT | KEY_CTRL), 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,             // Alt+Crl+h
  32, KEY_ALT, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Left
  //12, KEY_ALT, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Enter
  17, KEY_ALT, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Right
  16, (KEY_ALT | KEY_CTRL | KEY_SHIFT), 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Alt+Ctrl+Shift+Enter
};
#endif

#if defined(KEYMAP_TALKBACK_ESP32_WROOM_32)
  keyMap_t keyMap[] = {
  12, (KEY_ALT | KEY_CTRL), 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,             // Alt+Crl+h
  13, KEY_ALT, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Left
  14, KEY_ALT, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Enter
  26, KEY_ALT, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,                          // Alt+Right
  27, (KEY_ALT | KEY_CTRL | KEY_SHIFT), 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, // Alt+Ctrl+Shift+Enter
  };
#endif

