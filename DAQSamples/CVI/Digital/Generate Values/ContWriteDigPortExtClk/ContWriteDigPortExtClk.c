/*********************************************************************
*
* ANSI C Example program:
*    VC_ContWriteDigPortExtClk.cpp 
*
* Description:
*    This example demonstrates how to output a continuous digital
*    pattern using an external clock.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is output on the DAQ device.
*    2. Select the Clock Source for the generation.
*    3. Specify the Rate of the output digital pattern.
*    4. Enter the digital pattern data.
*
* Steps:
*    1. Create a task.
*    2. Create an Digital Output Channel.
*    3. Call the ArtDAQ_CfgSampClkTiming function which sets the sample
*       clock rate. Additionally, set the sample mode to continuous.
*    4. Write the data to the output buffer.
*    5. Call the Start function to start the task.
*    6. Wait until the user presses the Stop button.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. Also, make sure your external clock
*    terminal matches the Clock Source Control. For further
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

int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");
	
    int32       error=0;
    TaskHandle  taskHandle=0;
    uInt32      data[8]={1,2,4,8,16,32,64,128};
    char        errBuff[2048]={'\0'};
	char        strChannelName[100]={'\0'};
	char        strExternalClock[100]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	printf("Please enter channal number,such as Dev1/port0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
	printf("Please enter external clock source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strExternalClock);
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateDOChan(taskHandle,strChannelName,"",ArtDAQ_Val_ChanForAllLines));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,strExternalClock,1000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ Write Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_WriteDigitalU32(taskHandle,8,0,10.0,ArtDAQ_Val_GroupByChannel,data,NULL,NULL));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Generating digital output continuously. Press Enter to interrupt\n");
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
    printf("Task Done\n");

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        ArtDAQ_ClearTask(taskHandle);
        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}
