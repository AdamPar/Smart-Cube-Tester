#include <Arduino_ConnectionHandler.h>

#if defined(BOARD_HAS_WIFI)
  #define SECRET_WIFI_SSID "some_wifi"
  #define SECRET_WIFI_PASS "passwd"
#endif

/* ESP8266 ESP32 */
#if defined(BOARD_HAS_SECRET_KEY)
  #define SECRET_DEVICE_KEY "secret_key"
#endif
