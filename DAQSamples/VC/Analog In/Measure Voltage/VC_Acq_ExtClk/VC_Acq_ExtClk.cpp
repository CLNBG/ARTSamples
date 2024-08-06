/*********************************************************************
*
* ANSI C Example program:
*    Acq-ExtClk.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to Acquire a Finite amount of data
*    using an external clock.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the Minimum and Maximum Voltage Ranges.
*    Note: For better accuracy try to match the Input Ranges to the
*          expected voltage level of the measured signal.
*    3. Select the Source of the Clock for the acquisition.
*    4. Select how many Samples to Acquire on Each Channel.
*    5. Set the approximate Rate of the external clock. This allows
*       the internal characteristics of the acquisition to be as
*       efficient as possible.
*    Note: The Rate should be AT LEAST twice as fast as the maximum
*          frequency component of the signal being acquired.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Input Voltage channel.
*    3. Define the parameters for an External Clock Source.
*       Additionally, define the sample mode to be finite.
*    4. Call the Start function to begin the acquisition.
*    5. Use the Read function to Measure Multiple Samples from N
*       Channels on the Data Acquisition Card. Set a timeout so an
*       error is returned if the samples are not returned in the
*       specified time limit.
*    6. Call the Clear Task function to clear the task.
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
	int32       error=0;
	TaskHandle  taskHandle=0;
	int32       read;
	float64     data[1000];
	char        errBuff[2048]={'\0'};
	
	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(taskHandle,"Dev1/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
	ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"/Dev1/PFI2",1000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_FiniteSamps,1000));
	
	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));
	
	/*********************************************/
	// ArtDAQ Read Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_ReadAnalogF64(taskHandle,-1,10.0,ArtDAQ_Val_GroupByChannel,data,1000,&read,NULL));
	
	if( read>0 )
		printf("Acquired %d samples\n",(int)read);
	
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