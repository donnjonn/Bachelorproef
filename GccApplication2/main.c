
// Program ID
#include "lcd.c"
#include "rot_enc.c"
#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t program_author[]   = " Jonas & Anton";
uint8_t program_version[]  = "LCD-AVR-4d (gcc)";
uint8_t program_date[]     = "Sep 16, 2013";
uint8_t program_txt[]	   = " Anton is cool";
uint8_t program_txt2[]	   = " [x] doubt";
uint8_t getal = 44;

/******************************* Main Program Code *************************/
int main(void)
{
// configure the microprocessor pins for the data lines
    lcd_D7_ddr |= (1<<lcd_D7_bit);                  // 4 data lines - output
    lcd_D6_ddr |= (1<<lcd_D6_bit);
    lcd_D5_ddr |= (1<<lcd_D5_bit);
    lcd_D4_ddr |= (1<<lcd_D4_bit);

// configure the microprocessor pins for the control lines
    lcd_E_ddr |= (1<<lcd_E_bit);                    // E line - output
    lcd_RS_ddr |= (1<<lcd_RS_bit);                  // RS line - output

// initialize the LCD controller as determined by the defines (LCD instructions)
    lcd_init_4d();                                  // initialize the LCD display for a 4-bit interface

// display the first line of information
    //lcd_write_string_4d(program_author);

// set cursor to start of second line
    //lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
    _delay_us(80);                                  // 40 uS delay (min)
	getal++;
	
	
	_delay_ms(1000);
// display the second line of information
    //lcd_write_string_4d(program_version);
	//lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree);
	//lcd_write_string_4d(program_txt);
	//lcd_write_string_4d(program_txt2);
	DDRB = 0xFF;
	TCNT1 = 40536;
	TCCR1A = 0x00;
	TCCR1B = (1<<CS11) | (1<<CS10);;
	TIMSK1 = (1 << TOIE1);
	sei();
// endless loop
    while(1);
    return 0;
}

ISR(TIMER1_OVF_vect) {
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	lcd_write_string_4d(" ");
	getal++;
	lcd_write_character_4d(getal);
	//_delay_us(80);
	PORTB ^= 0x10;
	TCNT1 = 40536;
}
/******************************* End of Main Program Code ******************/

/*============================== 4-bit LCD Functions ======================*/
/*
  Name:     lcd_init_4d
  Purpose:  initialize the LCD module for a 4-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/

 
        
