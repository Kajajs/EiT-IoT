#include "Adafruit_VL53L0X.h"
#include <Wire.h>
#include <VL53L0X.h>
#include <Udp.h>
#include <TelenorNBIoT.h>

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x16
#define LOX2_ADDRESS 0x19
#define RED 8
#define YELLOW 7
#define GREEN 6

// set the pins to shutdown
#define SHT_LOX1 7
#define SHT_LOX2 6

#include <TelenorNBIoT.h>

#ifdef SERIAL_PORT_HARDWARE_OPEN
/*
   For Arduino boards with a hardware serial port separate from USB serial.
   This is usually mapped to Serial1. Check which pins are used for Serial1 on
   the board you're using.
*/
#define ublox SERIAL_PORT_HARDWARE_OPEN
#else
/*
   For Arduino boards with only one hardware serial port (like Arduino UNO). It
   is mapped to USB, so we use SoftwareSerial on pin 10 and 11 instead.
*/
#include <SoftwareSerial.h>
#endif

// Find addresses of devices

// objects for the vl53l0x
Adafruit_VL53L0X *lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X *lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */


boolean directionArray [3] = {false, false, true};
int sendArray [4] = {0,0,0,0};

// How often we want to send a message, specified in milliseconds
// 15 minutes = 15 (min) * 60 (sec in min) * 1000 (ms in sec)
//unsigned long INTERVAL_MS = (unsigned long) 1 * 15 * 1000;

void red(){
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
}

void yellow(){
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, LOW);
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

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1->begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);
   
  //initing LOX2
  if(!lox2->begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

void read_dual_sensors() {
  
  lox1->rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2->rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  //check_direction(measure1.RangeMilliMeter, measure2.RangeMilliMeter);
  //reset_direction(measure1.RangeMilliMeter, measure2.RangeMilliMeter);
}

void find_devices(){

  VL53L0X sensor;
  VL53L0X sensor2;
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

  byte count = 0;
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      delay (1);
    }
  }
}

void send_data(){
  SoftwareSerial ublox(10, 11);

  TelenorNBIoT nbiot;

  // The remote IP address to send data packets to
  // u-blox SARA N2 does not support DNS
  IPAddress remoteIP(172, 16, 15, 14);
  int REMOTE_PORT = 1234;

  
  ublox.begin(9600);

  Serial.print("Connecting to NB-IoT module...\n");
  while (!nbiot.begin(ublox)) {
    Serial.println("Begin failed. Retrying...");
    delay(1000);
  }
  
  while (!nbiot.createSocket()) {
    Serial.print("Error creating socket. Error code: ");
    Serial.println(nbiot.errorCode(), DEC);
    delay(100);
  }
}


void setup() {
  find_devices();
  
  Serial.begin(115200);
  //while (! Serial){ delay(100);}
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  all();

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  setID();
  green();
}
/*
void reset_direction(int val_1, int val_2){
  if ((val_1 > 1000) && (val_2 > 1000)){
    delay(10);
    directionArray[0] = false;
    directionArray[1] = false;
    directionArray[2] = true;
  }
}

void set_color(){
  if (sendArray[2] < 2){
    green();
  }
  if (sendArray[2] > 1 && sendArray[2] < 4){
    yellow();
  }
  if (sendArray[2] > 3){
    red();
  }
}

void check_direction(int val_1, int val_2) {
  if ((val_1 < 1000) && (!directionArray[1])) {
    directionArray[0] = true;
    directionArray[1] = false;
    if (directionArray[2]){
      directionArray[2] = false;
      sendArray[0]++;
      if (sendArray[2] < 6){
        sendArray[2]++;
      }
      set_color();
      //Serial.println("0");
    }
  }
  
  if ((val_2 < 1000) && (!directionArray[0])) {
    directionArray[0] = false;
    directionArray[1] = true;
    if (directionArray[2]){
      directionArray[2] = false;
      sendArray[1]++;
      if (sendArray[2] > 0){
        sendArray[2]--;
      }
      set_color();
      //Serial.println("1");
    }
  }

  sendArray[3]++;
}

void transmit_data(){
  if (nbiot.isConnected()) {
    // Successfully connected to the network

    Serial.println("afsbhhasfjk:");

    String sendString = String(sendArray[0]);
  
    Serial.println(sendString);

    // Send message to remote server
    if (nbiot.sendString(remoteIP, REMOTE_PORT, "Hello, this is not an Arduino calling")) {
      Serial.println("Successfully sent data");
    } else {
      Serial.println("Failed sending data");
    }
    
    // Send test message to remote server
    if (true == nbiot.sendBytes(remoteIP, REMOTE_PORT, test_msg_ptr, leng)) {
      Serial.println("Successfully sent data");
    } else {
      Serial.println("Failed sending data");
    }

    // Wait specified time before sending again (see definition above)
    //delay(INTERVAL_MS);
  } else {
    // Not connected yet. Wait 5 seconds before retrying.
    Serial.println("Connecting...");
    delay(5000);
  }
}
*/

void loop() {
  //read_dual_sensors();
  Serial.println(sizeof(*lox1));
  delay(1000);
  //transmit_data();
}
