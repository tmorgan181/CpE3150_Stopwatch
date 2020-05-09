void flash(void)
{
	DDRE=0XFF;
	DDRD=0XFF;
	PORTE=0X00;
	PORTD=0X00;

	short count2=5;
	
	while(count2>0)
	{
		short count1=30;
		while(count1>0)
		{
			TCCR0A=0X00;
			TCCR0B=0X05;
			TCNT0=-200;
			
			
			// use TIFR for ATmega32
			while (!(TIFR0 & (1<<TOV0)))
			{
			}
			
			TIFR0 = 0x01;   // write a 1 to TOV0 to reset overflow flag
			TCNT0 = -200;    // reset preload
			PORTE ^= 0x10; // 00001000
			count1=count1-1;
			
		}
		DDRE^=0XFF;
		DDRD^=0XFF;
		count2=count2-1;
	}
	return 0;
}
