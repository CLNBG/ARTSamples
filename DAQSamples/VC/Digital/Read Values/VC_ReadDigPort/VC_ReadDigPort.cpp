/*********************************************************************
*
* ANSI C Example program:
*    ReadDigPort.c
*
* Example Category:
*    DI
*
* Description:
*    This example demonstrates how to read values from a digital
*    input port.
*
* Instructions for Running:
*    1. Select the digital port on the DAQ device to be read.
*    Note: The Data Read indicator is in hexadecimal.
*
* Steps:
*    1. Create a task.
*    2. Create a Digital Input channel. Use one channel for all
*       lines. In this case, the port itself acts as an individual
*       channel.
*    3. Call the Start function to start the task.
*    4. Read the digital data. This read function reads a single
*       sample of digital data on demand, so no timeout is necessary.
*    5. Call the Clear Task function to clear the Task.
*    6. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminals match the Port I/O
*    Control. In this case wire your digital signals to the first N
*    digital lines on your DAQ Device.
*
*********************************************************************/

#include <stdio.h>
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
	int32		error=0;
	TaskHandle	taskHandle=0;
	uInt32		data;
	char		errBuff[2048]={'\0'};
	int32		read;

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateDIChan(taskHandle,"Dev1/port0","",ArtDAQ_Val_ChanForAllLines));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	/*********************************************/
	// ArtDAQ Read Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_ReadDigitalU32(taskHandle,1,10.0,ArtDAQ_Val_GroupByChannel,&data,1,&read,NULL));
	
	printf("Data acquired: 0x%X\n",(unsigned)data);
	
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