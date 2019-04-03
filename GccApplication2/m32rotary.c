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
uint8_t counterstring[] = "";
uint8_t counter = 0;
static uint8_t voristatus = 0;
uint8_t accum2 = 0;
//init timer2 
void Timer2_Init(void)
{
	TCNT2=0x00;
} 
//start timer2
void Timer2_Start(void)
{
	TCCR2B|=(1<<CS22)|(1<<CS21)|(1<<CS20); //prescaler 256 ~122 interrupts/s
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
			if(voristatus == 1){
				accum2++;
			}
			else{
				accum2==0;
			}
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_Clear);
			_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			//ks0108DrawRoundRect(5, 20, 117, 20, 8, BLACK);
			//ks0108GotoXY(20,25);
			counter= counter-1-(1*accum)-(1*accum2);
			itoa(counter, counterstring, 10);
			lcd_write_string_4d(counterstring);
			_delay_us(80);
			voristatus == 1;
		}
		else if (RotaryGetStatus() == 2)
		{	
			if(voristatus == 1){
				accum2++;
			}
			else{
				accum2==0;
			}
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_Clear);
			_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			//ks0108DrawRoundRect(5, 20, 117, 20, 8, BLACK);
			//ks0108GotoXY(20,25);
			counter = counter + 1 +(1*accum2);;
			itoa(counter, counterstring, 10);
			lcd_write_string_4d(counterstring);
			_delay_us(80);
			voristatus == 1;
		}
		else {
			//accum2 = 0;
			//voristatus = 0;
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


