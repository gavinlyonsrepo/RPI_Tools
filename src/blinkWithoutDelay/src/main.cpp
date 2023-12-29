/*!
	@file    main.cpp
	@author   Gavin Lyons
	@brief Example code to replicate famous Arduino sketch blink without delay 
	@note https://github.com/gavinlyonsrepo/RPI_tools
	@details uses bcm2835 library to blink LED , output to console input from command argument
*/
 
// === Library includes ===
#include <time.h>
#include <stdint.h>
#include <stdio.h> // printf
#include <bcm2835.h> // GPIO control 

// === Definitions ===
#define LED_GPIO_PIN 26 /**<GPIO the LED is connected to */

// === Function prototypes ===
static uint64_t NanoCount(void);
static uint64_t MircoCount(void);
static uint64_t MilliCount(void);

bool Setup(void);
void HeartBeat(char);
void EndProgram(void);

// === Main ===
int main(int argc, char **argv)
{

	printf("Start\n");
	// parse the command line arguments user entered.
	printf("Program name %s\n", argv[0]);
	char userChoice = '0';
	
	if (argc == 2)
	{
		// printf("The argument supplied is %c\n",*argv[1]);
		userChoice = *argv[1];
		switch (userChoice)
		{
			case 'n' : printf("Nano second Test \n"); break;
			case 'r' : printf("Micro second Test \n"); break;
			case 'm' : printf("Milli second Test \n"); break;
			default :
				printf("Error 1203 : Unsupported argument \n"); 
				return -1;
			break ;
		}
	}else
	{
		printf("Error 1204 : One argument excepted \n"); 
		return -1;
	}
	
	if (!Setup()) return -1;
	HeartBeat(userChoice);
	EndProgram();
	return 0;
} // End of main

/*!
	@brief handles blinking of LED on timing
	@param userChoice n r or m timing resolution calculation 
	@details n = nanosecond  r = Microsecond m = millisecond 
*/
void HeartBeat(char userChoice)
{
	//SETUP HEARTBEAT TIMER
	uint8_t HeartBeatCount = 1;
	const uint8_t HeartBeatStopLimit = 11;
	uint64_t Interval = 0;
	uint64_t previouscount = 0;
	uint8_t  ledState = LOW; // ledState used to set the LED


	switch (userChoice)
	{
		case 'n' : Interval = 1000000000U; break; //Nano
		case 'r' : Interval = 1000000U; break;  // Micro
		case 'm' : Interval = 1000U; break; // Milli
		default : return; break;
	}

	while(HeartBeatCount <= HeartBeatStopLimit)
	{
		uint64_t currentCount = 0;
		switch (userChoice)
		{
			case 'n' : currentCount = NanoCount(); break;
			case 'r' : currentCount = MircoCount(); break;
			case 'm' : currentCount = MilliCount(); break;
			default : return; break;
		}
		if (currentCount - previouscount >= Interval) // rolls over every interval
		{
			if(ledState == LOW) 
				ledState = HIGH;
			else
				ledState = LOW;
			
			bcm2835_gpio_write(LED_GPIO_PIN , ledState);
			previouscount = currentCount ;
			printf("Heart Beat :: %u\n", HeartBeatCount);
			HeartBeatCount++;
		}
	} // end of while 
}

/*!
	@brief This returns nano-seconds as a 64-bit unsigned number, monotonically increasing, 
	probably since system boot
	@return nanoseconds. The actual resolution looks like microseconds. 
	@details
		8 307 293 886 527 returns normal 
		8 437 000 000 000 ((uint64_t)now.tv_sec * 1000000000U)
		3907199 (uint64_t)now.tv_nsec
*/
static uint64_t NanoCount(void)
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  ((uint64_t)now.tv_sec * 1000000000U) + (uint64_t)now.tv_nsec; 
}

/*!
	@brief This returns micro-seconds as a 64-bit unsigned number, monotonically increasing, 
	probably since system boot.
	@return microseconds.
	@details
		20 102 000 000
		08 437 000 000  ((uint64_t)now.tv_sec * 1000000U)
*/
static uint64_t MircoCount(void)
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  ((uint64_t)now.tv_sec * 1000000U) + ((uint64_t)now.tv_nsec / 1000U); 
}

/*!
	@brief This returns milli-seconds as a 64-bit unsigned number, monotonically increasing, 
	probably since system boot. 
	@return milliseconds.
	@details
		20922069, multiple now.tv_sec * 1000 to convert to millis then add + ((uint64_t)now.tv_nsec/1000000U#
		to get millis resolution
*/
static uint64_t MilliCount(void)
{
	struct timespec now;
	clock_gettime( CLOCK_MONOTONIC, &now );
	return  (uint64_t)now.tv_sec * 1000U + ((uint64_t)now.tv_nsec/1000000U);
}


/*!
  @brief  Closes bcm2835 library.
*/
void EndProgram(void)
{
	bcm2835_gpio_write(LED_GPIO_PIN , LOW);
	bcm2835_close(); // Close the library
	printf("End\n");
}

/*!
  @brief sets up GPIO with bcm2835 library
  @return Will return false if fails to init Bcm2835 library 
*/
bool Setup(void)
{
	if (!bcm2835_init())
	{
		printf("Error 1201 :: bcm2835_init failed. Are you running as root??\n");
		return false;
	}
	printf("bcm2835 :: library Version Number :: %u\n", bcm2835_version());
	bcm2835_gpio_fsel(LED_GPIO_PIN , BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(LED_GPIO_PIN , LOW);
	
	return true;
}

