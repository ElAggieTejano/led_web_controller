# Overview

`led_web_controller.ino` is an Arduino sketch for the ESP32 Dev board that sets up the board as a Wi-Fi access point and renders a website. Once a device (such as a phone or laptop) connects to the ESP32's network, users can navigate to `192.98.24.1` in a web browser to access a simple interface for controlling two sets of WS2811 LED strips.

By default, the sketch will flash ESP32 Dev board to drive RGB LEDs (like WS2811) at GPIO5 (P5) and GPIO18 (P18).

# Setup

1. Download Arduino IDE from its official website.
2. If using Arduino IDE for Linux (installer or AppImage) make sure to add default user or users to dialout group:
  ```sh
  $ sudo usermod -a -G dialout user-name
  $ reboot # reboot required for user modifications to take effect
  ```

  * Optional: if using AppImage on Linux, perform the following as default user:
    ```sh
    $ chmod +x arduino-ide_2.3.6_Linux_64bit.AppImage
    $ ./arduino-ide_2.3.6_Linux_64bit.AppImage
    ```

3. Launch Arduino IDE and open the Board Manager.
4. In the search box, search for `esp32`, select `esp32 by Espressif Systems`, and install the board manager.
5. Next, open the Library Manager, search for `FastLED`, select `FastLED by Daniel Garcia`, and install the library.
6. Next, clone this project down to your host device.
7. In Arduino IDE, go to File > Open and select the recently cloned project.
8. At this time, user should plugin ESP32 board.
9. With the project opened, navigate to Tools > Board > esp32 and select ESP32 Dev Module.
  * If using a different board, ensure to select the correct board.
10. With the board detected by the IDE and the user being part of the dialout group, click on the upload button on the top left hand side of the IDE to flash the ino sketch to the board.
11. Once the IDE has finished flashing the board, connect a browser capable device (laptop or phone) and connect to the Wifi emitted by the ESP32 board.
  * SSID = LED_Controller
  * Password = 12345678
12. With a device successfully connected to the ESP32's network, launch a browser on the connected device.
13. In the URL, enter `192.98.24.1` to access the HTML site hosted by the ESP32.
14. Browser should then present the user with a series of buttons to control LEDs connected to the ESP32.