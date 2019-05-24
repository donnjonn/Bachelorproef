/*
Rotary encoder test 
 */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "rotary.c"
#include "PWM.c"

uint8_t counterstring[] = ""; //variable for storing the value of the variable you're currently modifying in string format
uint8_t counterdc = 50; //Initial duty cycle is 50%
uint8_t counteramp = 50; //Initial amplitude is 50%
double counterfreq = 75.0; //Initial frequency is 75hz (double because precision is 0.1Hz)
uint8_t modus = 1; //Initial mode is 1 (modify frequency)
uint8_t modestring[] = ""; //variable for storing the value of the modus in string format

void MainScreenUpdate(void) // main loop constantly calls this function to keep everything updated
{
	if(modus == 0) //Modify duty cycle
	{
		if (RotaryGetStatus() == 1) //Rotate left
		{
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree); //Cursor @ beginning of line 3
			_delay_us(80);
			lcd_write_string_4d("                    "); //Clear this line
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree); //Cursor @ beginning of line 3
			_delay_us(80);
			if (counterdc - 10 < 0) //Negative wrap around to 100%
				counterdc = 0;
			else
				counterdc -= 10;
			itoa(counterdc, counterstring, 10); //Store the new value in counterstring
			lcd_write_string_4d("Duty cycle: ");
			lcd_write_string_4d(counterstring); //print on lcd
			lcd_write_string_4d("%");
			_delay_us(80);
		}
			
		else if (RotaryGetStatus() == 2) //Rotate right
		{
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree); //Cursor @ beginning of line 3
			_delay_us(80);
			lcd_write_string_4d("                    "); //Clear this line
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree); //Cursor @ beginning of line 3
			_delay_us(80);
			if (counterdc + 10 >= 100) //Positive wrap around to 0%
				counterdc = 100;
			else
				counterdc += 10;
			itoa(counterdc, counterstring, 10); //Store the new value in counterstring
			lcd_write_string_4d("Duty cycle: ");
			lcd_write_string_4d(counterstring); //Print on lcd
			lcd_write_string_4d("%");
			_delay_us(80);
		}
			
		if (RotaryGetStatus() == 3) //Push button
		{
			RotaryResetStatus();
			modus = 1; //change to next mode 
			lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour); //Cursor @ beginning of line 3
			_delay_us(80);
			itoa(modus, modestring, 10); //Store the new value in modestring
			lcd_write_string_4d(modestring); //print modestring to lcd
			_delay_us(80);
		}
	}
	else if(modus == 1) //Modify Frequency
	{
		if (RotaryGetStatus() == 1) //Rotate left
		{
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo); //Cursor @ beginning of line 2
			_delay_us(80);
			lcd_write_string_4d("                    "); //Clear this line
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo); //Cursor @ beginning of line 2
			_delay_us(80);
			if (counterfreq - 0.1 <= 0) //Negative wrap around to 100Hz
				counterfreq = 100;
			else{
				counterfreq -= 0.1;
			}
			Freq_change(counterfreq,0);
			_delay_us(80);
			dtostrf(counterfreq,3,1, counterstring); //Store the new value in counterstring
			lcd_write_string_4d("Frequentie: ");
			lcd_write_string_4d(counterstring); //print on lcd
			lcd_write_string_4d("Hz"); 
			_delay_us(80);
		}
			
		else if (RotaryGetStatus() == 2)
		{
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo); //Cursor @ beginning of line 3
			_delay_us(80);
			lcd_write_string_4d("                    "); //Clear this line
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo); //Cursor @ beginning of line 3
			_delay_us(80);
			if (counterfreq + 0.1 >= 100) //Positive wrap around to 0%
				counterfreq = 100;
			else{
				counterfreq += 0.1;
			}
			Freq_change(counterfreq,0);
			_delay_us(80);
			dtostrf(counterfreq,3,1, counterstring); //Store the new value in counterstring
			lcd_write_string_4d("Frequentie: ");
			lcd_write_string_4d(counterstring); //print on lcd
			lcd_write_string_4d("Hz");
			_delay_us(80);
		}
			
		if (RotaryGetStatus() == 3)
		{
			RotaryResetStatus();
			modus = 2;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour); //Cursor @ beginning of line 3
			_delay_us(80);
			itoa(modus, modestring, 10); //Store the new value in modestring
			lcd_write_string_4d(modestring); //print modestring to lcd
			_delay_us(80);
		}
	}
	
	else if(modus == 2)
	{
		//update encoder status on screen
		if (RotaryGetStatus() == 1)
		{
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne); //Cursor @ beginning of line 3
			_delay_us(80);
			lcd_write_string_4d("                    "); //Clear this line
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne); //Cursor @ beginning of line 3
			_delay_us(80);
			if (counteramp - 10 < 0) //Negative wrap around to 100
				counteramp = 0;
			else{
				counteramp -= 10;
			}
			setPWM(counteramp);
			itoa(counteramp, counterstring, 10); //Store the new value in counterstring
			lcd_write_string_4d("Amplitude: ");
			lcd_write_string_4d(counterstring); //print on lcd
			lcd_write_string_4d("%");
			_delay_us(80);
		}
		
		else if (RotaryGetStatus() == 2)
		{
			RotaryResetStatus();
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne); //Cursor @ beginning of line 3
			_delay_us(80);
			lcd_write_string_4d("                    "); //Clear this line
			_delay_us(80);
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne); //Cursor @ beginning of line 3
			_delay_us(80);
			if (counteramp + 10 >= 100) //Positive wrap around to 0%
				counteramp = 100;
			else{
				counteramp += 10;
			}
			setPWM(counteramp);
			itoa(counteramp, counterstring, 10); //Store the new value in counterstring
			lcd_write_string_4d("Amplitude: ");
			lcd_write_string_4d(counterstring); //print on lcd
			lcd_write_string_4d("%");
			_delay_us(80);
		}
		
		if (RotaryGetStatus() == 3)
		{
			RotaryResetStatus();
			modus = 0;
			lcd_write_instruction_4d(lcd_SetCursor | lcd_lineFour); //Cursor @ beginning of line 3
			_delay_us(80);
			itoa(modus, modestring, 10); //Store the new value in modestring
			lcd_write_string_4d(modestring); //print modestring to lcd
			_delay_us(80);
		}
	}
}

double dcCalc(void) //Calculate the time the output to led needs to be enabled in secs
{
	double tijd;
	tijd = (1/counterfreq)*(counterdc/100.0);
	return tijd;
}