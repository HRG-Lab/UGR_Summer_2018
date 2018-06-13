#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial GPRS(7,8);

void setup() {
  GPRS.begin(19200); // Arduino communicates with SIM900 GSM shield at baud rate of 19200
  delay(500);
  Serial.begin(19200);
}

void loop() {
  // put your main code here, to run repeatedly:
  SendTextMessage();
}
void SendTextMessage()
{
  GPRS.print("AT+CMGF=1\r");    //Sending the SMS in text mode, AT command to set SIM900 to SMS mode
  delay(100);
  GPRS.println("AT + CMGS = \"2818862113\"");//Send SMS command - Parameters: <target phone number>, <message> <ctrl+z>
  delay(100);
  GPRS.println("hello0000");//the content of the message
  delay(100);
  GPRS.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);
  GPRS.println();
}
