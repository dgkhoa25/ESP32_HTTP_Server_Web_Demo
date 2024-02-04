This project shows how to use the Wi-Fi Station functionality of the Wi-Fi driver of ESP for connecting to an Access Point.

## How to use

### Configure the project

* Set the Wi-Fi configuration.
  * #define EXAMPLE_ESP_WIFI_SSID      "yourSSID"
  * #define EXAMPLE_ESP_WIFI_PASS      "yourPassword"

### Build and Flash

Build the project and flash it to the board, then run the monitor tool to view the serial output:

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)
