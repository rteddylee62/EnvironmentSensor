/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>
//#include <SPI.h>
/*End of auto generated code by Atmel studio */

//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

#include <SD.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include "Main.h"

// debug define for controlling serial communications
#define DEBUG 1

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
#define SDCARD_CS	4
#define SS10		10

#define DHTPIN      2         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht( DHTPIN, DHTTYPE );

#define TIME_DELAY	900000	// 15 min
//#define TIME_DELAY	30000	// 30 sec

// file object
File dataFile;

void setup() 
{
	// initialize serial port
	Serial.begin( 9600 );
	int res = InitSdCard();
#if DEBUG
	if ( res )
		Serial.println( "card initialized." );
	else
	{
		Serial.println( "card initialization failed." );
		while ( 1 ) ;
	}
#endif

	// Open up the file we're going to log to!
	dataFile = openFile( "datalog.txt" );
	// Initialize DHT sensor
	dht.begin();
#if DEBUG
	Serial.println( "DHTxx Unified Sensor Example" );
#endif
	// Print temperature sensor details.
	sensor_t sensor;
	dht.temperature().getSensor( &sensor );

#if DEBUG
	Serial.println( "------------------------------------" );
	Serial.println( "Temperature" );
	Serial.print  ( "Sensor:       " ); Serial.println( sensor.name );
	Serial.print  ( "Driver Ver:   " ); Serial.println( sensor.version );
	Serial.print  ( "Unique ID:    " ); Serial.println( sensor.sensor_id );
	Serial.print  ( "Max Value:    " ); Serial.print( sensor.max_value ); Serial.println( " *C" );
	Serial.print  ( "Min Value:    " ); Serial.print( sensor.min_value ); Serial.println( " *C" );
	Serial.print  ( "Resolution:   " ); Serial.print( sensor.resolution ); Serial.println( " *C" );
	Serial.println( "------------------------------------" );
#endif

	// Print humidity sensor details.
	dht.humidity().getSensor( &sensor );

#if DEBUG
	Serial.println( "------------------------------------" );
	Serial.println( "Humidity" );
	Serial.print  ( "Sensor:       " ); Serial.println( sensor.name );
	Serial.print  ( "Driver Ver:   " ); Serial.println( sensor.version );
	Serial.print  ( "Unique ID:    " ); Serial.println( sensor.sensor_id );
	Serial.print  ( "Max Value:    " ); Serial.print( sensor.max_value ); Serial.println( "%" );
	Serial.print  ( "Min Value:    " ); Serial.print( sensor.min_value ); Serial.println( "%" );
	Serial.print  ( "Resolution:   " ); Serial.print( sensor.resolution ); Serial.println( "%" );
	Serial.println( "------------------------------------" );
#endif

}

void loop() 
{
	// make a string for assembling the data to log:
	String dataString = "";

	// Get temperature event and print its value.
	sensors_event_t event;
	dht.temperature().getEvent( &event );
	
	if( isnan( event.timestamp ) )
	{
#if DEBUG
		Serial.println( "Error: no timestamp" );
#endif
		dataString += "-1,";
	}
	else
	{
#if DEBUG
		Serial.print( "Timestamp: " );
		Serial.print( event.timestamp );
		Serial.println( " ms" );
#endif

		dataString += String( event.timestamp / 1000 );
		dataString += ",";
	}
	
	if (isnan(event.temperature)) 
	{
#if DEBUG
		Serial.println( "Error reading temperature!" );
#endif
		dataString += "-1,";
	}
	else 
	{
#if DEBUG
		Serial.print( "Temperature: " );
		Serial.print( event.temperature );
		Serial.println( " *C" );
#endif
		
		dataString += String( event.temperature );
		dataString += ",";
	}
	
	// Get humidity event and print its value.
	dht.humidity().getEvent( &event );
	if( isnan( event.relative_humidity ) ) 
	{
#if DEBUG
		Serial.println( "Error reading humidity!" );
#endif
		dataString += "-1";
	}
	else 
	{
#if DEBUG
		Serial.print( "Humidity: " );
		Serial.print( event.relative_humidity );
		Serial.println( "%" );
#endif

		dataString += String( event.relative_humidity );
	}

#if DEBUG
	Serial.println( dataString );
	Serial.println();
#endif
	
	dataFile.println( dataString );
	dataFile.flush();

	// DHT11 sampling rate
	delay( TIME_DELAY );
}

/** 
 * initialize the SD card
 */
int InitSdCard()
{
#if DEBUG
	Serial.print( "Initializing SD card..." );
#endif

	// make sure that the default chip select pin is set to
	// output, even if you don't use it:
	pinMode( SS10, OUTPUT );
	//disconnect the W5200
	digitalWrite( SS10, HIGH );
	
	pinMode( SDCARD_CS, OUTPUT );
	
	// see if the card is present and can be initialized:
	int res = SD.begin( SDCARD_CS );
		
	return res;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
File openFile( String filename )
{
	Serial.println(filename);
	File file = SD.open( filename, FILE_WRITE );
	if( !file ) {
#ifdef DEBUG
		Serial.println( "error opening datalog.txt" );
#endif
		// Wait forever since we cant write data
		while( 1 ) ;
	}

	return file;
}