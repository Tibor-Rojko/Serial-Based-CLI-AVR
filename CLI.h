/* 
 *	Simple serial based command line interface for driving LED or motors, relay and so on....
 *  CLI.h
 *  Author: Tibor Rojko
 */ 


#ifndef CLI_H_
#define CLI_H_

#define blocks_of_year		0
#define blocks_of_month		2
#define blocks_of_day		3
#define blocks_of_hour		4
#define blocks_of_minute	5

void initCLI();
void printString(char myString[]);
void readCMD(char myCMD[], char myPARAM[], uint8_t maxLength);
void cmdHandler(void);

#endif /* CLI_H_ */