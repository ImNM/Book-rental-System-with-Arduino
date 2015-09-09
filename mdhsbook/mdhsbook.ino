/**************************************************************************/
/*!
@file     readMifare.pde
@author   Adafruit Industries
@license  BSD (see license.txt)

This example will wait for any ISO14443A card or tag, and
depending on the size of the UID will attempt to read from it.

If the card has a 4-byte UID it is probably a Mifare
Classic card, and the following steps are taken:

- Authenticate block 4 (the first block of Sector 1) using
the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
- If authentication succeeds, we can then read any of the
4 blocks in that sector (though only block 4 is read here)

If the card has a 7-byte UID it is probably a Mifare
Ultralight card, and the 4 byte pages can be read directly.
Page 4 is read by default since this is the first 'general-
purpose' page on the tags.


This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout
----> https://www.adafruit.com/products/364

Check out the links above for our tutorials and wiring diagrams
These chips use SPI or I2C to communicate.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal.h>


// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (2)
#define PN532_RESET (3)  // Not connected by default on the NFC Shield
// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:
// Or use this line for a breakout or shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4); // lcd pin 12 11 7- 6 5 4
int green = 10; //green led 10pin
int spk = 9;  // spk 9 pin
int red = 8;  //red led 8pin


void setup(void) {
	Serial.begin(115200);
	Serial.println("Hello!");
	tone(spk, 1400, 200);

	pinMode(green, OUTPUT);
	pinMode(red, OUTPUT);
	// lcd print ------------------------------------------------------------/
	char arr1[12] = { "book rental" };
	char arr2[7] = { "system" };
	int i = 0;

	lcd.begin(16, 2);

	for (i = 0; i < 11; i++) {
		lcd.write(arr1[i]);
		delay(100);
	}

	lcd.setCursor(0, 1);
	for (i = 0; i < 6; i++){
		lcd.write(arr2[i]);
		delay(100);
	}
	//-----------------------------------------------------------------------/

	nfc.begin();

	uint32_t versiondata = nfc.getFirmwareVersion();
	if (!versiondata) {
		Serial.print("Didn't find PN53x board");
		while (1); // halt
	}
	// Got ok data, print it out!
	Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
	Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
	Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

	// configure board to read RFID tags
	nfc.SAMConfig();

	Serial.println("Waiting for an ISO14443A Card ...");
}


void loop(void) {
	uint8_t success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
	uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	char taguid[9];
	unsigned long n;
	// Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
	// 'uid' will be populated with the UID, and uidLength will indicate
	// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

	if (success) {

		sprintf(taguid, "%X%X%X%X", uid[3], uid[2], uid[1], uid[0]);// sprintf 사용 배열값을 합쳐버림 ex) e0ed108f
		n = strtoul(taguid, NULL, 16);//unsigned long 형을 반환하여 int형의 범위를 벗어나서 호환
	    
		//n = (int)strtol(taguid, NULL, 16);
		// Display some basic information about the card

		Serial.println("Found an ISO14443A card");
		Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
		Serial.print("  UID Value: ");
		nfc.PrintHex(uid, uidLength);
		Serial.println(n);
		Serial.println(taguid);
		//-------------------------------------------------------------------------------------------------------
		//시각적,청각적 효과
		digitalWrite(green, HIGH);
		tone(spk, 1400, 200);
		noTone(spk);
		digitalWrite(green, LOW);
		lcd.clear();
		lcd.print(taguid);
		lcd.setCursor(0, 1);
		lcd.print(n);
		delay(1000);
		char arr1[12] = { "book rental" };
		char arr2[7] = { "system" };
		int i = 0;
		lcd.clear();
		for (i = 0; i < 11; i++) {
			lcd.write(arr1[i]);
			delay(100);
		}

		lcd.setCursor(0, 1);
		for (i = 0; i < 6; i++){
			lcd.write(arr2[i]);
			delay(100);
		}
		//-------------------------------------------------------------------------------------------------------

		if (uidLength == 4)
		{
			// We probably have a Mifare Classic card ... 
			Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

			// Now we need to try to authenticate it for read/write access
			// Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
			Serial.println("Trying to authenticate block 4 with default KEYA value");
			uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

			// Start with block 4 (the first block of sector 1) since sector 0
			// contains the manufacturer data and it's probably better just
			// to leave it alone unless you know what you're doing

		}
		else
		{
			digitalWrite(red, HIGH);//-----------------------------------------------------------------
			tone(spk, 400, 200);
			noTone(spk);
			tone(spk, 400, 200);
			noTone(spk);
			digitalWrite(red, LOW);//------------------------------------------------------------------
			Serial.println("Ooops ... authentication failed: Try another key?");
		}
	}
}

