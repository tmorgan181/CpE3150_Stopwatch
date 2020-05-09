//When started, play start beep then keep count of the running time (tenths of
//seconds). After increment, display one’s digit on LEDs. When split time
//button is pressed, print current total time in terminal. When stopped, play
//stop beep then print total time in terminal.
void Stopwatch()
{
	//Functions: 	SW1 = start
	//				SW2 = split time
	//				SW3 = stop
	//				SW4 = exit stopwatch

	//Set PORTA (switches) to input
	DDRA = 0x00;
	//Enable pull-up resistors
	PORTA = 0xFF;

	//Set PORTD (LEDs) to output
	DDRD = 0xFF;
	//Turn off LEDs
	PORTD = 0xFF;

	//Run stopwatch mode until exit button is pressed
	bool exit_stopwatch = false;
	while(!exit_stopwatch)
	{
		//Check SW1 (start button)
		if(~PINA & 0x01)
		{
			//Wait until button is released
			while(~PINA & 0x01);
			//Debounce

			//Play start beep
			startbeep();

			bool stopwatch_running = true;
			unsigned char total_time = 0;
			while(stopwatch_running)
			{
				//Delay and increment
				Tenth_Second_Delay();
				total_time++;

				//Display one's digit on LEDs
				PORTD = ~(total_time % 10);

				//Check SW2 (split time button)
				if(~PINA & 0x02)
				{
					//Display current time in terminal
					USART_SendString("Split Time: " + total_time + " tenths of a second");
				}

				//Check SW3 (stop button)
				if(~PINA & 0x04)
				{
					//Play stop beep
					stopbeep();

					//Stop timing
					stopwatch_running = false;
					//Display total run time in terminal
					USART_SendString("Total Run Time: " + total_time + " tenths of a second");
				}
			}
		}

		//Check SW4 (exit stopwatch button)
		if(~PINA & 0x08)
		{
			//Return to menu
			exit_stopwatch = true;
		}
	}

	return;
}