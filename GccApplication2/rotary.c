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
	if(ROTA & (!ROTB))
	{
		loop_until_bit_is_set(ROTPIN, ROTPA);
		if (ROTB)
			rotarystatus = 1;
		//check if rotation is right
	}
	else if(ROTB & (!ROTA))
	{
		loop_until_bit_is_set(ROTPIN, ROTPB);
		if (ROTA)
			rotarystatus = 2;
	}
	else if (ROTA & ROTB)
	{
		loop_until_bit_is_set(ROTPIN, ROTPA);
		if (ROTB)
			rotarystatus = 1;
		else 
			rotarystatus = 2;
	}
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
