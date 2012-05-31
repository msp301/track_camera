#include <stdlib.h>

#include <Servo.h>

Servo servo_x; //define servo to control x-axis movement
Servo servo_y; //define servo to control y-axis movement

int servo_x_position;
int servo_y_position;
int prev_x;
int prev_y;
int is_x_centre;
int is_y_centre;

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

	servo_y_position = 80;
	servo_y.write( servo_y_position );

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

		if( prev_x != NULL )
		{
			double change;
			double angle;

			is_x_centre = 1;
			if( x > ( 50 + 10 ) )
			{
				servo_x_position = servo_x_position + 5; //update x-axis servo position
				if( servo_x_position > 180 ) servo_x_position = 180; //stop at max position
				is_x_centre = 0;
			}
			else if( x < ( 50 - 10 ) )
			{
				servo_x_position = servo_x_position - 5; //update x-axis servo position
				if( servo_x_position < 0 ) servo_x_position = 0; //stop at min position
				is_x_centre = 0;
			}
		}

		if( prev_y != NULL )
		{
			double change;
			double angle;

			is_y_centre = 1;
			if( y > ( 37 + 5 ) )
			{
				servo_y_position = servo_y_position + 5; //update y-axis servo position
				if( servo_y_position > 100 ) servo_y_position = 100; //stop at min position
				is_y_centre = 0;
			}
			else if( y < ( 37 - 5 ) )
			{
				servo_y_position = servo_y_position - 5; //update y-axis servo position
				if( servo_y_position < 25 ) servo_y_position = 25; //stop at max position
				is_y_centre = 0;
			}
		}

		//check whether stand needs to be moved left or right
		if( !is_x_centre )
		{
			servo_x.write( servo_x_position ); //move servo to new position
			delay( 3 ); //wait for servo to move to new position
		}

		//check whether stand needs to be moved up or down
		if( !is_y_centre )
		{
			servo_y.write( servo_y_position );
			delay( 3 );
		}

		prev_x = x; //update just seen x coordinate as last seen
		prev_y = y; //update just seen y coordinate as last seen

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
	}

	return data;
}
