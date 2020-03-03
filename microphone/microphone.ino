#define LITTLE 0
#define MUCH 1

long val = 0;
int readval;

void setup(){ 
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
}

void loop(){
  
  for (int i = 0; i < 5000; i++){
    readval = (int) analogRead(A0);
    if (readval > val){
      val = readval;
    }
    //val = val + (long) analogRead(A0);
    //delay(10);
  }
  Serial.println(val);
  val = 0;
  
}
