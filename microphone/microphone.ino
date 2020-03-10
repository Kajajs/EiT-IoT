#include <Udp.h>
#include <TelenorNBIoT.h>
#include <SoftwareSerial.h>

#define LIMITVALUE 320
#define LOX1_ADDRESS 0x16
#define LOX2_ADDRESS 0x19

SoftwareSerial ublox(10, 11);

TelenorNBIoT nbiot;

IPAddress remoteIP(172, 16, 15, 14);
int REMOTE_PORT = 1234;

int val = 0;
int readval;
int lm;

int measure = 0;
String stringSend = "";

void setup(){ 
  Serial.begin(9600);
  /*
  ublox.begin(9600);
  while (!nbiot.begin(ublox)) {
    delay(1000);
  }

  while (!nbiot.createSocket()) {
    delay(100);
  }

  while (!nbiot.isConnected()){
    delay(3000);
  }
  */
}

void read_data(){
  lm = 0;
  for (int c = 0; c < 50; c++){
    val = 0;
    for (int i = 0; i < 1000; i++){
      readval = (int) analogRead(A0);
      if (readval > val){
        val = readval;
      }
    }
    if (val > LIMITVALUE){
      lm += val - LIMITVALUE;
    }
  }
  //Serial.println(lm);
  stringSend += String(lm) + String(":");
  measure++;
  Serial.println(lm);
}

boolean should_transmit_data(){
  return measure > 24;
}
/*
void transmit_data(){
  if (nbiot.isConnected()) {
    if (nbiot.sendString(remoteIP, REMOTE_PORT, stringSend)) {
      measure = 0;
      stringSend = "";
    }
  }
}
*/
void loop(){
  read_data();
  //if (should_transmit_data()){
    //transmit_data();
  //}
  
}
