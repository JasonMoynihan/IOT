#define BUTTON 4
#define LED 5
bool buttonstate;
bool clickedflag = false;
bool ledon = false;

void setup() {
  Serial.begin(9600);
   
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);   
}

// the loop function runs over and over again forever
void loop() {
  buttonstate = digitalRead(BUTTON);

  if(buttonstate and not clickedflag){
    clickedflag = true;
  }

  if(not buttonstate and clickedflag){

    clickedflag = false;
    //Serial.println("clicked");
    if(not ledon)
    {
      digitalWrite(LED, HIGH);
      ledon = true;
    }
    else
    {
      digitalWrite(LED, LOW);
      ledon = false;
    }
  }

}
