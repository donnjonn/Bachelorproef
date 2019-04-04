#define ROTARY_H
#include <avr/io.h>
//define port where encoder is connected
#define ROTPORT PORTC
#define ROTDDR DDRC
#define ROTPIN PINC
//define rotary encoder pins
#define ROTPA PC0
#define ROTPB PC1
#define ROTPBUTTON	PC2
//define macros to check status
#define ROTA !((1<<ROTPA)&ROTPIN)
#define ROTB !((1<<ROTPB)&ROTPIN)
#define ROTCLICK !((1<<ROTPBUTTON)&ROTPIN)

void RotaryInit(void);
void RotaryCheckStatus(void);
uint8_t RotaryGetStatus(void);
void RotaryResetStatus(void);