/*even parity, little endian float
 * stopbit 1 
 */

#define MAX485_DE 3
#define MAX485_RE_NEG 2



#include <ModbusMaster.h>


ModbusMaster node;

const int ID = 1;

uint8_t result;

uint16_t v1 = 3927;

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


void setup() {
  // put your setup code here, to run once:

  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
  
  Serial.begin(9600,SERIAL_8E1);
  
  while (!Serial) {}


  node.begin(ID, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);


}

void loop() {
  // put your main code here, to run repeatedly:

  result = node.readHoldingRegisters(3030,20); // jwhether read or not
  
  if (result == node.ku8MBSuccess) {
    Serial.print("V1 is: ");
    for (int i=0; i<20;i+=2){
      Serial.print("Index ");
      Serial.print(i);
      Serial.print("---");
      Serial.println(node.getResponseBuffer(i));
      
      
      }

  } else {
    Serial.println("The ku8MBSuccess Flag is False");

  }

  delay(3000);

}
