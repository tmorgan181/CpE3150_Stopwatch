//Using timer/counter 0, create a 0.1 s delay to be used as the tick rate in
//the stopwatch and timer features.
void Tenth_Second_Delay()
{
	//Timer needs to run 50 times to achieve desired delay
	unsigned char LOOP_COUNT = 50;

	//Turn on timer configured for normal mode and prescale of 256
	TCCR0B = (1<<CS02);

	//Run timer for 0.1s (50 * 125 * 256 = 1600000 => 0.1s)
	for(unsigned char i = 0; i < LOOP_COUNT; i++)
	{
		//Initialize/reset counter value
		TCNT0 = -125;

		//Wait until compare match flag is raised
		while(!(TIFR0 & (1<<TOV0)));

		//Clear overflow flag
		TIFR0 = (1<<TOV0);
	}

	//Turn off timer
	TCCR0B = 0;

	return;
}