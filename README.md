# ESP8266_RFID_OLED.ino
## 12/12/2016  Mike Lance

##### No license.  Open to the public domain.  Please respect liceses of sources.

Simple code for reading a Sunfounder RFID-RC522 card on a
NodeMCU board displaying to a 128x64 I2C OLED.  This was
written for d.Tech/OEF intersession December 2016.

Libraries used:
[miguelbalboa/rfid](https://github.com/miguelbalboa/rfid)
[adafruit/Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

Note, you will need to adjust the Adafruit_SSD1306.h file
in the library for the 128x64 display the first time.

The code reads the card UID and TYPE (copied from Miguel's
examples) and displays it on the OLED.  It waits a specified
period (5 seconds by default) between card reads.  It also
logs actions to serial for debugging purposes.

Wiring table
Sunfounder SPI RFID-RC522 board
Signal      GPIO         NodeMCU       MFRC522 board
------------------------------------------------------------
Reset        2             D4             RST
SPI SS       15            D8             NSS
SPI MOSI     13*           D7             MOSI
SPI MISO     12*           D6             MISO
SPI SCK      14*           D5             SCK
----         NC            NC             IRQ (not used here)

I2C SSD1306 OLED board
Signal      GPIO         NodeMCU        OLED board
------------------------------------------------------------
I2C SCL      5*            D1             SCL
I2C SDA      4*            D2             SDA

Note the * indicates that these pins are fixed.  For SPI, it's fixed
 in the ESP library.  for I2C, the Adafruit library does not give a
 method to redefine it (although other OLED libraries do).

