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
uint8_t counterstring[] = "";
uint8_t counterdc = 50;
uint8_t counteramp = 50;
uint8_t modus = 1;
uint8_t modestring[] = "";
double counterfreq = 75.0;

void MainScreenUpdate(void)
{
	if(modus == 0)
	{
	//update encoder status on screen
		if (RotaryGetStatus() == 1)
		{
			RotaryResetStatus();
			//lcd_write_instruction_4d(lcd_Clear);
			//_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree);
			_delay_us(80);
			lcd_write_string_4d("                    ");
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree);
			_delay_us(80);
			if (counterdc - 10 <= 0)
				counterdc = 100;
			else
				counterdc -= 10;
			itoa(counterdc, counterstring, 10);
			lcd_write_string_4d("Duty cycle: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("%");
			_delay_us(80);
		}
			
		else if (RotaryGetStatus() == 2)
		{
			RotaryResetStatus();
			//lcd_write_instruction_4d(lcd_Clear);
			//_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree);
			_delay_us(80);
			lcd_write_string_4d("                    ");
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree);
			_delay_us(80);
			if (counterdc + 10 >= 100)
				counterdc = 0;
			else
				counterdc += 10;
			itoa(counterdc, counterstring, 10);
			lcd_write_string_4d("Duty cycle: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("%");
			_delay_us(80);
		}
			
		if (RotaryGetStatus() == 3)
		{
			RotaryResetStatus();
			modus = 1;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour);
			_delay_us(80);
			itoa(modus, modestring, 10);
			lcd_write_string_4d(modestring);
			_delay_us(80);
		}
	}
	else if(modus == 1)
	{
		//update encoder status on screen
		if (RotaryGetStatus() == 1)
		{
			RotaryResetStatus();
			//lcd_write_instruction_4d(lcd_Clear);
			//_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			lcd_write_string_4d("                    ");
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			if (counterfreq - 1 <= 0)
				counterfreq = 100;
			else{
				counterfreq -= 1;
			}
			//Freq_change(counterfreq);
			
			dtostrf(counterfreq,3,1, counterstring);
			lcd_write_string_4d("Frequentie: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("Hz");
			_delay_us(80);
		}
			
		else if (RotaryGetStatus() == 2)
		{
			RotaryResetStatus();
			//lcd_write_instruction_4d(lcd_Clear);
			//_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			lcd_write_string_4d("                    ");
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
			_delay_us(80);
			if (counterfreq + 1 >= 100)
				counterfreq = 0;
			else{
				counterfreq += 1;
			}
			//Freq_change(counterfreq);
			dtostrf(counterfreq,3,1, counterstring);
			lcd_write_string_4d("Frequentie: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("Hz");
			_delay_us(80);
		}
			
		if (RotaryGetStatus() == 3)
		{
			RotaryResetStatus();
			modus = 2;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour);
			_delay_us(80);
			itoa(modus, modestring, 10);
			lcd_write_string_4d(modestring);
			_delay_us(80);
		}
	}
	
	else if(modus == 2)
	{
		//update encoder status on screen
		if (RotaryGetStatus() == 1)
		{
			RotaryResetStatus();
			//lcd_write_instruction_4d(lcd_Clear);
			//_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			_delay_us(80);
			lcd_write_string_4d("                    ");
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			_delay_us(80);
			if (counteramp - 10 <= 0)
				counteramp = 100;
			else{
				counteramp -= 10;
			}
			setPWM(counteramp);
			itoa(counteramp, counterstring, 10);
			lcd_write_string_4d("Amplitude: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("%");
			_delay_us(80);
		}
		
		else if (RotaryGetStatus() == 2)
		{
			RotaryResetStatus();
			//lcd_write_instruction_4d(lcd_Clear);
			//_delay_ms(4);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			_delay_us(80);
			lcd_write_string_4d("                    ");
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
			_delay_us(80);
			if (counteramp + 10 >= 100)
				counteramp = 0;
			else{
				counteramp += 10;
			}
			setPWM(counteramp);
			itoa(counteramp, counterstring, 10);
			lcd_write_string_4d("Amplitude: ");
			lcd_write_string_4d(counterstring);
			lcd_write_string_4d("%");
			_delay_us(80);
		}
		
		if (RotaryGetStatus() == 3)
		{
			RotaryResetStatus();
			modus = 0;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour);
			_delay_us(80);
			itoa(modus, modestring, 10);
			lcd_write_string_4d(modestring);
			_delay_us(80);
		}
	}
}

double dcCalc(void)
{
	double tijd;
	tijd = (1/counterfreq)*(counterdc/100.0);
	return tijd;
}