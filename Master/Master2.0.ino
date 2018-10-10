/*
  Simple example for receiving

  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>
#include <stdlib.h>

RCSwitch mySwitch = RCSwitch();
boolean ended = false;
String concatenated;
String id = "00000001";
String idDestino = "10100000";
String turnOn = "00001111";
String on = "11111111";
String turnOff = "00001100";
String off = "00000000";
unsigned long timeToRun;
char string[17];
String onString;
String offString;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableTransmit(10);
  mySwitch.setRepeatTransmit(15);
  Serial.println("Test");
  while(Serial.available() <= 0);
    String incomingByte = Serial.readString();
    onString = incomingByte;
  while(Serial.available() <= 0);
    incomingByte = Serial.readString();
    offString = incomingByte;
    timeToRun = millis();
}

void loop() {
   if(mySwitch.available()) {
    if(mySwitch.getReceivedValue() == stringToInt(id + turnOn) && (millis() - timeToRun) > 500) {
      timeToRun = millis();
      delay(1000);
      Serial.println("Send confirmation");
      delay(300);
    }
    mySwitch.resetAvailable();
  }
//  if (Serial.available() > 0) {
//    String incomingByte = Serial.readString();
//    setState(incomingByte);
//  }
}

void setState(String state) {
//  Serial.println(state);
//  if (state.equals(onString)) {
//    Serial.println("LED on");
//    concatenated = (idDestino + turnOn);
//    concatenated.toCharArray(string, 17);
//    mySwitch.send(string);
//    delay(300);
//    while (!ended) {
//      mySwitch.send(string);
//      delay(300);
//      if (mySwitch.available()) {
//        if (mySwitch.getReceivedValue() == stringToInt(id + on)) {
//          Serial.println("Confirmed");
//          ended = true;
//        }
//        mySwitchRx.resetAvailable();
//      }
//    }
//    ended = false;
//  } else if (state.equals(offString)) {
//    Serial.println("LED off");
//    concatenated = (idDestino + turnOff);
//    concatenated.toCharArray(string, 17);
//    mySwitch.send(string);
//    delay(300);
//    while (!ended) {
//      mySwitch.send(string);
//      delay(300);
//      if (mySwitch.available()) {
//        if (mySwitch.getReceivedValue() == stringToInt(id + off)) {
//          Serial.println("Confirmed");
//          ended = true;
//        }
//        mySwitchRx.resetAvailable();
//      }
//    }
//    ended = false;
//  }
}

long int stringToInt(String string) {
  char s[17] = {0};
  stringToCharArray(string, s);
  unsigned long value = 0;
  for (int i = 0; i < strlen(s); i++) // for every character in the string  strlen(s) returns the length of a char array
  {
    value *= 2; // double the result so far
    if (s[i] == '1') value++;  //add 1 if needed
  }
  return value;
}

void stringToCharArray(String string, char * buff) {
  string.toCharArray(buff, 17);
}

void stringToSendMessage(String string, char * buff, int position) {
  char internalBuff[8];
  string.toCharArray(internalBuff, 8);
  for(int i = position;i < position + 8; i++) {
    buff[i] = internalBuff[i-position];
  }
}

void intToSendMessage(unsigned long longue, char * buff, int start) {
  for (int i=start + 7; i>=start; i--)
    buff[i] = bitRead(longue,i%8);
}

void intToReceiveMessage(unsigned long longue, char * buff) {
  for (int i=23; i>=0; i--) {
    buff[i] = bitRead(longue,i);
  }
}

String charArrayToString(char * buff, int init, int finish) {
  String returnString = "";
  for(int i = init; i<finish; i++) {
    returnString += buff[i];
  }
  return returnString;
}
