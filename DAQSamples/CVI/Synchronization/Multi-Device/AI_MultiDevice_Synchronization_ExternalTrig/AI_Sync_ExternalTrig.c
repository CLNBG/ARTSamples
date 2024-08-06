/*********************************************************************
*
* ANSI C Example program:
*    VC_AI_Sync_ExternalTrig.c
*
* Example Category:
*    Sync
*
* Description:
*    This example demonstrates how to acquire a continuous amount of
*    data using the DAQ device's internal clock. It also shows how to
*    synchronize two devices, to simultaneously acquire the data.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltage range.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the rate of the acquisition.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*    4. Set the number of samples to acquire per channel.
*    5. Choose which type of devices you are trying to synchronize.
*       This will select the correct synchronization method to use.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel for both the Master
*       and Slave devices.
*    3. Set timing parameters. Note that sample mode is set to
*       Continuous Samples. In this example, the Rate and the Samples 
*		per Channel is set the same for both devices. 
*    4. Set the parameters for a digital edge start trigger.
*    5. Call the Start function to start the devices.
*    6. Read all of the data continuously. The 'Samples per Channel'
*       control will specify how many samples per channel are read
*       each time. If either device reports an error or the user
*       presses the 'Stop' button, the acquisition will stop.
*    7. Call the Clear Task function to clear the task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/

#include "conio.h"
#include "windows.h"
#include "Art_DAQ.h"
#include <ansi_c.h>

static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void);

static TaskHandle masterTaskHandle=0,slaveTaskHandle=0;

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");
	
	int32       error=0;
    char        errBuff[2048]={'\0'};
	char        strMasterChannelName[100]={'\0'};
	char        strSlaveChannelName[100]={'\0'};
	char        strMasterStartTrigger[100]={'\0'};
	char        strSlaveStartTrigger[100]={'\0'};
	
    /*********************************************/
    // ArtDAQ_ Configure Code
    /*********************************************/
	printf("Please enter master card channal number,such as Dev1/ai0,Dev1 is the name identified in the DMC:");
    scanf("%s", strMasterChannelName);
	printf("Please enter slave card channal number,such as Dev2/ai0,Dev2 is the name identified in the DMC:");
    scanf("%s", strSlaveChannelName);
    ArtDAQErrChk (ArtDAQ_CreateTask("",&masterTaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(masterTaskHandle,strMasterChannelName,"",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(masterTaskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,10000));
    ArtDAQErrChk (ArtDAQ_CreateTask("",&slaveTaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(slaveTaskHandle,strSlaveChannelName,"",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(slaveTaskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,10000));

	printf("Please enter master card digital start trigger source name,such as /Dev1/PFI1,Dev1 is the name identified in the DMC:");
    scanf("%s", strMasterStartTrigger);
	printf("Please enter slave card digital start trigger source name,such as Dev2/PFI1,Dev2 is the name identified in the DMC:");
    scanf("%s", strSlaveStartTrigger);
	ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(masterTaskHandle,strMasterStartTrigger,ArtDAQ_Val_Rising));
	ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(slaveTaskHandle,strSlaveStartTrigger,ArtDAQ_Val_Rising));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(masterTaskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(masterTaskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ_ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(slaveTaskHandle));
    ArtDAQErrChk (ArtDAQ_StartTask(masterTaskHandle));
	
    printf("Acquiring samples continuously. Press Enter to interrupt\n");
	printf("\nRead:\tMaster\tSlave\tTotal:\tMaster\tSlave\n");
	getch();
	
Error:
    if( ArtDAQFailed(error) )
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
    /*********************************************/
	// ArtDAQ_ Stop Code
    /*********************************************/
	if( masterTaskHandle ) {
		ArtDAQ_StopTask(masterTaskHandle);
		ArtDAQ_ClearTask(masterTaskHandle);
		masterTaskHandle = 0;
    }
    if( slaveTaskHandle ) {
        ArtDAQ_StopTask(slaveTaskHandle);
        ArtDAQ_ClearTask(slaveTaskHandle);
        slaveTaskHandle = 0;
    }
        
    if( ArtDAQFailed(error))
        printf("ArtDAQ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getch();
    return 0;
}

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    int32           error=0;
	char            errBuff[2048]={'\0'};
	static int32    masterTotal=0,slaveTotal=0;
	int32           masterRead,slaveRead;
	float64         masterData[1000],slaveData[1000];

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_ReadAnalogF64(masterTaskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,masterData,1000,&masterRead,NULL));
	ArtDAQErrChk (ArtDAQ_ReadAnalogF64(slaveTaskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,slaveData,1000,&slaveRead,NULL));
	
	if( masterRead>0 )
		masterTotal += masterRead;
	if( slaveRead>0 )
		slaveTotal += slaveRead;
	printf("\t%d\t%d\t\t%d\t%d\r",(int)masterRead,(int)slaveRead,(int)masterTotal,(int)slaveTotal);
	fflush(stdout);

Error:
	if( ArtDAQFailed(error) ) {
		ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		if( masterTaskHandle ) {
			ArtDAQ_StopTask(masterTaskHandle);
			ArtDAQ_ClearTask(masterTaskHandle);
		}
		if( slaveTaskHandle ) {
			ArtDAQ_StopTask(slaveTaskHandle);
			ArtDAQ_ClearTask(slaveTaskHandle);
		}
		printf("ArtDAQ Error: %s\n",errBuff);
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
		if( slaveTaskHandle ) {
			ArtDAQ_StopTask(slaveTaskHandle);
			ArtDAQ_ClearTask(slaveTaskHandle);
		}
        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}
