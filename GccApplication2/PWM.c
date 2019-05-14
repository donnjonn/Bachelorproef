//https://sites.google.com/site/qeewiki/books/avr-guide/pwm-on-the-atmega328

#include <avr/io.h>

void setPWM(uint8_t counter)
{
	OCR0A = counter*2.55;
	// set PWM for 50% duty cycle

	TCCR0A |= (1 << COM0A1);
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS01);
	// set prescaler to 8 and starts PWM
}