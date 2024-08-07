/*********************************************************************
*
* ANSI C Example program:
*    DigPulse.c
*
* Example Category:
*    CO
*
* Description:
*    This example demonstrates how to generate a single digital pulse
*    from a Counter Output Channel. The Initial Delay, High Time, Low
*    Time, and Idle State are all configurable.
*
*    This example shows how to configure the pulse in terms of time,
*    but can easily be modified to generate a pulse in terms of
*    Frequency/Duty Cycle or Ticks.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to output your signal to on the DAQ device.
*    2. Enter the Low Time and High Time (in seconds) to define the
*       pulse parameters. Additionally, you can set the Initial Delay
*       (in seconds) which will delay the beginning of the pulse from
*       the start call. You can also change the Idle State to
*       determine the beginning and end states of the output. If the
*       Idle State is High, the generation will use inverted logic.
*    Note: Use the Measure Pulse Width example to verify you are
*          outputting the pulse on the DAQ device.
*
* Steps:
*    1. Create a Counter Output channel to produce a Pulse in terms
*       of Time. If the Idle State of the pulse is set to low, the
*       state of the line will begin low and remain low after the
*       generation is stopped.
*    2. Call the Start function to arm the counter and begin the
*       pulse
*    generation. The pulse would not begin until after the Initial
*    Delay (in seconds) has expired.
*    3. Call the Clear Task function to clear the Task.
*    4. Display an error if any.
*
* I/O Connections Overview:
*    The counter will output the pulse on the output terminal of the
*    counter specified in the Physical Channel I/O control.
*
*    This example uses the default output terminal for the counter of
*    your device. To determine what the default counter pins for your
*    device are or to set a different output terminal, refer to the
*    Connecting Counter Signals topic in the ArtDAQ Help (search
*    for "Connecting Counter Signals").
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
	
    char        errBuff[2048]={'\0'};
    TaskHandle  taskHandle=0;
    int32       error=0;
	char        strChannelName[100]={'\0'};

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
	printf("Please enter channal number,such as Dev1/ctr0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateCOPulseChanTime(taskHandle,strChannelName,"",ArtDAQ_Val_Seconds,ArtDAQ_Val_Low,1.00,0.50,1.00));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    /*********************************************/
    // ArtDAQ Wait Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_WaitUntilTaskDone(taskHandle,10.0));

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
        printf("ArtDAQ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getch();
    return 0;
}
