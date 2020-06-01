/* this file contains 2 functions: the first for configuration UART1 and make it ready for interrupts, the second is UART1 handler
which is called when the PC sends any operation to microcontroller whether this operation is: setup, checkin, room cleaning, or checkout*/

void UART1_init(void)
{
	SYSCTL_RCGCUART_R |= (1<<1);
	SYSCTL_RCGCGPIO_R |= (1<<1);
	while (!(SYSCTL_PRGPIO_R & (1<<1)));
	GPIO_PORTB_CR_R |= 0x03;
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_PCTL_R = ( GPIO_PORTB_PCTL_R & 0xFFFFFF00 ) | 0x00000011;  // we concern PB0 and PB1 only
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_AMSEL_R &=~ 0x03;
	UART1_CTL_R = 0;
	UART1_IBRD_R = 520;  // assume B.R = 9600 bits/sec and sys clock = 80MHZ, so BRD = 520.8333
	UART1_FBRD_R = 53;   // (.8333*64) + 0.5 = 53
	UART1_LCRH_R = 0x70;
	UART1_ICR_R = 0x10;
	UART1_IM_R = 0x10;
	NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF00FFFF ) | 0x00200000;
	NVIC_EN0_R |= (1<<6);
	UART1_CTL_R = 0x301;
}

void UART1_Handler(void)
{
	static char terminated = 0;  // a flag to determine if the current mode is setup mode or not
	static unsigned char i=0;
	unsigned char j = 0; 
	unsigned char temp;
	static unsigned char k = 0;
	UART1_ICR_R = 0x10;
	if (terminated)             // we have finished setup mode
	{
		done = 0;
		operation[i++] = UART1_DR_R & 0x0FF;
		if ((UART1_DR_R & 0x0FF) == '\n')       // when PC sends \n , then this operation is done and we are ready to get new one
		{i = 0;  done = 1; while(j<4) { password[j] = operation[j+2]; j++;}}
	}
	else              // we are still in setup mode 
	{
			temp = UART1_DR_R & 0x0FF;
			if (temp == 'T')   // a signal to terminate setup mode
				terminated = 1;
			else
				rooms[k++] = UART1_DR_R & 0x0FF;
	}
}
