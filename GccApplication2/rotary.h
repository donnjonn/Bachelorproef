#define ROTARY_H
#include <avr/io.h>
//define port where encoder is connected
#define ROTPORT PORTD
#define ROTDDR DDRD
#define ROTPIN PIND
//define rotary encoder pins
#define ROTPA PD3
#define ROTPB PD4
#define ROTPBUTTON PD5
//define macros to check status
#define ROTA !((1<<ROTPA)&ROTPIN)
#define ROTB !((1<<ROTPB)&ROTPIN)
#define ROTCLICK !((1<<ROTPBUTTON)&ROTPIN)

void RotaryInit(void);
void RotaryCheckStatus(void);
uint8_t RotaryGetStatus(void);
void RotaryResetStatus(void);