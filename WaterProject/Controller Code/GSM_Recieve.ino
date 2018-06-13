#include <SoftwareSerial.h>

//Configure software serial port
SoftwareSerial GPRS(7,8);

char incoming_char=0;

void setup() {
  // put your setup code here, to run once:
  GPRS.begin(19200);
  delay(500);
  Serial.begin(19200);

  GPRS.print("AT+CMGF=1\r");  // Set message format - 1 for SMS
  delay(100);
  GPRS.print("AT+CNMI=2,2,0,0,0\r");  // New Message Indication
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(GPRS.available()>0){
    incoming_char=GPRS.read();
    Serial.print(incoming_char);
  }
}
