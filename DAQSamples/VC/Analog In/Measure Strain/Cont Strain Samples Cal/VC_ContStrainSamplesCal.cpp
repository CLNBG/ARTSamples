/*********************************************************************
*
* ANSI C Example program:
*    ContStrainSampleswCal.c
*
* Example Category:
*    AI
*
* Description:
*    This example acquires strain measurements on input channels.
*
* Instructions for Running:
*    1. Enter the list of physical channels, and set the attributes
*       of the strain configuration connected to all the channels.
*       The 'Maximum Value' and 'Minimum Value' inputs specify the
*       range, in strains, that you expect of your measurements.
*    2. Make sure all strain gages are in their relaxed state.
*    3. You may turn on the 'Do Strain Null?' checkbox to
*       automatically null out your gage by performing a hardware
*       nulling operation (if supported by the hardware) followed by
*       a software nulling operation. (NOTE: The software nulling
*       operation will cause a loss in dynamic range while a hardware
*       nulling operation will not cause any loss in the dynamic
*       range).
*    4. You can turn on the 'Do Shunt Cal?' checkbox to perform a
*       shunt calibration (gain adjust calculation) on your gage (the
*       location and value of the shunt resistor are needed).
*    5. Run the example and do not start straining the gages until
*       data starts being plotted.
*
* Steps:
*    1. Create a task.
*    2. Create a Strain input channel.
*    3. Set timing parameters. Note that sample mode set to
*       Continuous Samples.
*    4. Set filter parameters.
*    5. If nulling is desired, call
*       ArtDAQ_PerformBridgeOffsetNullingCal function to perform both
*       hardware nulling (if supported) and software nulling.
*    6. If shunt calibration is desired (should only be done if you
*       have shunt resistors connected), perform shunt calibration by
*       calling function ArtDAQ_PerformStrainShuntCal, you can measure 
*       the actual excitation on your bridge for more accurate measurements).
*    7. Call the Start function to start the acquisition.
*    8. Read the data in the EveryNCallback function until the user
*       hits the stop button or an error occurs.
*    9. Call the Clear Task function to clear the Task.
*    10. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control.
*
*********************************************************************/

#include <stdio.h>
#include <Art_DAQ.h>

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
	int32       error=0;
	TaskHandle  taskHandle=0;
	char        errBuff[2048]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateAIStrainGageChan(taskHandle,"","",-0.001,0.001,ArtDAQ_Val_Strain,ArtDAQ_Val_FullBridgeI,ArtDAQ_Val_Internal,2.50,2.0,0.0,120.0,0.285,0.0,""));
	ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",10.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

	ArtDAQErrChk (ArtDAQ_PerformBridgeOffsetNullingCal(taskHandle,""));
	ArtDAQErrChk (ArtDAQ_PerformStrainShuntCal(taskHandle,"",100000,0,0));

	ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(taskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
	ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	printf("Acquiring samples continuously. Press Enter to interrupt\n");
	getchar();

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

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	int32       error=0;
	char        errBuff[2048]={'\0'};
	static int  totalRead=0;
	int32       read=0;
	float64     data[1000];

	/*********************************************/
	// ArtDAQ Read Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_ReadAnalogF64(taskHandle,-1,10.0,ArtDAQ_Val_GroupByScanNumber,data,1000,&read,NULL));
	if( read>0 ) {
		printf("Acquired %d samples. Total %d\r",(int)read,(int)(totalRead+=read));
		fflush(stdout);
	}

Error:
	if( ArtDAQFailed(error) ) {
		ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// ArtDAQ Stop Code
		/*********************************************/
		ArtDAQ_StopTask(taskHandle);
		ArtDAQ_ClearTask(taskHandle);
		printf("ArtDAQ_ Error: %s\n",errBuff);
	}
	return 0;
}

int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	int32   error=0;
	char    errBuff[2048]={'\0'};

	// Check to see if an error stopped the task.
	ArtDAQErrChk (status);

Error:
	if( ArtDAQFailed(error) ) {
		ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
		ArtDAQ_ClearTask(taskHandle);
		printf("ArtDAQ_ Error: %s\n",errBuff);
	}
	return 0;
}
