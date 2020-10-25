
/*even parity, little endian word order Big Endian Byte order // float
   stopbit 1
*/

#define MAX485_DE 3
#define MAX485_RE_NEG 2
// can be same pin

#include <ModbusMaster.h>

ModbusMaster node;

const int ID = 1;

uint8_t result;

uint16_t data[2];

uint32_t value = 0;

double reading = 0; //test with float, if no ovf then switch for memory

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
  result = node.readHoldingRegisters(3914, 2);
  if (result == node.ku8MBSuccess) {

    for (int j = 0; j < 20; j += 2) { // 0, 2, 4 6 8 10 12 14 16 18 
      int p =0;
      for (int i = j; i <(j+2); i += 1) {
        data[p] = node.getResponseBuffer(i);
        p++;

      }
      
      value = data[1];
      value = value << 16;
      value = value + data[0]; // byte shifting since Arduino processor byte order Little Endian, but PM1200 Big Endian

      reading = HexToFloat(value);
      Serial.print("reading at buffer ");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(reading);

    }

  } else {
    Serial.println("The ku8MBSuccess Flag is False");

  }

  delay(3000);

}
