/*********************************************************************
*
* ANSI C Example program:
*    VC_PeriodBuffContLargeRange.cpp  
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to continuously measure buffered
*    period using two counters on a Counter Input Channel. The
*    Divisor, Maximum and Minimum Frequency Values, and the Edge
*    Parameter are configurable.
*
*    This example shows how to measure period on the counter's
*    default input terminal (refer to the I/O Connections Overview
*    section below for more information).
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to measure period on the DAQ device.
*    2. Enter the Measurement Divisor which specifies how many
*       periods of the unkown signal are used to calculated the
*       period.
*    Note: It is important to set the Maximum and Minimum Values of
*          your unknown period as accurately as possible so the
*          best internal timebase can be chosen to minimize
*          measurement error. The default values specify a range that
*          can be measured by the counter using the 100MHzTimebase Use
*          the Gen Dig Pulse Train-Continuous example to verify that
*          you are measuring correctly on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel for Period. The Edge
*       parameter is used to determine if the counter will begin
*       measuring on a rising or falling edge. The Divisor specifies
*       how many periods of the unknown signal are used to calculate
*       the frequency. The higher this is, the more accurate your
*       measurement will be, it will also take the measurement
*       longer. It is important to set the Maximum and Minimum Values
*       of your unknown period as accurately as possible so the
*       best internal timebase can be chosen to minimize measurement
*       error. The default values specify a range that can be
*       measured by the counter using the 100MHzTimebase
*    3. Call the ArtDAQ Timing function (Implicit) to configure the
*       Sample Mode and Samples per Channel.

*    Note: For time measurements with counters, the implicit timing
*          function is used because the signal being measured itself
*          determines the sample rate.
*    4. Call the Start function to arm the counter and begin
*       measuring.
*    5. For continuous measurements, the counter will continually
*       read all available data until Stop button is pressed.
*    6. Call the Clear Task function to clear the task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    The counter will measure frequency on the input terminal of the
*    counter specified in the Physical Channel I/O control.
*
*    This example uses the default source (or gate) terminal for the
*    counter of your device. To determine what the default counter
*    pins for your device are or to set a different source (or gate)
*    pin, refer to the Connecting Counter Signals topic in the
*    ArtDAQ Help (search for "Connecting Counter Signals").
*
*********************************************************************/

#include <stdio.h>
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
    int         error=0;
    TaskHandle  taskHandle=0;
    int32       read;
    float64     data[1000];
    char        errBuff[2048]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateCIPeriodChan(taskHandle,"Dev3/ctr0","",0.000000002,4.29496,ArtDAQ_Val_Seconds,ArtDAQ_Val_Rising,ArtDAQ_Val_LargeRng2Ctr,0.001,10,""));
    ArtDAQErrChk (ArtDAQ_CfgImplicitTiming(taskHandle,ArtDAQ_Val_ContSamps,1000));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Continuously reading. Press Ctrl+C to interrupt\n");
    while( 1 ) {
        /*********************************************/
        // DAQmx Read Code
        /*********************************************/
        ArtDAQErrChk (ArtDAQ_ReadCounterF64(taskHandle,1000,10.0,data,1000,&read,0));

        printf("Acquired %d samples\n",(int)read);
        fflush(stdout);
    }
	
Error:
	if( ArtDAQFailed(error) )
		ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
	if( taskHandle!=0 ) {
		/*********************************************/
		// ArtDAQ Stop Code
		/*********************************************/
		ArtDAQ_StopTask(taskHandle);
		ArtDAQ_ClearTask(taskHandle);
	}
	if( ArtDAQFailed(error) )
		printf("ArtDAQ_ Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 0;
}