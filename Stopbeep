void stopbeep(void)
{
		DDRE=0X10;
		long count=5000;
    /* Replace with your application code */ 
       
       while(count>0)
       {
	       TCCR0A=0X00;
		   TCCR0B=0X02;
		   TCNT0=-199;
	       
	       
	       // use TIFR for ATmega32
	       while (!(TIFR0 & (1<<TOV0)))
	       {
	       }
	       
	       TIFR0 = 0x01;   // write a 1 to TOV0 to reset overflow flag
	       TCNT0 = -16;    // reset preload
	       PORTE ^= 0x10; // 00001000
		   count=count-1;
	       
       }
       
       return 0;
       }

