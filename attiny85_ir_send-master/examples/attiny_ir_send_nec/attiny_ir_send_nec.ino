//Anor Batista Esteves Neto 
//  January 20015 https://github.com/anorneto/attiny85_ir_send
//  Attiny send IR NEC CODE with button press

#include <attiny85_ir_send.h>

// defining pins
#define buttonPin  4    //button connected to attiny physical pin 3 and ground 
#define ledPin     0      // (optional)attiny physical pin 5 , normal led for visual feedback of when we are sending an ir command. 
                          //Use at least a 470R resistor in series with it

//change 00000 to a NEC code!
#define YOUR_NEC_CODE 00000


IRsend irsend; // IR led on physical pin 6 (Digital 1) , the IR output will be always on attiny physical pin 6

// Variables that will change:
int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly overflow an int
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin,HIGH); //activating internal pull-up on button pin
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
}

void loop() {
  check_button_state();
 }


void check_button_state( void ){
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);
    
    // check to see if you just pressed the button 
    // (i.e. the input went from LOW to HIGH),  and you've waited 
    // long enough since the last press to ignore any noise:  

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    } 

    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
      // if the button state has changed:
      if (reading != buttonState)  
          buttonState = reading;
      if (buttonState == LOW) 
          send_ir(); // if button goes low lets send an IR code!
    }
    // save the reading.  Next time through the loop,
    // it'll be the lastButtonState:
    lastButtonState = reading;
}

void send_ir(){
    irsend.sendNEC(YOUR_NEC_CODE, 32); // sending the nec code
    digitalWrite(ledPin,HIGH); // blinks an led just for  a visual feedback
    delay(200);
    digitalWrite(ledPin,LOW);
}
