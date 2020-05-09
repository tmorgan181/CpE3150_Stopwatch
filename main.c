
#define F_CPU 16000000UL					// fosc = 16 MHz
#define preload -200

#include <avr/io.h>
#include <avr/interrupt.h>


void USART_Init(unsigned long);				// initialize USART function
char USART_RxChar();						// Data receiving function
void USART_TxChar(char);					// Data transmitting function
void USART_SendString(char*);				// Send string of USART data function
void delayroutine();


//	This program uses USART1 to transmit and receive data from a serial terminal.
//	USART1 creates a virtual COM port if it is connected to a PC via a USB cable.

unsigned char value;  // used if using polling for receiving data


int main(void)
{
	DDRA = 0x00;	// set port A to input
	PORTA = 0xFF;	// use pull-up resistors on Port A for the input switches
	
	
	USART_Init(9600);  // initialize USART to 9600 baud

	while(1)
	{

		// this code is used if polling is used to check for received characters
		value = USART_RxChar();  // continually poll to see if a character is received

		// value will contain received byte if using serial

		// check to see if a button is pressed on the board
		// switches need to be debounced (that is not necessarily done in this code)
		if(~PINA & 0x01)  // check SW1 / PA0
		{
			USART_SendString("Stopwatch Mode Activated.");
			//Stopwatch();		_delay_ms(500);
		}
		else if(~PINA & 0x02) // check SW2 / PA1
		{
			USART_SendString("Timer Mode Activated.");
			//Timer();
		}
		else if(~PINA & 0x04) // check SW3 / PA2
		{
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
char USART_RxChar()
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

void delayroutine()
{
	//TCNT0 = preload;   
	TCCR0A = 0b00000000;
	TCCR0B = 0b000000101;  // timer0 clk 1024 prescaler
	// TCCR0 = 0b00000101 for Atmega32
	while (!(TIFR0 & (1<<TOV0))); // polling
	TCCR0A = 0x00;
	TCCR0B = 0x00;        // turn off timer 0 - end of delay
	TIFR0 = 1<<TOV0;
	
}
