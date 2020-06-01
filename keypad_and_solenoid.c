/* this file contains all functions needed to configure keypad, get a char from it into microcontroller, load this character
into 4-digit password, and configure solenoid as it has only 2 states: locked(red led is on) , unlocked(green led is on).*/

void keypad_init(void)
{
	SYSCTL_RCGCGPIO_R |= (1<<0);
	while (!(SYSCTL_PRGPIO_R & (1<<0)));
	GPIO_PORTA_CR_R = 0xFF;
	GPIO_PORTA_AFSEL_R = 0;
	GPIO_PORTA_DEN_R = 0xFF;
	GPIO_PORTA_AMSEL_R =~ 0xFF;
	GPIO_PORTA_DIR_R = 0x0F;    // PA0-PA3 are rows(output)  , PA4-PA7 are columns (input)
	GPIO_PORTA_ODR_R = 0x0F;    // open drain to protect output pins from short sircuits
	GPIO_PORTA_PUR_R = 0xF0;    // pull up resistors to avoid noise effect
}

char getkey(void)
{
	unsigned char row;
	char keypad[4][4] = {
		{ '1', '2', '3', 'A'},
		{ '4', '5', '6', 'B'},
		{ '7', '8', '9', 'C'},
		{ '*', '0', '#', 'D'}
	};
	while (1) 
	{
		GPIO_PORTA_DATA_R = ( GPIO_PORTA_DATA_R & 0xF0 ) | 0x0E;   // only first row is low
		if ( (GPIO_PORTA_DATA_R & 0xF0) != 0xF0 ) { row=0; break; }  // key pressed belongs to first row
		GPIO_PORTA_DATA_R = ( GPIO_PORTA_DATA_R & 0xF0 ) | 0x0D;   // only second row is low
		if ( (GPIO_PORTA_DATA_R & 0xF0) != 0xF0 ) { row=1; break; }  // key pressed belongs to second row
		GPIO_PORTA_DATA_R = ( GPIO_PORTA_DATA_R & 0xF0 ) | 0x0B;   // only third row is low
		if ( (GPIO_PORTA_DATA_R & 0xF0) != 0xF0 ) { row=2; break; }  // key pressed belongs to third row
		GPIO_PORTA_DATA_R = ( GPIO_PORTA_DATA_R & 0xF0 ) | 0x07;   // only fourth row is low
		if ( (GPIO_PORTA_DATA_R & 0xF0) != 0xF0 ) { row=3; break; }  // key pressed belongs to fourth row
	}
	if ( (GPIO_PORTA_DATA_R & 0xF0) == 0xE0 ) { return keypad[row][0]; }
  else if ( (GPIO_PORTA_DATA_R & 0xF0) == 0xD0 ) { return keypad[row][1]; }
	else if ( (GPIO_PORTA_DATA_R & 0xF0) == 0xB0 ) { return keypad[row][2]; }
	return keypad[row][3]; 
}

int put_char(char c,char* myinput)  // returns 1 if all 4 digits are put , 0 otherwise
{
	static unsigned char i =0;
	myinput[i++] = c;
	if (i==4) { i=0; return 1;}
	return 0;	
}

void solenoid_init(void)  // we have PD0 connected to selonoid and Red led turns on in case it's locked, Green led turns on in case it's unlocked
{
	SYSCTL_RCGCGPIO_R |= (1<<5);
	while(!(SYSCTL_PRGPIO_R & (1<<5)));
	GPIO_PORTF_CR_R =0x0A;
	GPIO_PORTF_AFSEL_R = 0;
	GPIO_PORTF_DEN_R = 0xFF;
	GPIO_PORTF_AMSEL_R = 0;
	GPIO_PORTF_DIR_R |= 0x0A;
	SYSCTL_RCGCGPIO_R |= (1<<3);
	while(!(SYSCTL_PRGPIO_R & (1<<3)));
	GPIO_PORTD_CR_R =0x01;
	GPIO_PORTD_AFSEL_R = 0;
	GPIO_PORTD_DEN_R = 0xFF;
	GPIO_PORTD_AMSEL_R = 0;
	GPIO_PORTD_DIR_R |= 0x01;
}
