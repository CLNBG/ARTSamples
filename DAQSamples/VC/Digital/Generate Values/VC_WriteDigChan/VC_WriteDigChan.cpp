/*********************************************************************
*
* ANSI C Example program:
*    WriteDigChan.c
*
* Example Category:
*    DO
*
* Description:
*    This example demonstrates how to write values to a digital
*    output channel.
*
* Instructions for Running:
*    1. Select the digital lines on the DAQ device to be written.
*    2. Select a value to write.
*    Note: The array is sized for 8 lines, if using a different
*          amount of lines, change the number of elements in the
*          array to equal the number of lines chosen.
*
* Steps:
*    1. Create a task.
*    2. Create a Digital Output channel. Use one channel for all
*       lines.
*    3. Call the Start function to start the task.
*    4. Write the digital Boolean array data. This write function
*       writes a single sample of digital data on demand, so no
*       timeout is necessary.
*    5. Call the Clear Task function to clear the Task.
*    6. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminals match the Lines I/O
*    Control. In this case wire the item to receive the signal to the
*    first eight digital lines on your DAQ Device.
*
*********************************************************************/

#include <stdio.h>
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
	int32       error=0;
	TaskHandle  taskHandle=0;
	uInt8       data[8]={1,0,0,1,1,1,1,1};
	char        errBuff[2048]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateDOChan(taskHandle,"Dev1/port0/line0:7","",ArtDAQ_Val_ChanForAllLines));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	/*********************************************/
	// ArtDAQ Write Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_WriteDigitalLines(taskHandle,1,1,10.0,ArtDAQ_Val_GroupByChannel,data,NULL,NULL));
	
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