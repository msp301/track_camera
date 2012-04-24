#include <stdlib.h>

#include <Servo.h>

Servo servo_x; //define servo to control x-axis movement
Servo servo_y; //define servo to control y-axis movement

//function prototypes
char* readData( int available_data );

void setup()
{
	Serial.begin( 9600 ); //set baud rate
	pinMode( 7, OUTPUT ); //set pin 7 as digital output

	//setup servo control pins
	servo_x.attach( 5, 544, 2500 ); //set min & max pulses to ensure 180 degree rotation
	servo_y.attach( 6 ); //min & max pulses set to defaults (544 & 2400)
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
