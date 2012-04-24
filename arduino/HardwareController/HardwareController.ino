#include <stdlib.h>

//function prototypes
char* readData( int available_data );

void setup()
{
	Serial.begin( 9600 ); //set baud rate
	pinMode( 7, OUTPUT ); //set pin 7 as digital output
}

void loop()
{
	int data_size = Serial.available(); //check how much data is available

	//wait for data before continuing to process
	if( data_size == 2 )
	{
		char *coord = readData( data_size );

		//convert received coordinates to integer values
		int x = coord[0];
		int y = coord[1];

		Serial.print( "X = " );
		Serial.println( x, DEC );
		Serial.print( "Y = " );
		Serial.println( y, DEC );
		Serial.println( "------" );
		Serial.flush();
	}
}

//read 'n' bytes of data off serial port
char* readData( int available_data )
{
	char data[ available_data ];

	//read given number of bytes of data off serial port
	for( int i=0; i < available_data; i++ )
	{
		data[i] = Serial.read();
		Serial.print( "Adding " );
		Serial.println( data[i] );
	}

	return data;
}
