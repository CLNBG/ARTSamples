/*********************************************************************
*
* ANSI C Example program:
*    VC_Volt_Update.c
*
* Example Category:
*    AO
*
* Description:
*    This example demonstrates how to output a single Voltage Update
*    (Sample) to an Analog Output Channel.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is output on the DAQ device.
*    2. Enter the Minimum and Maximum Voltage Ranges.
*    Note: Use the Acq One Sample example to verify you are
*          generating the correct output on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Output Voltage Channel.
*    3. Use the Write function to Output 1 Sample to 1 Channel on the
*       Data Acquisition Card.
*    4. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include "DAQ/Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
	int32       error = 0;
	TaskHandle  taskHandle = 0;
	char        errBuff[2048] = { '\0' };
	float64     data[1] = { 1.0 };

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_CreateTask("", &taskHandle));
	ArtDAQErrChk(ArtDAQ_CreateAOVoltageChan(taskHandle, "Stere/ao0", "", -10.0, 10.0, ArtDAQ_Val_Volts, ""));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_StartTask(taskHandle));

	/*********************************************/
	// ArtDAQ Write Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_WriteAnalogF64(taskHandle, 1, 0, 10.0, ArtDAQ_Val_GroupByChannel, data, NULL, NULL));

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
	auto x = getchar();

	return 0;
}
