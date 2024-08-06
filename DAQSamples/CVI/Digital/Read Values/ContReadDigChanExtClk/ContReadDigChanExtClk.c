/*********************************************************************
*
* ANSI C Example program:
*    VC_ContReadDigChanExtClk.cpp 
*
* Example Category:
*    DI
*
* Description:
*    This example demonstrates how to input a continuous digital
*    waveform using an external clock.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Select the Clock Source for the acquistion.
*    3. Set the approximate Rate of the external clock. This allows
*       the internal characteristics of the acquisition to be as
*       efficient as possible. Also set the Samples to Read control.
*       This will determine how many samples are read each time. This
*       also determines how many points are plotted on the graph each
*       iteration.
*
* Steps:
*    1. Create a task.
*    2. Create a Digital Input channel.
*    3. Define the parameters for an External Clock Source.
*       Additionally, set the sample mode to be continuous.
*    4. Call the Start function to start the acquisition.
*    5. Read the waveform data continuously until the user hits the
*       stop button or an error occurs.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control. Also, make sure your external clock
*    terminal matches the Physical Channel I/O Control. For further
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
	
    int32		error=0;
    TaskHandle	taskHandle=0;
    uInt32		data[1000];
    int32		sampsRead,totalRead=0;
    char		errBuff[2048]={'\0'};
	char        strChannelName[100]={'\0'};
	char        strExternalClock[100]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	printf("Please enter channal number,such as Dev1/port0/line0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter external clock source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strExternalClock);
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateDIChan(taskHandle,strChannelName,"",ArtDAQ_Val_ChanPerLine));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,strExternalClock,10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Acquiring samples continuously. Press Ctrl+C to interrupt\n");
    while( 1 ) {
        /*********************************************/
        // DAQmx Read Code
        /*********************************************/
        ArtDAQErrChk (ArtDAQ_ReadDigitalU32(taskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,data,1000,&sampsRead,NULL));

        if( sampsRead>0 ) {
            totalRead += sampsRead;
            printf("Acquired %d samples. Total %d\r",(int)sampsRead,(int)totalRead);
            fflush(stdout);
        }
    }
    printf("\nAcquired %d total samples.\n",(int)totalRead);
	
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
