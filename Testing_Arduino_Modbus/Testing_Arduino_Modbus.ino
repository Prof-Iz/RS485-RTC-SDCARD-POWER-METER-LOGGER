#include <ModbusRtu.h>

Modbus bus;

uint16_t modbus_array[] = {0,0,0};
 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bus = Modbus(1,1,4);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  String Response = Serial.readStringUntil("\n");
  Serial.print("Response is");
  Serial.println(Response);

}
