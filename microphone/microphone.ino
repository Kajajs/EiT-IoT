#define LITTLE 0
#define MUCH 1

int counter = -1;
int measurements [100];
int val;

void setup(){ 
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}

int average(){
  int total = 0;
  for (int i = 0; i < 100; i++){
    total = total + measurements[i];
  }
  return (int) total / 100;
}

void loop(){
  
  counter++;
  if (counter > 99){
    counter = 0;
    Serial.println(average());
  }
  measurements[counter] = analogRead(A0);   //connect mic sensor to Analog 0
  delay(10);
  /*
  val = analogRead(A0);
  if (val > 545) {
    Serial.print("Sound=");
    Serial.println(val,DEC);//print the sound value to serial  
    }
  */
}
