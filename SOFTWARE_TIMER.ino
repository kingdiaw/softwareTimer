#include "ESP32TimerInterrupt.h"


// SOFTWARE TIMER
#define TIMER_MAX  10
#define TIMER_MST 10  // ms/tick
#define TIMER0_INTERVAL_MS        10
unsigned char TimerFlag = 0;
unsigned int TimerArray[TIMER_MAX];

void timerClr(uint8_t index)
{
  if(index>=TIMER_MAX) return;
  TimerArray[index]=0;
} 

void timerSet(uint8_t index, unsigned long value)
{
  if(index>=TIMER_MAX) return;
  value /= TIMER_MST;
  TimerArray[index]=(unsigned int)value;
  TimerFlag=1;
} 

uint8_t timerUp(uint8_t index)
{
  if(index>=TIMER_MAX) return 0;
  if(TimerArray[index]) return 0;
  return 1;
}

uint8_t timerBusy(uint8_t index)
{
  if(index>=TIMER_MAX) return 0;
  if(TimerArray[index]) return 1;
  return 0;
}


void IRAM_ATTR TimerHandler0(void){
uint8_t i;
uint16_t dd;
  //TIMER0 10ms Interrupt
    if(TimerFlag) // to save processing time
    {
      TimerFlag = 0;
      for(i=0;i<TIMER_MAX;i++)
      {
        dd=TimerArray[i];
        if(dd) { dd--; TimerFlag=1; }
        TimerArray[i]=dd;
      }
    }  
}

ESP32Timer ITimer0(0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);
  delay(100);

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println(F("Starting  ITimer0 OK"));

  //Assign Initial Value
  timerClr(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(timerUp(0)){
    timerSet(0,1000);
    Serial.print("Time0 Up");Serial.print("\t");Serial.print("millis()=");Serial.println(millis());
  }
}
