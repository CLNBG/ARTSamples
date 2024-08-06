/*********************************************************************
*
* ANSI C Example program:
*    VC_PeriodLowFreq.cpp 
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to measure period using one
*    counter on a Counter Input Channel. The Edge, Minimum Value and
*    Maximum Value are all configurable.

*
*    This example shows how to measure period on the counter's
*    default input terminal (refer to the I/O Connections Overview
*    section below for more information).
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to measure period on the DAQ device.
*    2. Enter the Measurement Edge to specify the edge on which you
*       want the counter to start measuring. Enter the Maximum and
*       Minimum Value to specify the range of your unknown period.

*    Note: It is important to set the Maximum and Minimum Values of
*          your unknown period as accurately as possible so the
*          best internal timebase can be chosen to minimize
*          measurement error. The default values specify a range that
*          can be measured by the counter using the 100MHzTimebase.
*          Use the Gen Dig Pulse Train-Continuous example to verify
*          that you are measuring correctly on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel for Period. The Edge
*       parameter is used to determine if the counter will begin
*       measuring on a rising or falling edge. It is important to set
*       the Maximum and Minimum Values of your unknown signal as
*       accurately as possible so the best internal timebase can be
*       chosen to minimize measurement error. The default values
*       specify the range that can be measured by the counter using
*       the 100MHzTimebase.
*    3. Call the Start function to arm the counter and begin
*       measuring.
*    4. Call the Read function to return the next period
*       measurement. Set a timeout so an error is returned if a
*       period is not returned in the specified time limit.
*    5. Call the Stop function to clean up the task.
*    6. Display an error if any.
*
* I/O Connections Overview:
*    The counter will measure period on the input terminal of the
*    counter specified in the Physical Channel I/O control.

*
*    This example uses the default source (or gate) terminal for the
*    counter of your device. To determine what the default counter
*    pins for your device are or to set a different source (or gate)
*    pin, refer to the Connecting Counter Signals topic in the
*    Help (search for "Connecting Counter Signals").
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
	
    int         error=0;
    TaskHandle  taskHandle=0;
    float64     data[1];
    char        errBuff[2048]={'\0'};
	char        strChannelName[100]={'\0'};

	/*********************************************/
	// ArtDAQ Configure Code
	/*********************************************/
	printf("Please enter channal number,such as Dev1/ctr0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
	ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
	ArtDAQErrChk (ArtDAQ_CreateCIPeriodChan(taskHandle,strChannelName,"",0.00000002,42.9496,ArtDAQ_Val_Seconds,ArtDAQ_Val_Rising,ArtDAQ_Val_LowFreq1Ctr,0.001,4,""));

	/*********************************************/
	// ArtDAQ Start Code
	/*********************************************/
	ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    /*********************************************/
    // ArtDAQ Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadCounterScalarF64(taskHandle,10.0,&data[0],0));

    printf("Frequency: %.9f s\n",data[0]);
	
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
