
void setup(){ 
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  int sensor_pin = 0;
}

void loop(){
  int val;
  val = analogRead(A0);   //connect mic sensor to Analog 0
  if (val > 550) {
    Serial.print("Sound=");
    Serial.println(val,DEC);//print the sound value to serial  
    }
}
