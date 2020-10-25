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
    Modbus Master by Doc Walker
*/

#define MAX485_DE 3
#define MAX485_RE_NEG 2 //data enable and receiver enable on TTL

RTC_DS1307 rtc;

File logger;

ModbusMaster node;

char filename[12]; //to store the updating filename

int ID = 1; // Slave ID

uint8_t result;

int error_counter = 2;

uint16_t data[2];

uint32_t value = 0;

double reading = 0;

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

float HexToFloat(uint32_t x) { //cconvert Hex uint32_t value to float under IEEE 754

  return (*(float*)&x);
}

void setup()
{
  //allow enable pins to be output
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);


  EEPROM.write(0, 0); // stores the day of the month, start with zero so setup is okay


  Serial.begin(9600, SERIAL_8E1); // baudrate 9600, 8bit, Even Parity, 1 Stop bit
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

  if (now.day() != EEPROM[0])
  { // only remake file name for each day
    sprintf(filename, "%02d%02d%02d.csv", now.year() - 2000, now.month(), now.day());
    Serial.println(filename);
    EEPROM[0] = now.day();
  }

  logger = SD.open(filename, FILE_WRITE);

  if (logger)
  {
    // req and write modbus
    Serial.println("File made");

    do
    { // give 2 chances to get proper reading.
      result = node.readHoldingRegisters(0x3030, 20); //read 5 registers from 0x3911 onwards, function code 03
      //    freq = node.read(0x3915); // check if reading seperately still accurate enough
      //    VLN = node.read(0x3911);
      error_counter--;
      Serial.print(error_counter);
      Serial.println(" tries left");
    } while ((!node.ku8MBSuccess) and (error_counter > 0));

    error_counter = 2; //reset error counter

    if (result == node.ku8MBSuccess)
    {


      logger.print(now.hour());
      logger.print(":");
      logger.print(now.minute());
      //      00:00
      logger.print(",");


    }

    // debugging



    data.close(); // to ensure that its saved properly
  }
  delay(10000); // a minute delay
}
