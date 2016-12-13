/* 12/12/2016 ML
 *  Currently configured for NodeMCU/Wemos board, tested on 1.6.12
 *  Using Adafruit_SD1306 library for OLED
 *  Deleted the mfrc522 write test, just display that there's a card!
 */
 
/**
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/mfrc522
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * This sample shows how to read and write data blocks on a MIFARE Classic PICC
 * (= card/tag).
 * 
 * BEWARE: Data will be written to the PICC, in sector #1 (blocks #4 to #7).
 * 
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 * 
 */

/* Wiring table 
 * RFID board
 * Signal      GPIO         NodeMCU       MFRC522 board
 * ------------------------------------------------------------
 * Reset        2             D4             RST
 * SPI SS       15            D8             NSS
 * SPI MOSI     13*           D7             MOSI
 * SPI MISO     12*           D6             MISO
 * SPI SCK      14*           D5             SCK
 * ----         NC            NC             IRQ (not used here)
 * 
 * I2C SSD1306 OLED board  -- This is HARD defined in Adafruit's libraries.
 * Signal      GPIO         NodeMCU        OLED board
 * ------------------------------------------------------------
 * I2C SCL      5*            D1             SCL
 * I2C SDA      4*            D2             SDA
 */

// Program variables
uint8_t scanDelay = 5;              // 5 second delay between reads

// Libraries for RFID (mfrc522)
#include <SPI.h>
#include <MFRC522.h>
// Libraries for OLED
//#include "SSD1306.h"
// #include <Adafruit_GFX.h>        // Not using anything fancy here
#include <Adafruit_SSD1306.h>

// Instantiate mfrc522
#define RST_PIN         D4          // Configurable, see typical pin layout above
#define SS_PIN          D8          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

// Instantiate OLED - Adafruit Library.  Can't define SCL/SDA :(
#define OLED_RESET -1               // This is presented on Adafruit boards, not on cheap eBay units.
Adafruit_SSD1306 display(OLED_RESET);

/**
 * Initialize.
 */
void setup() {
    Serial.begin(115200);

    Serial.println();
    Serial.println("Initializing SPI...");
    SPI.begin();                   // Init SPI bus
    Serial.println("Initializing RFID...");
    mfrc522.PCD_Init();            // Init MFRC522 card
    Serial.println("Initializing OLED...");
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);   // Adafruit...and get their logo from .cpp
    display.display();
    delay(1000);
    
    display.clearDisplay();

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate basic recognition."));
    Serial.print(F("Using key (for A and B):"));
    serial_dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();
}

/**
 * Main loop.
 */
void loop() {
    display.setTextColor(WHITE, BLACK);
    display.setTextSize(1);
    display.setCursor(0,0);

    display.println("Scanning...");
    display.println();
    display.display();
    
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    serial_dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    display.println("Card UID: ");
    oled_dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    display.println();
    Serial.print(F("PICC type: "));
    display.println(("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    display.println(mfrc522.PICC_GetTypeName(piccType));
    display.display();

    // Halt PICC
    mfrc522.PICC_HaltA();

    Serial.print("Delay scanning...");
    delay(scanDelay * 1000);       //Turn seconds into milliseconds
    Serial.println(" ...Ready to scan again!");
    display.clearDisplay();
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void serial_dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}    
void oled_dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        display.print(buffer[i] < 0x10 ? " 0" : " ");
        display.print(buffer[i], HEX);
    }
}

