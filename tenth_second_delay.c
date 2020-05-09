//Using timer/counter 0, create a 0.1 s delay to be used as the tick rate in
//the stopwatch and timer features.
void Tenth_Second_Delay()
{
	//Timer needs to run 50 times to achieve desired delay
	unsigned char LOOP_COUNT = 50;

	//Initialize timer counter register to 0
	TCNT0 = 0;
	//Initialize output compare register to 124 (0->124 = 125 ticks)
	OCR0A = 124;
	//Enable output compare interrupt
	TIMSK = (1<<OCIE0)
	//Turn on timer configured for CTC mode and prescale of 256
	TCCR0B = (1<<WGM00) | (1<<CS02);

	//Run timer for 0.1s (50 * 125 * 256 = 1600000 => 0.1s)
	for(unsigned char i = 0; i < LOOP_COUNT; i++)
	{
		//Wait until compare match flag is raised
		while((TIFR0 & (1<<OCF0)) != 0);

		//Clear output compare flag
		TIFR0 = (1<<OCF0);
	}

	//Turn off timer
	TCCR0B = 0;

	return;
}