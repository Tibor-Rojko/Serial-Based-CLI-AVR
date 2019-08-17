/* 
 *  Simple serial based command line interface for driving LED or motors, relay and so on....
 *  CLI.c
 *  Author: Tibor Rojko
 */ 

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <avr/eeprom.h>
 #define F_CPU 16000000
 #define BAUD 9600
 #include <util/setbaud.h>
 #include "CLI.h"

 //help variables
 uint8_t maxLenght = 50;
 char CMD[10];
 char PARAM[10];
 char buff[50];

 //Settings variables, actualy year-month-day, just for example
 uint16_t year;
 uint8_t month;
 uint8_t day;

 void initCLI()
 {
	//Set baud rate - Set USART/UART Baud Rate Register - defined in setbaud.h
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	//Enable receiver and transmitter, and receive interrupt
	UCSR0B |= ((1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0));

	//Set frame format: 8 data, 1 stop bit
	UCSR0C = 3 << UCSZ00;
	
	//Enable global interrupt
	sei();

	 year = eeprom_read_word((uint16_t *) blocks_of_year);
	 month = eeprom_read_byte((uint8_t *) blocks_of_month);
	 day = eeprom_read_byte((uint8_t *) blocks_of_day);

 }

 
static void transmitByte(unsigned char data)
{
	// Wait for empty transmit buffer
	/*
	The UDRE0 flag indicates if the transmit buffer (UDR0) is ready to receive new data. If UDRE0 is one, the buffer is empty,
	and therefore ready to be written.
	*/
	while (!(UCSR0A & (1<<UDRE0)));
	// Put data into buffer, sends the data 
	UDR0 = data;
}

void printString(char myString[])
{
	uint8_t i = 0;
	while (myString[i]) {
		transmitByte(myString[i]);
		i++;
	}
}

static unsigned char receiveByte(void)
{
	/* Wait for data to be received 
	RXC0 flag bit is set when there are unread data in the receive buffer and cleared when the receive buffer is empty
	*/
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

 void readCMD(char myCMD[], char myPARAM[], uint8_t maxLength)
 {
	 uint8_t i;
	 char response;
	 i = 0;
	 uint8_t part = 0;
	 while (i < (maxLength - 1)) {                   //prevent over-runs
		 response = receiveByte();
		 transmitByte(response);		 //echoing
		 if (response == '\r') {                 //enter marks (\r) the end
			 break;
		 }
		 if (response == '=')
		 {
			 part = 1;
			 i = 0;
		 }
		 if(!part)
		 {
			 myCMD[i] = response;
		 }
		 else
		 {
			 myPARAM[i] = response;
		 }
		 if(response != '='){
			 i++;
		 }
	 }
 }


 void cmdHandler(void)
 {
	if(!strcmp(CMD, "setYear"))
	{
		year = atoi(PARAM);
		//Setting the year
		//Write Year to EEPROM | 16 bit 2 block | 0-1 >> block 0
		eeprom_update_word((uint16_t *) blocks_of_year, year);
		year = eeprom_read_word((uint16_t *) blocks_of_year);
		sprintf(buff, "Year set to %d\r\n", year);
		printString(buff);
	}
	else if(!strcmp(CMD, "setMonth"))
	{
		//setting month
		month = atoi(PARAM);
		eeprom_update_byte((uint8_t *) blocks_of_month, month);
		month = eeprom_read_byte((uint8_t *) blocks_of_month);
		sprintf(buff, "Month set to %d\r\n", month);
		printString(buff);
	}
	else if(!strcmp(CMD, "setDay"))
	{
		//setting day
		day = atoi(PARAM);
		eeprom_update_byte((uint8_t *) blocks_of_day, day);
		day = eeprom_read_byte((uint8_t *) blocks_of_day);
		sprintf(buff, "Day set to %d\r\n", day);
		printString(buff);
	}
	else
	{
		sprintf(buff, "Actual settings: %d-%d-%d\r\n", year, month, day);
		printString(buff);
	}
 }

 static void erase(char myString[])
 {
	 for(uint8_t i = 0; i<=9; i++)
	 {
		 myString[i] = '\0';
	 }
 }

 ISR(USART_RX_vect)
 {
	 readCMD(CMD, PARAM, maxLenght);
	 cmdHandler();
	 erase(CMD);
	 erase(PARAM);
 }
