#include <avr/io.h>
#include <math.h>
#include "AD9833.h"


/*************************************************************************
Function: SPI_init()
Purpose:  initialize the SPI bus 
Input:    none
Returns:  none
**************************************************************************/
void SPI_init (void) //Initialize SPI-bus
{
	//DDRB = 0xFF;
	DDRB |= ((1<<PB3) | (1<<PB2) | (1<<PB5));
	//PORTB = 0xFF;
	PORTB |= ((1<<PB3) | (1<<PB2) | (1<<PB5));
	
	DDRB  = _BV(PB2) | _BV(PB3) | _BV(PB5);	// 	set SCK,MOSI,PB2 as Fsync 
	PORTB = _BV(PB5) | _BV(PB2);			// 	SCK and PB2 high 
	SPCR  = _BV(SPE)| _BV(MSTR)| _BV(CPOL); // 	Enable SPI // Set Master mode //	Set clk to inv.
	// SPCR |= _BV(SPR0)					//	Clk speed = fck/4, SPR0,SPR1 = 0,0 // uncomment for fck/16
}


/*************************************************************************
Function: SPI_write16()
Purpose:  send a 16bit word to the AD9833 
Input:    unsigned short data = 16bits
Returns:  none
Comment:  uses 8bit filter and two consecutive writes while fsync stays low
**************************************************************************/
void SPI_write16 (unsigned short data)    	// 	send a 16bit word and use fsync
{

	unsigned char MSdata = ((data>>8) & 0x00FF);  	//filter out MS
	unsigned char LSdata = (data & 0x00FF);			//filter out LS

	PORTB &= ~_BV(PB2);						// 	Fsync Low --> begin frame
	_delay_ms(10);
	
	SPDR = MSdata;							// 	send First 8 MS of data
	while (!(SPSR & (1<<SPIF)));			//	while busy

	SPDR = LSdata;							// 	send Last 8 LS of data
	while (!(SPSR & (1<<SPIF)));			//	while busy

	PORTB |= _BV(PB2);						// 	Fsync High --> End of frame
}


/*************************************************************************
Function: Freq_change()
Purpose:  change the frequency and select AD9833 onboard register
Input:    unsigned short freq_out = frequency, unsigned int select = register 0 or 1
Returns:  none
Comment:  uses 14 bit filter and adds control words, 
**************************************************************************/
void Freq_change ( double freq_out, unsigned int select )  // take base10 frequency and do frequency hop
{
	unsigned long freq_reg = freq_out * 65.536; 	// make freq register from frequency // set for 4 MHz Mclk
	unsigned short MS_reg = ((freq_reg>>14) & 0x3FFF);  // filter out MS -- make 2 x 14 bit frequency words
	unsigned short LS_reg = (freq_reg & 0x3FFF);		// filter out LS -- make 2 x 14 bit frequency words

	MS_reg += 0x4000; 									// add control bits hex = 0x4000
	LS_reg += 0x4000; 									// add control bits hex = 0x4000

	if (select == 0 ) { SPI_write16(0x2000);}			// prep ad9833 to receive full 28bit word for freq 0
	if (select == 1 ) { SPI_write16(0x2800);}			// prep ad9833 to receive full 28bit word for freq 1
	SPI_write16(0x2028);

	SPI_write16(LS_reg);								// send the LS word first, to the ad9833
	SPI_write16(MS_reg);								// send the MS word last,  to the ad9833
}
/*
void Phase_change ( double phase_out, unsigned int select )  // take base10 phase and do phase hop
{
	unsigned long phase_reg = phase_out * 65.536; 	// make freq register from frequency // set for 4 MHz Mclk
	unsigned short MS_reg = ((phase_reg>>14) & 0x3FFF);  // filter out MS -- make 2 x 14 bit frequency words
	unsigned short LS_reg = (phase_reg & 0x3FFF);		// filter out LS -- make 2 x 14 bit frequency words

	MS_reg += 0x4000; 									// add control bits hex = 0x4000
	LS_reg += 0x4000; 									// add control bits hex = 0x4000

	if (select == 0 ) { SPI_write16(0x2000);}			// prep ad9833 to receive full 28bit word for freq 0
	if (select == 1 ) { SPI_write16(0x2800);}			// prep ad9833 to receive full 28bit word for freq 1
	SPI_write16(0x2028);

	SPI_write16(LS_reg);								// send the LS word first, to the ad9833
	SPI_write16(MS_reg);								// send the MS word last,  to the ad9833
}
*/

/*************************************************************************
Function: AD9833_init()
Purpose:  Init the AD9833
Input:    none
Returns:  none
Comment:  this function isn't necessary, can be done manually
**************************************************************************/
void AD9833_init (void)
{
	SPI_write16(0b0000000100000000);		// reset
	_delay_ms(50);
	//SPI_write16(0b0000000001101000);		// init
	SPI_write16(0b0101001110101001);		// LSB 01001110101001 = 75Hz

	SPI_write16(0b1100000000000000);		// phase = 0
	SPI_write16(0x0028);
	//SPI_write16(0b0000000001101000);
}


