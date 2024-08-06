/*********************************************************************
*
* ANSI C Example program:
*    VC_CntBufContExtClk.c
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to count buffered digital events
*    on a Counter Input Channel. The Initial Count, Count Direction,
*    Edge, and Sample Clock Source are all configurable.
*
*    Edges are counted on the counter's default input terminal (refer
*    to the I/O Connections Overview section below for more
*    information).
*
*    Note: For buffered event counting, an external sample clock is
*          necessary to signal when a sample should be inserted into
*          the buffer. This is set by the Sample Clock Source
*          control.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to count edges on the DAQ device.
*    2. Enter the Initial Count, Count Direction, and measurement
*       Edge to specify how you want the counter to count. Set the
*       Sample Clock Source.
*    Note: An external sample clock must be used. Counters do not
*          have an internal sample clock available. You can use the
*          Gen Dig Pulse Train-Continuous example to generate a pulse
*          train on another counter and connect it to the Sample
*          Clock Source you are using in this example.
*    3. Set the Samples to Read control. This will determine how many
*       samples are read each time.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel to Count Events. The Edge
*       parameter is used to determine if the counter will increment
*       on rising or falling edges.
*    3. Call the ArtDAQ Timing function (Sample Clock) to configure
*       the external sample clock timing parameters such as Sample
*       Mode and Sample Clock Source. The Sample Clock Source
*       determines when a sample will be inserted into the buffer.
*       The Edge parameter can be used to determine when a sample is
*       taken.
*    4. Call the Start function to arm the counter and begin
*       counting. The counter will be preloaded with the Initial
*       Count.
*    5. For continuous measurements, the counter will continually
*       read all available data until the Stop button is pressed on
*       the front panel.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    The counter will count edges on the input terminal of the
*    counter specified in the Physical Channel I/O control.
*
*    The counter will take measurements on valid edges of the
*    external Sample Clock Source which is PFI9 in this example.
*
*    This example uses the default source (or gate) terminal for the
*    counter of your device. 
*
*********************************************************************/

#include <stdio.h>
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
	int32		error=0;
    TaskHandle	taskHandle=0;
    int32       read;
    uInt32      data[1000];
	char		errBuff[2048]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateCICountEdgesChan(taskHandle,"Dev1/ctr0","",ArtDAQ_Val_Rising,0,ArtDAQ_Val_CountUp));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"/Dev1/PFI3",1000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	printf("Continuously reading. Press Ctrl+C to interrupt\n");
	while( 1 ) {
		/*********************************************/
		// ArtDAQ Read Code
		/*********************************************/
        ArtDAQErrChk (ArtDAQ_ReadCounterU32(taskHandle,1000,10.0,data,1000,&read,NULL));

        printf("\rAcquired %d samples",(int)read);
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