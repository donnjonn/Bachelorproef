//*****************************************************************************
//
// File Name	: 'rotary.c'
// Title		: Reading rotarry encoder functions
// Author		: Scienceprog.com - Copyright (C) 2011
// Created		: 2011-08-01
// Revised		: 2011-10-13
// Version		: 1.1
// Target MCU	: Atmel AVR series (Atmega32)
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//2011.10.13 - rewrite of RotaryCheckStatus function
//*****************************************************************************
#include "rotary.h"


static uint8_t rotarystatus=0;
static uint8_t wait=0;

void RotaryInit(void)
{
	
//set pins as input

ROTDDR &= ~((1<<ROTPA)|(1<<ROTPB)|(1<<ROTPBUTTON));

//DDRC = 0x00;
//enable interrnal pullups;
ROTPORT |= (1<<ROTPA)|(1<<ROTPB)|(1<<ROTPBUTTON);

}
void RotaryCheckStatus(void)
{
	
//reading rotary and button
//check if rotation is left
 	if(ROTA && (!wait))
		wait=1;
	if (ROTB && ROTA && (wait))
		{
			
			rotarystatus=2;
			wait=2;
		}
	else if(ROTA && (!ROTB) && wait)
		{
			rotarystatus=1;
			wait=2;	
		}
	if ((!ROTA)&&!(ROTB)&&(wait==2))
		wait=0;
	//check button status
	/*if (ROTCLICK)
		{
			for(volatile uint16_t x=0;x<0x0FFF;x++);
			if (ROTCLICK)
			rotarystatus=3;
			
		}*/
}

//return button status
uint8_t RotaryGetStatus(void)
{
return rotarystatus;
}
//reset status
void RotaryResetStatus(void)
{
rotarystatus=0;
}
