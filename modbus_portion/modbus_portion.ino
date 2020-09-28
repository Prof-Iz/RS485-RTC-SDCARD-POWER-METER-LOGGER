//#include <SoftwareSerial.h>

/*even parity, little endian float
   stopbit 1
*/

#define MAX485_DE 3
#define MAX485_RE_NEG 2





#include <ModbusMaster.h>

//SoftwareSerial mySerial(4,5);
ModbusMaster node;

const int ID = 1;

uint8_t result;

//uint16_t data[2];
//
//uint32_t value = 0;
//
//float reading = 0;

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

float HexToFloat(uint32_t x) {

  return (*(float*)&x);
}


void setup() {
  // put your setup code here, to run once:

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);

  Serial.begin(9600, SERIAL_8E1);
  //  mySe/rial.begin(9600);

  while (!Serial) {}


  node.begin(ID, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);


}

void loop() {
  // put your main code here, to run repeatedly:

  result = node.readHoldingRegisters(3914, 2); // jwhether read or not
  if (result == node.ku8MBSuccess) {

    uint16_t data[2];

    uint32_t value = 0;

    double reading = 0;


    for (int i = 0; i < 2; i += 1) {
      data[i] = node.getResponseBuffer(i);

    }
    Serial.print("data 1 is: ");
    Serial.println(data[0]);
    Serial.print("data 2 is: ");
    Serial.println(data[1]);
    value = data[1];
    value = value << 16;
    value = value + data[0];
    Serial.print("value is: ");
    Serial.println(value);

    reading = HexToFloat(value);
    Serial.print("reading is: ");
    Serial.println(reading);

  } else {
    Serial.println("The ku8MBSuccess Flag is False");

  }

  delay(3000);

}
