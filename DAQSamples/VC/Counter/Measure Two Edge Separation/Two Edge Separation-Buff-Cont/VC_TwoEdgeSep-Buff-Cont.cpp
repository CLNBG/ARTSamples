/*********************************************************************
*
* ANSI C Example program:
*    TwoEdgeSep-Buff-Cont.c
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to perform a continuous number of
*    two edge separation measurements on a Counter Input Channel. The
*    First Edge, Second Edge, Minimum Value, Maximum Value, and
*    Samples to Read are all configurable.
*
*    This example shows how to perform a two edge separation
*    measurement on the counter's default input terminals (refer to
*    the I/O Connections Overview section below for more
*    information), but could easily be expanded to measure two edge
*    separation on any PFI, RTSI, or internal signal.
*
*    Refer to your device documentation to see if your device
*    supports two edge separation measurements.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       on the DAQ device you want to perform a two edge separation
*       measurement on.
*    2. Enter the First Edge and Second Edge corresponding to the two
*       edges you want the counter to measure. Enter the Maximum and
*       Minimum Value to specify the range of your unknown two edge
*       separation. Additionally, you can change the First and Second
*       Edge Input Terminals using the appropriate channel
*       attributes.
*    Note: It is important to set the Maximum and Minimum Values of
*          your unknown two edge separation as accurately as possible
*          so the best internal timebase can be chosen to minimize
*          measurement error. The default values specify a range that
*          can be measured by the counter using the 20MHzTimebase.
*    3. Set the Samples to Read control. This will determine how many
*       samples are read each time.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel to perform a Two Edge
*       Separation measurement. The First and Second Edge parameters
*       are used to specify the rising or falling edge of one digital
*       signal and the rising or falling edge of another digital
*       signal. It is important to set the Maximum and Minimum Values
*       of your unknown two edge separation as accurately as possible
*       so the best internal timebase can be chosen to minimize
*       measurement error. The default values specify a range that
*       can be measured by the counter using the 20MHzTimebase.
*    3. Call the ArtDAQ Timing function (Implicit) to configure the
*       Sample Mode.

*    Note: For time measurements with counters, the implicit timing
*          function is used because the signals being measured
*          determine the sample rate.
*    4. Call the Start function to arm the counter and begin
*       measuring.
*    5. For continuous measurements, the counter will continually
*       read data until the Stop button is pressed.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    The counter will perform a two edge separation measurement on
*    the First and Second Edge Input Terminals of the counter
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
#include <Art_DAQ.h>

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
	ArtDAQErrChk (ArtDAQ_CreateCITwoEdgeSepChan (taskHandle,"Dev1/ctr0","",0.000000100,0.838900000,ArtDAQ_Val_Seconds,ArtDAQ_Val_Rising,ArtDAQ_Val_Falling,""));
	ArtDAQErrChk (ArtDAQ_CfgImplicitTiming(taskHandle,ArtDAQ_Val_ContSamps,1000));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	printf("Continuously reading. Press Ctrl+C to interrupt\n");
	while( 1 ) {
		/*********************************************/
		// ArtDAQ Read Code
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
		printf("ArtDAQ Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 0;
}
