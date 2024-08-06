/*********************************************************************
*
* ANSI C Example program:
*    VC_CntDigEdges.c
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to count digital events on a
*    Counter Input Channel. The Initial Count, Count Direction, and
*    Edge are all configurable.
*
*    Edges are counted on the counter's default input terminal (refer
*    to the I/O Connections Overview section below for more
*    information), but could easily be modified to count edges on a
*    PFI or RTSI line.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to count edges on the DAQ device.
*    2. Enter the Initial Count, Count Direction, and measurement
*       Edge to specify how you want the counter to count.
*       Additionally, you can change the input terminal where events
*       are counted using the ArtDAQ Channel attributes.
*    Note: Use the Gen Dig Pulse Train-Continuous example to verify
*          that you are counting correctly on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel to Count Events. The Edge
*       parameter is used to determine if the counter will increment
*       on rising or falling edges.
*    3. Call the Start function to arm the counter and begin
*       counting. The counter will be preloaded with the Initial
*       Count.
*    4. The counter will be continually polled until the Stop button
*       is pressed on the front panel.
*    5. Call the Clear Task function to clear the Task.
*    6. Display an error if any.
*
* I/O Connections Overview:
*    The counter will count edges on the input terminal of the
*    counter specified in the Physical Channel I/O control.
*
*    This example uses the default source (or gate) terminal for the
*    counter of your device. 
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
	uInt32		data;
	char		errBuff[2048]={'\0'};
	char        strChannelName[100]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	printf("Please enter channal number,such as Dev1/ctr0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateCICountEdgesChan(taskHandle,strChannelName,"",ArtDAQ_Val_Rising,0,ArtDAQ_Val_CountUp));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

	printf("Continuously polling. Press Ctrl+C to interrupt\n");
	while( 1 ) {
		/*********************************************/
		// ArtDAQ Read Code
		/*********************************************/
		ArtDAQErrChk (ArtDAQ_ReadCounterScalarU32(taskHandle,10.0,&data,0));
		
		printf("\rCount: %u",(unsigned int)data);
		fflush(stdout);
	}
	
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
