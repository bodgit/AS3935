#include <Wire.h>
#include <AS3935.h>

AS3935 as3935(0x03);

volatile unsigned char isr = 0;

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("AS3935");
  
  attachInterrupt(1, interrupt, RISING);
  
  as3935.reset();
  as3935.calibrate(6);
}

void loop()
{
  uint8_t interrupt, distance;
  
  if (isr) {
    isr = 0;
    
    delay(2);
    
    interrupt = as3935.interrupt();
    
    switch (interrupt) {
    case AS3935_INTERRUPT_NOISE:
      Serial.println("noise");
      break;
    case AS3935_INTERRUPT_DISTURBER:
      Serial.println("disturber");
      break;
    case AS3935_INTERRUPT_LIGHTNING:
      Serial.println("lightning");
        
      distance = as3935.distance();
        
      switch (distance) {
      case AS3935_LIGHTNING_OVERHEAD:
        Serial.println("overhead");
        break;
      case AS3935_LIGHTNING_OUT_OF_RANGE:
        Serial.println("out of range");
        break;
      default:
        Serial.print(distance);
        Serial.println(" km");
        break;
      }
        
      break;
    default:
      Serial.print("unknown (");
      Serial.print(interrupt, HEX);
      Serial.println(")");
      break;
    }
  }
}

void interrupt(void)
{
  isr = 1;
}
