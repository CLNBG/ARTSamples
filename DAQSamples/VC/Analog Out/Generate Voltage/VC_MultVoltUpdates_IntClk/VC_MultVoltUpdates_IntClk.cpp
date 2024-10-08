/*********************************************************************
*
* ANSI C Example program:
*    VC_MultVoltUpdates_IntClk.c
*
* Example Category:
*    AO
*
* Description:
*    This example demonstrates how to output a finite number of
*    voltage samples to an Analog Output Channel using an internal
*    sample clock.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is output on the DAQ device.
*    2. Enter the Minimum and Maximum Voltage Ranges.
*    3. There is no need to set the Sample Clock rate because this
*       example uses the Waveform Information to determine it
*       automatically.
*    4. Select the desired waveform type.
*    5. The rest of the parameters in the Waveform Information
*       section will affect the way the waveform is created, before
*       it's sent to the analog output of the board. Select the
*       number of samples per cycle and the total number of cycles to
*       be used as waveform data.
*    6. Select the desired waveform frequency; this will determine
*       the update rate of the operation.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Output Voltage Channel.
*    3. Setup the Timing for the Measurement. In this example we use
*       the internal DAQ Device's clock to produce a finite number of
*       samples.
*    4. Use the Write function to Generate Multiple Samples to 1
*       Channel on the Data Acquisition Card. The auto start
*       parameter is set to False, so the Start function must
*       explicitly be called to begin the Voltage Generation.
*    5. Call the Start function.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include "DAQ/Art_DAQ.h"
#include <vector>

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
	int         error = 0;
	TaskHandle  taskHandle = 0;
	float64     data[25600 * 2] = {0};
	char        errBuff[2048] = { '\0' };
	int32   	written;

	std::vector<double> dataVec;

	for (int i = 0; i < 4000; i++)
		data[i] = 5.0 * (double)i / 4000.0;

	for (int i = 0; i < 15000; i++)
		dataVec.push_back(-7.5  + i * 0.001 );
	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_CreateTask("", &taskHandle));
	ArtDAQErrChk(ArtDAQ_CreateAOVoltageChan(taskHandle, 
		"Dev1/ao0", 
		"", 
		-10.0, 
		10.0, 
		ArtDAQ_Val_Volts, 
		""
	));
	ArtDAQErrChk(ArtDAQ_CfgSampClkTiming(taskHandle, 
		"", 
		25600, 
		ArtDAQ_Val_Rising, 
		ArtDAQ_Val_FiniteSamps, 
		15000
	));

	while (1)
	{

	/*********************************************/
	// ArtDAQ Write Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_WriteAnalogF64(taskHandle, 
		15000, 
		0, 
		10.0, 
		ArtDAQ_Val_GroupByChannel, 
		dataVec.data(), 
		&written, 
		NULL));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_StartTask(taskHandle));

	/*********************************************/
	// ArtDAQ_ Wait Code
	/*********************************************/
	ArtDAQErrChk(ArtDAQ_WaitUntilTaskDone(taskHandle, 10.0));
		ArtDAQ_StopTask(taskHandle);
	}

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
