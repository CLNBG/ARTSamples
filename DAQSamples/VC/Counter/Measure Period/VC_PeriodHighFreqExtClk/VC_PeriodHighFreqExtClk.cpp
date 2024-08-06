/*********************************************************************
*
* ANSI C Example program:
*    VC_PeriodHighFreqExtClk.cpp 
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to continuously measure
*    period on a Counter Input Channel with a sample clock.
*
*    This example shows how to measure period on the counter's
*    default input terminal (refer to the I/O Connections Overview
*    section below for more information).
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to measure period on the DAQ device.
*    2. Enter the Measurement Time which specifies how often a period
*       is calculated by counting the number of edges that have
*       passed in the elapsed time.
*    Note: Use the Gen Dig Pulse Train-Continuous example to verify
*          that you are measuring correctly on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel to measure Period. The Edge
*       parameter is used to determine if the counter will begin
*       measuring on a rising or falling edge. The Measurement Time
*       specifies how often a period is calculated by counting the
*       number of edges that have passed in the elapsed time.
*    Note: The Maximum and Minimum Values are not used when measuring
*          period using the High Frequency 2 Ctr Method.
*    3. Call the ArtDAQ Timing function (Sample Clock) to configure
*       the external sample clock timing parameters such as Sample
*       Mode and Sample Clock Source. The Sample Clock Source
*       determines when a sample will be inserted into the buffer.
*       The Edge parameter can be used to determine when a sample is
*       taken.
*    Note: If the sample clock frequency is not half or less of the
*          frequency measured you may encounter a over run.
*    4. Call the Start function to arm the counter and begin
*       measuring.
*    5. For continuous measurements, the counter will continually
*       read all available data until the stop button is pressed.
*    6. Call the Clear Task function to clear the task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    The counter will measure period on input terminal of the counter
*    specified in the Physical Channel I/O control.
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
	ArtDAQErrChk (ArtDAQ_CreateCIPeriodChan(taskHandle,"Dev1/ctr0","",0.001,0.100,ArtDAQ_Val_Seconds,ArtDAQ_Val_Rising,ArtDAQ_Val_HighFreq2Ctr,0.0001,4,""));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"/Dev1/PFI3",100,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

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