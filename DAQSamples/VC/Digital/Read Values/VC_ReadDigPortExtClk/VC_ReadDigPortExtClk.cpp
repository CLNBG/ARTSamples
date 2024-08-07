/*********************************************************************
*
* ANSI C Example program:
*    VC_ReadDigPortExtClk.cpp 
*
* Example Category:
*    DI
*
* Description:
*    This example demonstrates how to input a finite digital pattern
*    using an external clock.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Select the Clock Source for the acquisition.
*    3. Select how many Samples to Acquire.
*    4. Set the approximate Sample Clock Rate of the external clock.
*       This allows the internal characteristics of the acquisition
*       to be as efficient as possible.
*
* Steps:
*    1. Create a task.
*    2. Create a Digital Input channel.
*    3. Define the parameters for an External Clock Source.
*       Additionally, set the sample mode to be finite.
*    4. Call the Start function to begin the acquisition.
*    5. Read the digital pattern. Set a timeout so an error is
*       returned if the samples are not returned in the specified
*       time limit.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control. Also, make sure your external clock
*    terminal matches the Clock Source Control. For further
*    connection information, refer to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
    int32		error=0;
    TaskHandle	taskHandle=0;
    uInt32		data[1000];
    int32		sampsRead;
    char		errBuff[2048]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateDIChan(taskHandle,"Dev1/port0","",ArtDAQ_Val_ChanForAllLines));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"/Dev1/PFI0",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_FiniteSamps,1000));

    /*********************************************/
    // DAQmx Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    /*********************************************/
    // DAQmx Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadDigitalU32(taskHandle,-1,10.0,ArtDAQ_Val_GroupByChannel,data,1000,&sampsRead,NULL));

    printf("Acquired %d samples\n",(int)sampsRead);
	
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