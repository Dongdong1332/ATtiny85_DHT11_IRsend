#include <attiny85_ir_send.h>
IRsend irsend;

unsigned int rawDataCLOSE[115] = {3230,1620, 430,370, 430,370, 430,1220, 380,420, 380,1220, 430,370, 430,420, 380,420, 380,1220, 430,1170, 430,420, 380,420, 380,420, 380,1220, 430,1220, 380,420, 380,420, 380,420, 430,370, 430,370, 430,420, 380,420, 380,420, 430,370, 430,370, 430,420, 380,420, 380,420, 380,1220, 430,370, 430,420, 380,420, 380,420, 380,420, 430,370, 430,370, 430,1220, 380,420, 380,420, 430,370, 430,370, 430,1220, 380,420, 430,370, 430,370, 430,420, 380,420, 380,420, 380,1220, 430,370, 430,1220, 380,1220, 430,1170, 430,1220, 380,1220, 430,1170, 430};  // Protocol=PULSE_DISTANCE Address=0x0 Command=0x0 Raw-Data=0xFD0210 56 bits LSB first
const int thirtyMinutes = 2000; //半小時(多一點)
const int ONE_HOUR = 3600; //一小時

const int MAX = 2001; //頂值
int counts = MAX;
int clock_count = 0; //分鐘計時器

const int IRPin = 1;                    //IR LED Pin
const int showLEDPin = 2;               //show Mode LED Pin
#define DHT11_PIN 4
int inTimePin=3;
int val=0;
int NowHour = 20;       //讀取到目前的時間(24小時制)
bool needWork = true;  //是否開啟自動休眠
bool powerOFF = false; //不再動作直到重新開機
#include <dht.h>
dht DHT;
void Blink(int);

double temp = 0;
double humi = 0;

void setup(void)
{
  enableIROut(38);
  pinMode(IRPin,OUTPUT);
  pinMode(showLEDPin,OUTPUT);
  pinMode(inTimePin,INPUT);
  GET_NOWHOUR_BY_SW();
}
void Blink(int speedTime){
  for(int i=1;i<=4;i++){
    digitalWrite(showLEDPin, i%2);
    delay(speedTime);
  }
}
void state(int n){ //10進制四位輸出 (0會間隔500ms)
//  for(int i=1;i<=n/1000;i++){
//    digitalWrite(showLEDPin, HIGH);
//    delay(200);
//    digitalWrite(showLEDPin, LOW);
//    delay(200);
//  }
//  delay(500);
//  for(int i=1;i<=(n%1000)/100;i++){
//    digitalWrite(showLEDPin, HIGH);
//    delay(200);
//    digitalWrite(showLEDPin, LOW);
//    delay(200);
//  }
  delay(500);
  for(int i=1;i<=(n%100)/10;i++){
    digitalWrite(showLEDPin, HIGH);
    delay(200);
    digitalWrite(showLEDPin, LOW);
    delay(200);
  }
  delay(500);
  for(int i=1;i<=n%10;i++){
    digitalWrite(showLEDPin, HIGH);
    delay(200);
    digitalWrite(showLEDPin, LOW);
    delay(200);
  }
  delay(1500);
}

//  humi = (DHT.humidity);

void loop(void){
  if(needWork){ //有在工作
    int chk = DHT.read11(DHT11_PIN);
    temp = (DHT.temperature)-2;
    if(temp >= 28 && counts >= thirtyMinutes) {
      counts = 0;
      AC_OPEN_hardCode();
      delay(3000);
      clock_count+=3;
    }else{
      if(counts >= MAX) {
        counts = MAX;
      }
      else {
        counts++;
      }
    }
  }

  /*小時計時器*/
  if(clock_count >= ONE_HOUR){
    clock_count = 0;
    NowHour++;
    state(NowHour); //1~24
    if(NowHour > 23) NowHour = 0;
    if(NowHour >= 9 && NowHour < 23 || NowHour == 4 || NowHour == 5) needWork=false;
    else {
      needWork=true;
      counts = MAX; //結束直接判斷是否溫度過高
    }
  }
  delay(1000);
  clock_count++;
}

void GET_NOWHOUR_BY_SW(){
  int index = 0, x=0;
  delay(500);
  while(!digitalRead(inTimePin) && NowHour!=1){
    if(x++ > 1500){ x=0; index++;
      for(int i=0;i<index;i++){
        digitalWrite(showLEDPin, HIGH);
        delay(200);
        digitalWrite(showLEDPin, LOW);
        delay(200);
      }
      NowHour++;
      if(NowHour > 23) NowHour = 0;
    }
    delay(1);
  }
  digitalWrite(showLEDPin, LOW);
  delay(500);
  digitalWrite(showLEDPin, HIGH);
  delay(2000);
  digitalWrite(showLEDPin, LOW);
  state(NowHour);
}

//
//void AC_CLOSE(){
//  irsend.sendRaw(rawDataCLOSE,115,38); //AC_CLOSE
//  delay(3000);
//}

void mark(int16_t time) {
    // Sends an IR mark for the specified number of microseconds.
    // The mark output is modulated at the PWM frequency.
    
    // Clear OC0A/OC0B on Compare Match when up-counting.
    // Set OC0A/OC0B on Compare Match when down-counting.    
    TCCR0A |= _BV(COM0B1); // Enable pin 6 (PB1) PWM output        
    delayMicroseconds(time);    
}

/* Leave pin off for time (given in microseconds) */
void space(int16_t time) {
    // Sends an IR space for the specified number of microseconds.
    // A space is no output, so the PWM output is disabled.
    
    // Normal port operation, OC0A/OC0B disconnected.
    TCCR0A &= ~(_BV(COM0B1)); // Disable pin 6 (PB1) PWM output
    delayMicroseconds(time);    
}

void enableIROut(uint8_t khz) {
  // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
  // The IR output will be on pin 6 (OC0B).
  // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
  // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
  // TIMER0 is used in phase-correct PWM mode, with OCR0A controlling the frequency and OCR0B
  // controlling the duty cycle.
  // There is no prescaling, so the output frequency is F_CPU/ (2 * OCR0A)
  
  DDRB |= _BV(IRLED); // Set as output

  PORTB &= ~(_BV(IRLED)); // When not sending PWM, we want it low

  // Normal port operation, OC0A/OC0B disconnected  
  // COM0A = 00: disconnect OC0A
  // COM0B = 00: disconnect OC0B; to send signal set to 10: OC0B non-inverted    
  // WGM0 = 101: phase-correct PWM with OCR0A as top
  // CS0 = 000: no prescaling
  TCCR0A = _BV(WGM00);
  TCCR0B = _BV(WGM02) | _BV(CS00);

  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR0A.
  OCR0A = F_CPU / 2 / khz / 1000;
  OCR0B = OCR0A / 3; // 33% duty cycle
}
void AC_OPEN_hardCode(){
  mark( 3280 );
space( 1570 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1170 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 1170 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1170 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 1170 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 1170 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 1170 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 420 );
mark( 430 );
space( 370 );
mark( 430 );
space( 370 );
mark( 430 );
space( 420 );
mark( 380 );
space( 420 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 1220 );
mark( 380 );
space( 420 );
mark( 380 );
space( 1220 );
mark( 430 );
space( 1170 );
mark( 430 );

space(0);
}
