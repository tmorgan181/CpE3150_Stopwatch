//Using timer/counter 0, create a 0.1 s delay to be used as the tick rate in
//the stopwatch and timer features.
void Tenth_Second_Delay()
{
	//Timer needs to run 50 times to achieve desired delay
	unsigned char LOOP_COUNT = 50;

	//Initialize timer counter register to 0
	TCNT0 = 0;
	//Initialize output compare register to 124 (0->124 = 125 ticks)
	OCR0A = 0x00;
	OCR0B = 124;
	//Configure timer for CTC mode and prescale of 256
	TCCR0A = 0x00;
	TCCR0B = 0x0C;

	//Run timer for 0.1 s
	for(unsigned char i = 0; i < LOOP_COUNT; i++)
	{
		//Wait until compare match flag is raised
		while((TIFR0 & 0x02) != 0);

		//Clear output compare flag
		TIFR0 = 0xFF;
	}

	//Turn off timer
	TCCR0A = 0x00;
	TCCR0B = 0x00;

	return;
}