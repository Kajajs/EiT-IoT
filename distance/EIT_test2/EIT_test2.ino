#include <Wire.h>
#include <VL53L0X.h>
#include <Udp.h>
#include <TelenorNBIoT.h>
#include <SoftwareSerial.h>

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x16
#define LOX2_ADDRESS 0x19
#define RED 8
#define YELLOW 7
#define GREEN 6

boolean directionArray [3] = {false, false, true};
int sendArray [4] = {0,0,0,0};
int sensorRead [2] = {0,0};

VL53L0X sensor;
VL53L0X sensor2;

SoftwareSerial ublox(10, 11);

TelenorNBIoT nbiot;

IPAddress remoteIP(172, 16, 15, 14);
int REMOTE_PORT = 1234;

void redGreen(){
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, HIGH);
}

void red(){
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
}

void yellowRed(){
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
}

void yellow(){
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
}

void greenYellow(){
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, HIGH);
}

void green(){
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, HIGH);
}
void all(){
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, HIGH);
}

void read_dual_sensors() {
  sensorRead[0] = sensor.readRangeSingleMillimeters();
  sensorRead[1] = sensor2.readRangeSingleMillimeters();

  check_direction();
  reset_direction();
}

void init_devices(){
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  delay(500);
  Wire.begin();

  pinMode(4, INPUT);
  delay(150);;
  sensor.init(true);

  delay(100);
  sensor.setAddress((uint8_t)LOX1_ADDRESS);

  pinMode(5, INPUT);
  delay(150);
  sensor2.init(true);
  delay(100);
  sensor2.setAddress((uint8_t)LOX2_ADDRESS);
}

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  all();
  
  init_devices();
  
  Serial.begin(9600);

  yellowRed();

  ublox.begin(9600);
  while (!nbiot.begin(ublox)) {
    delay(1000);
  }

  greenYellow();

  while (!nbiot.createSocket()) {
    delay(100);
  }

  redGreen();

  while (!nbiot.isConnected()){
    delay(5000);
  }

  set_color();
}

void reset_direction(){
  if ((sensorRead[0] > 1000) && (sensorRead[1] > 1000)){
    delay(10);
    directionArray[0] = false;
    directionArray[1] = false;
    directionArray[2] = true;
  }
}

void set_color(){
  if (sendArray[2] < -4){
    green();
  }
  if (sendArray[2] > -5 && sendArray[2] < 5){
    yellow();
  }
  if (sendArray[2] > 4){
    red();
  }
}

void check_direction() {
  if ((sensorRead[0] < 1000) && (!directionArray[1])) {
    directionArray[0] = true;
    directionArray[1] = false;
    if (directionArray[2]){
      directionArray[2] = false;
      if (sendArray[2] < 40){
        sendArray[0]++;
        sendArray[2]++;
      }
      set_color();
      //Serial.println("0");
    }
  }
  
  if ((sensorRead[1] < 1000) && (!directionArray[0])) {
    directionArray[0] = false;
    directionArray[1] = true;
    if (directionArray[2]){
      directionArray[2] = false;
      if (sendArray[2] > -40){
        sendArray[1]++;
        sendArray[2]--;
      }
      set_color();
      //Serial.println("1");
    }
  }

  sendArray[3]++;
}

boolean shouldTransmitData(){
  return sendArray[3] > 3000;
}


void transmit_data(){
  if (nbiot.isConnected()) {
    String sendString = String(sendArray[0]) + String("-") + String(sendArray[1]) + String("-") + String(sendArray[2]);

    if (nbiot.sendString(remoteIP, REMOTE_PORT, sendString)) {
      all();
      sendArray[0] = 0;
      sendArray[1] = 0;
      sendArray[3] = 0;
      delay(100);
      set_color();
    }
  }
}


void loop() {
  read_dual_sensors();
  if (shouldTransmitData()){
    transmit_data();
  }
}
