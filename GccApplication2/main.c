#include "lcd.c"
#include "m32rotary.c"
#include "AD9833.c"
#include <avr/io.h>
#include <avr/interrupt.h>

//#define F_CPU 16000000
uint8_t countInterrupt;

void ExtIntInit(void)
{
	//DDRD = 0xf0;
	DDRD &= 0b11111011;
	EIMSK &= ~(1 << INT0); //External Interrupt Mask Register - EIMSK - is for enabling INT[6;3:0] interrupts, INT0 is disabled to avoid false interrupts when mainuplating EICRA
	EICRA |= (1 << ISC01)|(1 << ISC00); //External Interrupt Control Register A - EICRA - defines the interrupt edge profile, here configured to trigger on rising edge
	EIFR &= ~(1 << INTF0); //External Interrupt Flag Register - EIFR controls interrupt flags on INT[6;3:0], here it is cleared
	EIMSK |= (1 << INT0); //Enable INT0
}

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

void timer1_init(double tijd)
{
	countInterrupt = 0;
	//OCR1A = 0xffff;
	OCR1A = (tijd*8/100)*(0xFFFF);

	TCCR1B |= (1 << WGM12);
	// Mode 4, CTC on OCR1A

	TIMSK1 |= (1 << OCIE1A);
	//Set interrupt on compare match

	TCCR1B |= (1 << CS12) | (1 << CS10);
	// set prescaler to 1024 and start the timer
}

void LcdInit(void)
{
	lcd_write_instruction_4d(lcd_Clear);
	_delay_ms(4);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineOne);
	_delay_us(80);
	lcd_write_string_4d("Amplitude: 50%");
	_delay_us(80);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
	_delay_us(80);
	lcd_write_string_4d("Frequentie: 75.0Hz");
	_delay_us(80);
	lcd_write_instruction_4d(lcd_SetCursor | lcd_LineThree);
	_delay_us(80);
	lcd_write_string_4d("Duty cycle: 50%");
	_delay_us(80);
}

void MainInit(void)
{
	//all B-pins to output
	DDRB = 0xFF;
	
	//init
	RotaryInit();
	Timer2_Init();
	Timer2_Start();
	ExtIntInit();
	//SPI_init();
	//AD9833_init();
	//Freq_change(75,0);
	
	//enable global interrupts
	sei();
	LcdInit();
	setPWM(50);
}
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

// set cursor to start of second line
    lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
    _delay_us(80);                                  // 80 uS delay (min)
	
	MainInit();
	
// endless loop
    while(1){
		//_delay_ms(100);
		MainScreenUpdate();
	}
    return 0;
}

ISR(INT0_vect)
{
	DDRD |= (1 << DDD6);
	// PD6 is now an output
	double tijd = dcCalc();
	timer1_init(tijd);
}

ISR(TIMER2_OVF_vect)
{
	//reading rotary and button
	RotaryCheckStatus();
}

ISR(TIMER1_COMPA_vect)
{
	countInterrupt++;
	if (countInterrupt == 3)
	{
		DDRD &= ~(1 << DDD6);
		// PD6 is now no output
		TCCR1B &= 0b11111000;
		countInterrupt = 0;
	}
	//TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10);
}
 
        
