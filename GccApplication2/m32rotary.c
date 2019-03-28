/*
Rotary encoder test 
 */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "rotary.c"

//Strings in flash
const uint8_t left_string[] = "left";
const uint8_t right_string[] = "right";

//init timer2
void Timer2_Init(void)
{
	TCNT2=0x00;
} 
//start timer2
void Timer2_Start(void)
{
	TCCR2B|=(1<<CS22)|(1<<CS21); //prescaler 256 ~122 interrupts/s
	TIMSK2|=(1<<TOIE2);//Enable Timer0 Overflow interrupts
}
//timer ISR check for rotary encoder status
ISR(TIMER2_OVF_vect)
{
	//reading rotary and button
	RotaryCheckStatus();
}
void MainScreenUpdate(void)
{
		//lcd_write_string_4d("doubt");
		//_delay_us(80);
		//update encoder status on screen
		if (RotaryGetStatus() == 1)
		{	
			
			PORTB ^=  0x10;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			lcd_write_instruction_4d(lcd_Clear);
			_delay_us(80);
			//ks0108DrawRoundRect(5, 20, 117, 20, 8, BLACK);
			//ks0108GotoXY(20,25);
			lcd_write_string_4d("left");
			_delay_us(80);
			RotaryResetStatus();
		}
		else if (RotaryGetStatus() == 2)
		{	
			PORTB ^=  0x10;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			lcd_write_instruction_4d(lcd_Clear);
			_delay_us(80);
			//ks0108DrawRoundRect(5, 20, 117, 20, 8, BLACK);
			//ks0108GotoXY(20,25);
			lcd_write_string_4d("right");
			_delay_us(80);
			RotaryResetStatus();
		}
}
/*int main(void) {
	//initialize peripherals
	MainInit();
	//start loop
	while(1)
	{
		//update LCD information on status change
		MainScreenUpdate();
	}
	
}*/


