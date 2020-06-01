#include "C:\Keil\EE319Kware\inc\tm4c123gh6pm.h"
#include "stdint.h"
#include "string.h"

void SystemInit(void){}
void keypad_init(void);
int put_char(char c,char* myinput);
void solenoid_init(void);
void UART1_init(void);	
char getkey(void);
	
char operation[7] = {'\0','\0','\0','\0','\0','\0','\0'};
char password[5] = {'\0','\0','\0','\0','\0'};
unsigned char done = 0; // a flag to determine if UART sent the whole operation or still sending it
unsigned char rooms[20]; // used to store room numbers
int main(void)
{
	char c;
	char myinput[5] = {'\0','\0','\0','\0','\0'}; //user input in case of checkin mode
	solenoid_init();     //when solenoid is locked, red LED turns on, but when it's unlocked, green LED truns on
	keypad_init();
	UART1_init();
	
	while (1)    // 3 possible scenarios each time : checkin(1), cleaning room(2), or checkout(0)
	{ 
		__disable_irq();
		if (done)
		{
			if (operation[1] == '2') { GPIO_PORTF_DATA_R = (1<<3); }  //cleaning room
			else if (operation[1] == '0' ) { GPIO_PORTF_DATA_R = (1<<1); }  //checkout
			else if (operation[1] == '1' )   //checkin
			{
				GPIO_PORTA_DATA_R &= 0xF0;         // drive all rows to be low
				if ( (GPIO_PORTA_DATA_R & 0xF0) != 0xF0)   // check if any column are low 
				{
					c = getkey();
					if (put_char(c,myinput))
					{ if (!strcmp(myinput,password)) { GPIO_PORTF_DATA_R = (1<<3);}
						else {  GPIO_PORTF_DATA_R = (1<<1); }
					}
					while ( (GPIO_PORTA_DATA_R & 0xF0) != 0xF0);  // key is still not released
				}
			}
		}
		__enable_irq();
	}
}
