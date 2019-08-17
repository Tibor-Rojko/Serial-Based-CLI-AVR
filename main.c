/*
 * Command Line Interface (Using EEPROM)
 * main.c
 * Author : Tibor Rojko
 */ 
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "CLI.h"

int main(void)
{

	DDRB |= (1 << PINB5);
    
	initCLI();

	printString("Instructions:\r\n");
	printString("- Use setYear command to set the year (setYear=2019)\r\n");
	printString("- Use setMonth command to set the month (setMonth=1)\r\n");
	printString("- Use setDay command to set the day (setDay=17)\r\n");
	printString("\r\n");
	printString("When you press ENT, the actual settings will show up...\r\n");
	printString("\r\n");

    while (1) 
    {
		PORTB ^= (1 << PINB5);
		_delay_ms(1000);
    }
}



