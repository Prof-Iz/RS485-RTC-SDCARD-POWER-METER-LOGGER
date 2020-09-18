#include <EEPROM.h>

#include <SPI.h>

#include <SD.h>

#include <RTClib.h>



/* Librarires used in the folowing program are:
    a fork of JeeLab's RTC library,
    Physical Hardware uses DS1307.
    Double check whether the battery is CR1220 3V.
    Format SD Card to FAT16
*/

RTC_DS1307 rtc;

File data;

char filename[10]; //to store the updating filename
char time_stamp[5];


void setup() {
  EEPROM.write(0, 0); // stores the day of the month, start with zero so setup is okay
  Serial.begin(9600);
  while (!Serial) {}; // debugging

  if (!rtc.begin()) {
    Serial.println("RTC Error");
    abort(); // Prevent logging if the clock fails or SD not intialised
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!SD.begin(10)) {
    Serial.println("SD CARD ERROR");
  }

  // later change SD and rtc begin to same abort line

}


void loop() {
  DateTime now = rtc.now(); // snapshot of current time
  sprintf(time_stamp, "%i:%i", now.hour(), now.minute()); // prepare timestamp OR maybe do w modbus in one go?

  if (now.day() != EEPROM[0]) { // only remake file name for each day
    sprintf(filename, "%i%i%i.txt", now.year(), now.month(), now.day());
//    Serial.println(filename);
    EEPROM[0] = now.day();
    
  }

  if (data) {
    data = SD.open(filename, FILE_WRITE);
    // req and write modbus





    //

    data.close(); // to ensure that its saved properly
    delay(60000); // a minute delay
  }




}
