/*********************************************************************
*
* ANSI C Example program:
*    Acq-IntClk-DigStartRef.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire a finite amount of data
*    using an internal clock and a digital start and reference
*    trigger.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltage ranges.
*    Note: For better accuracy try to match the Input Ranges to the
*          expected voltage level of the measured signal.
*    3. Select the number of Samples per Channel to acquire.
*    4. Set the Rate of the acquisition.
*    Note: The Rate should be AT LEAST twice as fast as the maximum
*          frequency component of the signal being acquired.
*    5. Set the Source of the Start and Reference trigger as well as
*       the polarity of the Start and Reference Edges. Additionally,
*       set the number of Pre-Trigger Samples to be acquired.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Define the parameters for an Internal Clock Source.
*       Additionally, define the sample mode to be Finite.
*    4. Define the parameters for a Digital Edge Start Trigger and a
*       Digital Edge Reference Trigger.
*    5. Call the Start function to start the acquisition.
*    6. Use the Read function to retrieve the waveform. Set a timeout
*       so an error is returned if the samples are not returned in
*       the specified time limit.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control. Also, make sure your digital trigger
*    terminals match the Trigger Source Controls. For further
*    connection information, refer to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include "DAQ/Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	int32       read;
	float64     data[1000];
	char        errBuff[2048] = { '\0' };

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_CreateTask("", &taskHandle));
	ArtDAQErrChk(ArtDAQ_CreateAIVoltageChan(taskHandle, "Dev1/ai0", "", ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, NULL));
	ArtDAQErrChk(ArtDAQ_CfgSampClkTiming(taskHandle, "", 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000));
	ArtDAQErrChk(ArtDAQ_CfgDigEdgeStartTrig(taskHandle, "/Dev1/PFI0", ArtDAQ_Val_Rising));
	ArtDAQErrChk(ArtDAQ_CfgDigEdgeRefTrig(taskHandle, "/Dev1/PFI0", ArtDAQ_Val_Rising, 100));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_StartTask(taskHandle));

	/*********************************************/
	// ArtDAQ Read Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_ReadAnalogF64(taskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, data, 1000, &read, NULL));

	printf("Acquired %d samples\n", (int)read);

Error:
	if (ArtDAQFailed(error))
		ArtDAQ_GetExtendedErrorInfo(errBuff, 2048);
	if (taskHandle != 0) {
		/*********************************************/
		// ArtDAQ Stop Code
		/*********************************************/
		ArtDAQ_StopTask(taskHandle);
		ArtDAQ_ClearTask(taskHandle);
	}
	if (ArtDAQFailed(error))
		printf("ArtDAQ_ Error: %s\n", errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	return 0;
}
