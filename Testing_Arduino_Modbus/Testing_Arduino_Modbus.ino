#include <SensorModbusMaster.h>
modbusMaster modbus;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600,SERIAL_8E1);
  pinMode(2,OUTPUT);
  
  modbus.begin(1,Serial,2);
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(modbus.float32FromRegister(0x03,3927,littleEndian));
  delay(1000);
}
