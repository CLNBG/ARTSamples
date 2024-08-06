/*********************************************************************
*
* ANSI C Example program:
*    VC_ReadDigChanIntClkDigStart.cpp 
*
* Example Category:
*    DI
*
* Description:
*    This example demonstrates how to acquire a finite amount of data
*    using a digital start trigger.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Select how many Samples to Acquire on Each Channel.
*    3. Set the Rate of the Acquisition.
*    4. Select the Source and Edge of the Digital Start Trigger
*       for the acquisition.
*
* Steps:
*    1. Create a task.
*    2. Create a digital input channel.
*    3. Define the parameters for an Internal Clock Source.
*       Additionally, define the sample mode to be Finite.
*    4. Define the parameters for a Digital Edge Start Trigger.
*    5. Call the Start function to begin the acquisition.
*    6. Use the Read function to retrieve the waveform. Set a timeout
*       so an error is returned if the samples are not returned in
*       the specified time limit.
*    7. Call the Clear Task function to clear the task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
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

int main(void)
{
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");
	
    int32       error=0;
    TaskHandle  taskHandle=0;
    int32       numRead;
    uInt8       data[8000];
    char        errBuff[2048]={'\0'};
	char        strChannelName[100]={'\0'};
	char        strStartTrigger[100]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	printf("Please enter channal number,such as Dev1/port0/line0:7,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter digital start trigger source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strStartTrigger);
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateDIChan(taskHandle,strChannelName,"",ArtDAQ_Val_ChanPerLine));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_FiniteSamps,1000));
    ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(taskHandle,strStartTrigger,ArtDAQ_Val_Rising));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	/*********************************************/
	// ArtDAQ Read Code
	/*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadDigitalLines(taskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,data,8000,&numRead,NULL,NULL));

    printf("Acquired %d samples\n",(int)numRead);
	
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
