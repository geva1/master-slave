/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>
#include <stdlib.h>

RCSwitch mySwitch = RCSwitch();
boolean led = false;
unsigned long timeToRun;
String masterId = "";
String id = "";
String randId = "";
String checkState = "00000000";
String setId = "11111100";
String iWantId = "11001100";
String confirmId = "00110011";
String turnOn = "00001111";
String on = "10101010";
String turnOff = "11110000";
String off = "01010101";
String confirm = "11111111";
char string[17];

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableTransmit(10);
  mySwitch.setRepeatTransmit(15);

  unsigned long randomNumber = random(1, 255);
  char buff[25];
  boolean ended = false;
  char response[25];
  intToSendMessage(0, buff, 16);
  intToSendMessage(randomNumber, buff, 8);
  intToSendMessage(0, buff, 0);
  randId = charArrayToString(buff, 8, 16);
  mySwitch.send(buff);
  delay(300);
  while (!ended) {
    mySwitch.send(buff);
    delay(300);
    if (mySwitch.available()) {
      intToReceiveMessage(mySwitch.getReceivedValue(), response);
      if (charArrayToString(response, 16, 24) == randId && 
          charArrayToString(response, 0, 8) == setId) {
        masterId = charArrayToString(response, 8, 16);
        ended = true;
      }
      mySwitch.resetAvailable();
    }
  }
  ended = false;
  stringToSendMessage(masterId, buff, 16);
  stringToSendMessage(randId, buff, 8);
  stringToSendMessage(iWantId, buff, 0);
  mySwitch.send(buff);
  delay(300);
  while (!ended) {
    mySwitch.send(buff);
    delay(300);
    if (mySwitch.available()) {
      intToReceiveMessage(mySwitch.getReceivedValue(), response);
      if (charArrayToString(response, 16, 24) == randId && 
          charArrayToString(response, 0, 8) == confirmId) {
        id = charArrayToString(response, 8, 16);
        ended = true;
      }
      mySwitch.resetAvailable();
    }
  }
  stringToSendMessage(masterId, buff, 16);
  stringToSendMessage(id, buff, 8);
  stringToSendMessage(confirm, buff, 0);
  mySwitch.send(buff);
  delay(300);

  Serial.println("Test");
  timeToRun = millis();
}

void loop() {
//  if (mySwitch.available()) {
//      //Serial.println( mySwitchRx.getReceivedValue() );
//      Serial.println( stringToInt(id + turnOn) );
//      if(mySwitch.getReceivedValue() == stringToInt(id + turnOn) && (millis() - timeToRun) > 500) {
//        Serial.println("LED on");
//        timeToRun = millis();
//        digitalWrite(LED_BUILTIN, 1);
//        delay(1000);
//        //(idDestino + on).toCharArray(string, 17);
//        //mySwitch.send(string);
//        Serial.println("Send confirmation");
//        delay(300);
//      } else if(mySwitch.getReceivedValue() == stringToInt(id + turnOff) && (millis() - timeToRun) > 500) {
//        Serial.println("LED off");
//        timeToRun = millis();
//        digitalWrite(LED_BUILTIN, 0);
//        delay(1000);
//        //(idDestino + off).toCharArray(string, 17);
//        //mySwitch.send(string);
//        Serial.println("Send confirmation");
//        delay(300);
//      }
//    mySwitch.resetAvailable();
//  }
}

long int stringToInt(String string) {
  char s[17] = {0};
  stringToCharArray(string, s);
  unsigned int value = 0;
  for (int i=0; i< strlen(s); i++)  // for every character in the string  strlen(s) returns the length of a char array
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
