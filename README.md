# ESP8266_RFID_OLED
# 12/12/2016  ML
# No license.  Open to the public domain.  Please respect
#  liceses of sources.

Simple code for reading a Sunfounder RFID-RC522 card on a
NodeMCU board displaying to a 128x64 I2C OLED.  This was
written for d.Tech/OEF intersession December 2016.

Libraries used:
 https://github.com/miguelbalboa/rfid
 https://github.com/adafruit/Adafruit_SSD1306

Note, you will need to adjust the Adafruit_SSD1306.h file
in the library for the 128x64 display the first time.

THe code reads the card UID and TYPE (copied from Miguel's
examples) and displays it on the OLED.  It waits d specified
period (5 seconds by default) between card reads.  It also
logs actions to serial for debugging purposes.

