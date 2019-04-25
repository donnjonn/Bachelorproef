/*
Rotary encoder test 
 */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "rotary.c"
#include "PWM.c"

//Strings in flash
const uint8_t left_string[] = "left";
const uint8_t right_string[] = "right";
uint8_t counterstring[] = "";
uint8_t counterdc = 0;
uint8_t mode = 0;
uint8_t modestring[] = "";
double counterfreq = 0;

void MainScreenUpdate(void)
{
	switch(mode){
		case 0:
		//update encoder status on screen
			if (RotaryGetStatus() == 1)
			{
				RotaryResetStatus();
				lcd_write_instruction_4d(lcd_Clear);
				_delay_ms(4);
				lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour);
				_delay_us(80);
				itoa(mode, modestring, 10);
				lcd_write_string_4d(modestring);
				lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
				_delay_us(80);
				if (counterdc-- == 0)
					counterdc = 100;
				setPWM(counterdc);
				itoa(counterdc, counterstring, 10);
				lcd_write_string_4d("Duty cycle: ");
				lcd_write_string_4d(counterstring);
				lcd_write_string_4d("%");
				_delay_us(80);
			}
			else if (RotaryGetStatus() == 2)
			{
				RotaryResetStatus();
				lcd_write_instruction_4d(lcd_Clear);
				_delay_ms(4);
				lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour);
				_delay_us(80);
				itoa(mode, modestring, 10);
				lcd_write_string_4d(modestring);
				lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
				_delay_us(80);
				if (counterdc++ >= 100)
					counterdc = 0;
				setPWM(counterdc);
				itoa(counterdc, counterstring, 10);
				lcd_write_string_4d("Duty cycle: ");
				lcd_write_string_4d(counterstring);
				lcd_write_string_4d("%");
				_delay_us(80);
			}
			if (RotaryGetStatus() == 3)
			{
				RotaryResetStatus();
				mode++;
			}
		case 1:
			//update encoder status on screen
			if (RotaryGetStatus() == 1)
			{
				RotaryResetStatus();
				lcd_write_instruction_4d(lcd_Clear);
				_delay_ms(4);
				lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
				_delay_us(80);
				if (counterfreq-0.1 == 0)
					counterfreq = 100;
				else{
					counterfreq+=0.1;
				}
				dtostrf(counterfreq,5,1, counterstring);
				lcd_write_string_4d("Frequentie: ");
				lcd_write_string_4d(counterstring);
				lcd_write_string_4d("Hz");
				_delay_us(80);
			}
			else if (RotaryGetStatus() == 2)
			{
				RotaryResetStatus();
				lcd_write_instruction_4d(lcd_Clear);
				_delay_ms(4);
				lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
				_delay_us(80);
				if (counterfreq+0.1 >= 100)
					counterfreq = 0;
				else{
					counterfreq+=0.1;
				}
				dtostrf(counterfreq,5,1, counterstring);
				lcd_write_string_4d("Frequentie: ");
				lcd_write_string_4d(counterstring);
				lcd_write_string_4d("Hz");
				_delay_us(80);
			}
			if (RotaryGetStatus() == 3)
			{
				RotaryResetStatus();
				mode--;
			}
	}
}


