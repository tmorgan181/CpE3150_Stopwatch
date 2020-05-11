
#define F_CPU 16000000UL					// fosc = 16 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void USART_Init(unsigned long);				// initialize USART function
int USART_RxChar(void);						// Data receiving function
void USART_TxChar(char);					// Data transmitting function
void USART_SendString(char*);				// Send string of USART data function


//	This program uses USART1 to transmit and receive data from a serial terminal.
//	USART1 creates a virtual COM port if it is connected to a PC via a USB cable.
int TimerValue;  // used if using polling for receiving data


int main(void)
{
	DDRA = 0x00;	// set port A to input
	PORTA = 0xFF;	// use pull-up resistors on Port A for the input switches
	USART_Init(9600);  // initialize USART to 9600 baud

	while(1)
	{
		// this code is used if polling is used to check for received characters
		TimerValue= USART_RxChar();  // continually poll to see if a character is received
		// value will contain received byte if using serial

		// check to see if a button is pressed on the board
		// switches need to be debounced (that is not necessarily done in this code)
		if(~PINA & 0x01)  // check SW1 / PA0
		{
			while(~PINA & 0X01)
			{}
			delay();
			USART_SendString("Stopwatch Mode Activated.");
			Stopwatch();		
		}
		else if(~PINA & 0x02) // check SW2 / PA1
		{
			while(~PINA & 0X02)
			{}
			delay();
			USART_SendString("Timer Mode Activated.");
			DDRD=0XFF;
			Timer(TimerValue);
		}
		else if(~PINA & 0x04) // check SW3 / PA2
		{
			while(~PINA & 0X04)
			{}
			delay();
			USART_SendString("Quit Program.");
			//quit_program = true;
		}

		//USART_TxChar(value);
		
		
	}
	return 0;
}



// initializes USART1
void USART_Init(unsigned long BAUDRATE)				// USART initialize function
{
	int BAUD_PRESCALE = (((F_CPU / (BAUDRATE * 16UL))) - 1);  	// Define prescale value

	UCSR1B |= (1 << RXEN) | (1 << TXEN) ;			// Enable USART transmitter and receiver
	UCSR1C |= (1 << UCSZ0) | (1 << UCSZ1);			// Write USCRC for 8 bit data and 1 stop bit, asynchronous

	UBRR1L = BAUD_PRESCALE;							// Load UBRRL with lower 8 bit of prescale value
	UBRR1H = (BAUD_PRESCALE >> 8);					// Load UBRRH with upper 8 bit of prescale value
}


// Data receiving function
int USART_RxChar()
{
	if((UCSR1A & (1 << RXC)))	// checks to see if there is a character to receive
	return (UDR1);			//  if so, it returns the character
	else
	return '\0';			//  if not, it returns a null
}


// Data transmitting function
void USART_TxChar(char data)
{
	UDR1 = data;					 //writes data to be transmitting in UDR
	while (!(UCSR1A & (1<<UDRE)));	 //waits until data transmit and buffer get empty
}


// used to send a string of characters to the USART (instead of one character at a time).
void USART_SendString(char *str)
{
	int i=0;
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);						/* Send each char of string till the NULL */
		i++;
	}
}
void Timer(int TValue)
{
	startbeep();
	for(unsigned char i = TValue; i > 0; i--)
	{
		PORTD=~(i%10);
		Tenth_Second_Delay();
	}
	Flash();
}
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
void startbeep(void)
{
	DDRE=0X10;
	long count=10000;
	/* Replace with your application code */
	
	while(count>0)
	{
		TCCR0A=0X00;
		TCCR0B=0X02;
		TCNT0=-150;
		
		
		// use TIFR for ATmega32
		while (!(TIFR0 & (1<<TOV0)))
		{
		}
		
		TIFR0 = 0x01;   // write a 1 to TOV0 to reset overflow flag
		TCNT0 = -150;    // reset preload
		PORTE ^= 0x10; // 00001000
		count=count-1;
		
	}
	
	return;
}
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
void Flash(void)
{
	DDRE=0XFF;
	DDRD=0XFF;
	PORTE=0X00;
	PORTD=0X00;
	DDRA=0X00;
	PORTA=0XFF;
	
	while(1)
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
				if (~PINA & 0x01)
				{
					while(~PINA & 0x01)
					{}
					delay();
					PORTD=0XFF;
					PORTE=0XFF;
					return 0;
				}
			}
			
			TIFR0 = 0x01;   // write a 1 to TOV0 to reset overflow flag
			TCNT0 = -200;    // reset preload
			PORTE ^= 0x10; // 00001000
			count1=count1-1;
			
		}
		DDRE^=0XFF;
		DDRD^=0XFF;
	}
}
void delay()
{
	//TCNT0 = preload;   
	TCCR0A = 0b00000000;
	TCCR0B = 0b000000101;  // timer0 clk 1024 prescaler
	TCNT0 = -156;
	// TCCR0 = 0b00000101 for Atmega32
	while (!(TIFR0 & (1<<TOV0))); // polling
	TCCR0A = 0x00;
	TCCR0B = 0x00;        // turn off timer 0 - end of delay
	TIFR0 = 1<<TOV0;
	
}
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
			delay();

			//Play start beep
			startbeep();

			bool stopwatch_running = true;
			int total_time = 0;
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
					char* message;
					//Wait until button is released
					while(~PINA & 0x02);
					//Debounce
					delay();

					//Display current time in terminal
					char* buffer [sizeof(int)*8+1];
					message=strcat("Split Time: ",   itoa (total_time,buffer,10));
					message=strcat(message," tenths of a second.");
					USART_SendString(message);
					memset(buffer, 0, sizeof(buffer));

				}

				//Check SW3 (stop button)
				if(~PINA & 0x04)
				{
					//Wait until button is released
					while(~PINA & 0x04);
					//Debounce
					delay();

					//Play stop beep
					stopbeep();

					//Stop timing
					stopwatch_running = false;
					//Display total run time in terminal
					USART_SendString("Total Run Time: ");
					USART_TxChar(total_time);
					USART_SendString(" tenths of a second");
				}
			}
		}

		//Check SW4 (exit stopwatch button)
		if(~PINA & 0x08)
		{
			//Wait until button is released
			while(~PINA & 0x08);
			//Debounce
			delay();
			PORTD=0XFF;

			//Return to menu
			exit_stopwatch = true;
		}
	}

	return;
}
