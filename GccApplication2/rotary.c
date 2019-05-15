#include "rotary.h"

static uint8_t rotarystatus=0;

void RotaryInit(void)
{
	ROTDDR &= ~((1<<ROTPA)|(1<<ROTPB)|(1<<ROTPBUTTON)); //set pins as input
	ROTPORT |= (1<<ROTPA)|(1<<ROTPB)|(1<<ROTPBUTTON); 	//enable interrnal pullups;
}
void RotaryCheckStatus(void)
{
	//reading rotary and button
	if(ROTA & (!ROTB)) //rotation is left
	{
		loop_until_bit_is_set(ROTPIN, ROTPA);
		if (ROTB)
			rotarystatus = 1;
	}
	else if(ROTB & (!ROTA)) //rotation is right
	{
		loop_until_bit_is_set(ROTPIN, ROTPB);
		if (ROTA)
			rotarystatus = 2;
	}
	else if (ROTA & ROTB)
	{
		loop_until_bit_is_set(ROTPIN, ROTPA);
		if (ROTB){
			rotarystatus = 1; //rotation is left
		}
		else{ 
			rotarystatus = 2; //rotation is right
		}
	}
	if (ROTCLICK) //Button is pushed
	{
		rotarystatus=3;
	}
}
uint8_t RotaryGetStatus(void)
{
	return rotarystatus;
}
void RotaryResetStatus(void)
{
	rotarystatus=0;
}