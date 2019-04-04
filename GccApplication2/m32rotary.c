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
uint8_t counter = 0;

void MainScreenUpdate(void)
{
		//update encoder status on screen
		if (RotaryGetStatus() == 1)
		{	
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_Clear);
			_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			_delay_us(80);
			if (counter-- == 0)
				counter = 100;
			setPWM(counter);
			itoa(counter, counterstring, 10);
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
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			_delay_us(80);
			if (counter++ >= 100)
				counter = 0;
			setPWM(counter);
			itoa(counter, counterstring, 10);
			lcd_write_string_4d("Duty cycle: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("%");
			_delay_us(80);
		}
}


