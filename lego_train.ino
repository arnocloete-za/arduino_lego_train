#include <legopowerfunctions.h>


const byte interruptPin = 2;
//volatile byte state = HIGH;
bool pedestrain_state = false;
bool start = true;
int red_vehicles = 4;
int yellow_vehicles = 5;
int green_vehicles = 6;
int red_train = 7;
int green_train = 8;

//int train_one = 10;
LEGOPowerFunctions lego(9);

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor 
int red_vehicles_state = LOW;
int yellow_vehicles_state = LOW;
int green_vehicles_state = LOW;
int red_train_state = LOW;
int green_train_state = LOW;

unsigned long previousMillis = 0;        // will store last time LED was updated
int remember_next_wait = 300;
// constants won't change:
const long interval = 100;           // interval at which to blink (milliseconds)
int traffic_light_state = 1;


// the setup function runs once when you press reset or power the board
void setup() {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(red_vehicles, OUTPUT);
  pinMode(yellow_vehicles, OUTPUT);
  pinMode(green_vehicles, OUTPUT);
  pinMode(red_train, OUTPUT);
  pinMode(green_train, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pedestrian, CHANGE);
  
}

void pedestrian() {
  pedestrain_state = true;
}

// the loop function runs over and over again forever
void loop() {
  photocellReading = analogRead(photocellPin);  
  //Serial.print("Analog reading = ");
  //Serial.println(photocellReading);     // the raw analog reading
  
  if (start == true) {
    Serial.println("START");
    //digitalWrite(train_one, LOW);
    for (int i=0; i<5; i++) 
    {
      digitalWrite(red_vehicles, HIGH);
      digitalWrite(yellow_vehicles, LOW);
      digitalWrite(green_vehicles, LOW);   
      digitalWrite(red_train, HIGH);
      //digitalWrite(yellow_two, LOW);
      digitalWrite(green_train, LOW);   
      delay(100); 
      digitalWrite(red_vehicles, LOW);
      digitalWrite(yellow_vehicles, LOW);
      digitalWrite(green_vehicles, LOW);   
      digitalWrite(red_train, LOW);
      //digitalWrite(yellow_two, LOW);
      digitalWrite(green_train, LOW);     
      delay(1000); 
    }
    digitalWrite(red_vehicles, HIGH);
    digitalWrite(red_train, HIGH);
    start = false;
  }
  if (pedestrain_state == true)
  {
    Serial.println("PEDESTRIAN");
    digitalWrite(red_vehicles, LOW);
    digitalWrite(yellow_vehicles, HIGH);
    digitalWrite(green_vehicles, LOW);   
    delay(200); 
    digitalWrite(red_vehicles, HIGH);
    digitalWrite(yellow_vehicles, LOW);
    digitalWrite(green_vehicles, LOW);   
    delay(2000); 
    pedestrain_state = false;
     
  }
  else
  {
    // STOP THE TRAIN
    if (photocellReading < 500 and ((traffic_light_state == 1) or (traffic_light_state == 3))) {
      Serial.print("DARK HERE and traffic light 1 red: Stop the train");
      //digitalWrite(train_one, HIGH);
      //lego.SingleOutput(0, PWM_FLT, BLUE, CH4);
      
      lego.SingleOutput(0, PWM_FLT, RED, CH4);
      delay(100);
    }
    else if (traffic_light_state == 2) {
      Serial.print("Light and state 1 go.");
      lego.SingleOutput(0, PWM_FWD4, RED, CH4);
      delay(100);
    }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= remember_next_wait) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // TRAIN GREEN, VECHICLES RED, while waitinng in status 2 this is executed
      if (traffic_light_state == 1){
        //Serial.print("traffic_light_state 1");
        traffic_light_state++;
        remember_next_wait = 20000;
        digitalWrite(red_vehicles, HIGH);
        digitalWrite(yellow_vehicles, LOW);
        digitalWrite(green_vehicles, LOW);   
        digitalWrite(red_train, LOW);
        digitalWrite(green_train, HIGH); 
      // TRAIN RED, VECHICLES GREEN, , while waitinng in status 3 this is executed
      } else if (traffic_light_state == 2) {
        //Serial.print("traffic_light_state 2");
        traffic_light_state++;
        remember_next_wait = 20000;
        digitalWrite(red_vehicles, LOW);
        digitalWrite(yellow_vehicles, LOW);
        digitalWrite(green_vehicles, HIGH);   
        digitalWrite(red_train, HIGH);        
        digitalWrite(green_train, LOW);  
      // YELLOW VECHILES YELLOW, TRAIN RED, , while waitinng in status 1 this is executed
      } else if (traffic_light_state == 3) {
        //Serial.print("traffic_light_state 3");
        traffic_light_state = 1;
        remember_next_wait = 2000;        
        digitalWrite(red_vehicles, LOW);
        digitalWrite(yellow_vehicles, HIGH);
        digitalWrite(green_vehicles, LOW);   
        digitalWrite(red_train, HIGH);        
        digitalWrite(green_train, LOW);
      }
    }
  }  
}
