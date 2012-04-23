void setup()
{
	Serial.begin( 9600 ); //set baud rate
	pinMode( 7, OUTPUT ); //set pin 7 as digital output
}

void loop()
{
	if( Serial.available() )
	{
		if( Serial.read() == 'T' )
		{
			digitalWrite( 7, HIGH );
			delay( 10 );
			digitalWrite( 7, LOW );
		}
	}
}
