#include <stdlib.h>

#include <Servo.h>

Servo servo_x; //define servo to control x-axis movement
Servo servo_y; //define servo to control y-axis movement

int servo_x_position;
int servo_y_position;
int prev_x;
int prev_y;

//function prototypes
wchar_t* readData( int available_data );

void setup()
{
	Serial.begin( 9600 ); //set baud rate
	pinMode( 7, OUTPUT ); //set pin 7 as digital output

	//setup servo control pins
	servo_x.attach( 5, 544, 2500 ); //set min & max pulses to ensure 180 degree rotation
	servo_y.attach( 6 ); //min & max pulses set to defaults (544 & 2400)

	servo_x_position = 90; //set starting angle to centre position
	servo_x.write( servo_x_position ); //move stand to default position

	//define previous coordinates as null
	prev_x = NULL;
	prev_y = NULL;
}

void loop()
{
	int data_size = Serial.available(); //check how much data is available

	//wait for data before continuing to process
	if( data_size >= 2 )
	{
		wchar_t *coord = readData( data_size );

		//convert received coordinates to integer values
		int x = coord[0];
		int y = coord[1];

		Serial.print( "prev_x = " );
		Serial.println( prev_x, DEC );

		if( prev_x != NULL )
		{
			double change;
			double angle;

			int is_centre = 1;
			if( x > ( 50 + 10 ) )
			{
				servo_x_position = servo_x_position + 5; //update x-axis servo position
				if( servo_x_position > 180 ) servo_x_position = 180; //stop at max position
				is_centre = 0;
			}
			else if( x < ( 50 - 10 ) )
			{
				servo_x_position = servo_x_position - 5; //update x-axis servo position
				if( servo_x_position < 0 ) servo_x_position = 0; //stop at min position
				is_centre = 0;

			}

			if( !is_centre )
			{
				servo_x.write( servo_x_position ); //move servo to new position
				delay( 3 ); //wait for servo to move to new position
			}

			Serial.print( "Servo position = " );
			Serial.println( servo_x_position, DEC );
		}

		prev_x = x; //update just seen coordinate as last seen

		Serial.flush();
	}
}

//read 'n' bytes of data off serial port
wchar_t* readData( int available_data )
{
	wchar_t data[ available_data ];

	//read given number of bytes of data off serial port
	for( int i=0; i < available_data; i++ )
	{
		data[i] = Serial.read();
		Serial.print( "Adding " );
		Serial.println( data[i] );
	}

	return data;
}
