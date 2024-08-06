/*********************************************************************
*
* ANSI C Example program:
*    VC_ContGen_ExtClk_DigStart.c
*
* Example Category:
*    AO
*
* Description:
*    This example demonstrates how to continuously output a waveform
*    using an external sample clock and a digital start trigger.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is output on the DAQ device.
*    2. Enter the Minimum and Maximum Voltage range.
*    3. Select the Sample Clock source.
*    4. Select the Digital Trigger source.
*    5. Specify the desired Trigger Edge.
*    6. Select the desired waveform type.
*    7. The rest of the parameters in the Waveform Information
*       section will affect the way the waveform is created, before
*       it's sent to the analog output of the board. Select the
*       number of samples per cycle and the total number of cycles to
*       be used as waveform data.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Output Voltage channel.
*    3. Define the Sample Clock source. Additionally, define the
*       sample mode to be continuous.
*    4. Define the Triggering parameters: Source and Edge.
*    5. Write the waveform to the output buffer.
*    6. Call the Start function.
*    7. Wait until the user presses the Stop button.
*    8. Call the Clear Task function to clear the Task.
*    9. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. Also, make sure your digital trigger
*    terminal matches the Trigger Source Control. For further
*    connection information, refer to your hardware reference manual.
*
*********************************************************************/

#include "conio.h"
#include "windows.h"
#include "Art_DAQ.h"
#include <ansi_c.h>

static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void);

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

#define PI	3.1415926535

int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");
	
	int32       error=0;
	TaskHandle  taskHandle=0;
	float64     data[1000]={0};
	char        errBuff[2048]={'\0'};
	int         i=0;
	char        strChannelName[100]={'\0'};
	char        strExternalClock[100]={'\0'};
	char        strStartTriggerSource[100]={'\0'};
	
	for(;i<1000;i++)
		data[i] = 9.95*sin((double)i*2.0*PI/1000.0);
	
	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	printf("Please enter channal number,such as Dev1/ao0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter external clock source name,such as /Dev1/PFI2,Dev1 is the name identified in the DMC:");
    scanf("%s", strExternalClock);
	printf("Please enter digital start trigger source name,such as /Dev1/PFI1,Dev1 is the name identified in the DMC:");
    scanf("%s", strStartTriggerSource);
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateAOVoltageChan(taskHandle,strChannelName,"",-10.0,10.0,ArtDAQ_Val_Volts,""));
	ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,strExternalClock,1000,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));
	ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(taskHandle,strStartTriggerSource,ArtDAQ_Val_Rising));

	ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));
	
	/*********************************************/
	// ArtDAQ Write Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_WriteAnalogF64(taskHandle,1000,0,10.0,ArtDAQ_Val_GroupByChannel,data,NULL,NULL));
	
	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	printf("Generating voltage continuously. Press Enter to interrupt\n");
	getch();
	
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
	getch();
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
        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}
