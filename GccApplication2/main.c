#include "lcd.c"
#include "m32rotary.c"
#include "AD9833.c"
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t countInterrupt;

void ExtIntInit(void)
{
	DDRD &= 0b11111011;
	EIMSK &= ~(1 << INT0); //External interrupt mask register - Disable INT0, to avoid interrupts whili initializing
	EICRA |= (1 << ISC01)|(1 << ISC00); //External Interrupt Control Register A - Trigger on rising edge
	EIFR &= ~(1 << INTF0); //External Interrupt Flag Register - flag register cleared
	EIMSK |= (1 << INT0); //Enable INT0 ==> INT0_vect
}

void Timer2_Init(void)
{
	TCNT2=0x00; //startvalue 0
	TCCR2B|=(1<<CS22)|(1<<CS21)|(1<<CS20); //prescaler 256 ~122 interrupts/s (genoeg voor rotary encoder)
	TIMSK2|=(1<<TOIE2);//Enable Timer2 Overflow interrupts ==> TIMER2_OVF_vect
}

void timer1_init(double tijd)
{
	countInterrupt = 0;
	OCR1A = (tijd*8/100)*(0xFFFF); //Set compare value to match given time
	TCCR1B |= (1 << WGM12); // Mode 4, Clear Timer on Compare on OCR1A
	TIMSK1 |= (1 << OCIE1A); //Set interrupt on compare match ==> TIMER1_COMPA_vect 
	TCCR1B |= (1 << CS12) | (1 << CS10); // set prescaler to 1024 and start the timer
}

void LcdInit(void)
{
	// All lcd datalines are output
	lcd_D7_ddr |= (1<<lcd_D7_bit);
	lcd_D6_ddr |= (1<<lcd_D6_bit);
	lcd_D5_ddr |= (1<<lcd_D5_bit);
	lcd_D4_ddr |= (1<<lcd_D4_bit);

	// All lcd control lines are output
	lcd_E_ddr |= (1<<lcd_E_bit);
	lcd_RS_ddr |= (1<<lcd_RS_bit);

	// initialize the LCD controller as determined by the defines (LCD instructions)
	lcd_init_4d();                                  // initialize the LCD display for a 4-bit interface
	
	lcd_write_instruction_4d(lcd_Clear); //clear lcd
	_delay_ms(4); //necessary delay
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne); //cursor on line one
	_delay_us(80); //necessary delay
	lcd_write_string_4d("Amplitude: 50%"); //write string
	_delay_us(80); //necessary delay
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo); //cursor line two
	_delay_us(80); //necessary delay
	lcd_write_string_4d("Frequentie: 75.0Hz"); //write string
	_delay_us(80); //necessary delay
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree); //cursor line four
	_delay_us(80); //necessary delay
	lcd_write_string_4d("Duty cycle: 50%"); //write string
	_delay_us(80); //necessary delay
}

void MainInit(void)
{
	//DDRB = 0xFF; //all B-pins set as output
	LcdInit(); //Init lcd display
	//Init functions
	RotaryInit(); //init rotary encoders
	Timer2_Init(); //init timer2 for rotary encoder interrupts
	ExtIntInit(); //Init external interrupts
	SPI_init();
	AD9833_init();
	sei(); //Enable interrupts
	setPWM(50);//set amplitude pwm to 50%
	Freq_change(75.0,0);
}
int main(void)
{
	MainInit();
// endless loop
    while(1){
		MainScreenUpdate(); //Keep updating the screen and other outputs
	}
    return 0;
}

ISR(INT0_vect) //Externe interrupt routine
{
	DDRD |= (1 << DDD6);
	// PD6 is now an output
	double tijd = dcCalc();
	timer1_init(tijd);
}

ISR(TIMER2_OVF_vect) //Timer2 overflow interrupt routine
{
	RotaryCheckStatus(); //reading rotary and button status
}

ISR(TIMER1_COMPA_vect) //Timer1 compare match interrupt routine
{
	countInterrupt++; //count amount of interrupts, when 3 reset this number and start routine
	if (countInterrupt == 3)
	{
		DDRD &= ~(1 << DDD6); //PD6 is no output
		TCCR1B &= 0b11111000; //Disable timer1
		countInterrupt = 0; //Reset counter
	}
}
 
        
