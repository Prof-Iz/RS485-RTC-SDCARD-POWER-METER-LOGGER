#include <ModbusMaster.h>

#include <EEPROM.h>

#include <SPI.h>

#include <SD.h>

#include <RTClib.h>

/* Librarires used in the folowing program are:
    a fork of JeeLab's RTC library,
    Physical Hardware uses DS1307.
    Double check whether the battery is CR1220 3V.
    Format SD Card to FAT16
    Modbus Master by Walker
*/

#define MAX485_DE 3
#define MAX485_RE_NEG 2 //data enable and receiver enable on TTL

RTC_DS1307 rtc;

File data;

ModbusMaster node;

char filename[10]; //to store the updating filename
char time_stamp[5];

int ID = 1; // Slave ID
uint8_t result;

//uint8_t freq; //store result from modbus
//uint8_t VLN;

int error_counter = 2;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  EEPROM.write(0, 0); // stores the day of the month, start with zero so setup is okay
  Serial.begin(9600); // check if baudrate functions properly.
  //   according to docs noisy, EMI, RFI environments should run 4800, 2400 baud.

  while (!Serial)
  {
  }; // debugging

  if (!rtc.begin() || !SD.begin(10))
  {
    Serial.println("Error");
    abort(); // Prevent logging if the clock fails or SD not intialised
  }

  if (!rtc.isrunning())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  node.begin(ID, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop()
{
  DateTime now = rtc.now(); // snapshot of current time
  //  sprintf(time_stamp, "%i:%i", now.hour(), now.minute()); // prepare timestamp OR maybe do w modbus in one go?

  if (now.day() != EEPROM[0])
  { // only remake file name for each day
    sprintf(filename, "%i%i%i.txt", now.year(), now.month(), now.day());
    //    Serial.println(filename);
    EEPROM[0] = now.day();
  }

  if (data)
  {
    data = SD.open(filename, FILE_WRITE);
    // req and write modbus

    do
    {                                              // give 2 chances to get proper reading.
      result = node.readInputRegisters(0x3911, 5); //read 5 registers from 0x3911 onwards
                                                   //    freq = node.read(0x3915); // check if reading seperately still accurate enough
                                                   //    VLN = node.read(0x3911);
      error_counter--;
    } while ((!node.ku8MBSuccess) and (error_counter > 0));

    error_counter = 2; //reset error counter

    if (node.ku8MBSuccess)
    {
      data.print(now.hour());
      data.print(":");
      data.print(now.minute());
      //      00:00

      data.print(",");
      data.print(node.getResponseBuffer(0x00)); // VLN
      data.print(",");
      data.println(node.getResponseBuffer(0x04)); // Freq
    }

    //

    data.close(); // to ensure that its saved properly
  }
  delay(60000); // a minute delay
}
