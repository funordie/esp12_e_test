#include "ds18b20.hpp"
#include "mqtt.hpp"
#include "wifi.hpp"

#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;
float temperature;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("\n Starting...");

	// see if the card is present and can be initialized:
	if (!SD.begin(chipSelect)) {
		Serial.println("Card failed, or not present");
	}

	setup_wifi();
	setup_mqtt();
	setup_temperature();

	Serial.println("exit setup");
}

void loop() {

	// put your main code here, to run repeatedly:
	loop_mqtt();

	if(!loop_temperature(&temperature)) {
		//valid temperature

		publish_temperature_mqtt(temperature);

		//write temperature
		// open the file. note that only one file can be open at a time,
		// so you have to close this one before opening another.
		File dataFile = SD.open("datalog.txt", FILE_WRITE);
		// if the file is available, write to it:
		if (dataFile) {
			String dataString = "";
			dataString += String(temperature);
			dataFile.println(dataString);
			dataFile.close();
			// print to the serial port too:
			Serial.println(dataString);
		}
		// if the file isn't open, pop up an error:
		else {
			Serial.println("error opening datalog.txt");
		}
	}
}
