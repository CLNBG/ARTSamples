/*********************************************************************
*
* ANSI C Example program:
*    DigPulseTrain-Cont-Buff-ExtClk.c
*
* Example Category:
*    CO
*
* Description:
*    This example demonstrates how to generate a continuous buffered
*    sample clocked digital pulse train from a Counter Output
*    Channel. The Frequency, Duty Cycle, and Idle State are all
*    configurable. The default data generated is a pulse train with a
*    fixed frequency but a duty cycle that varies based on the Duty
*    Cycle Max/Min and the signal type. The duty cycle will update
*    with each sample clock edge.

*
*    This example shows how to configure the pulse in terms of
*    Frequency/Duty Cycle, but can easily be modified to generate a
*    pulse in terms of Time or Ticks.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to output your signal to on the DAQ device.
*    2. Enter the Sample Clock Rate, samples per channel and clock
*       source to configure the timing.

*    Note: The sample clock rate should be less than half the output
*          PWM Frequency to avoid an over run error.
*    3. Enter the Frequency and Duty Cycle Min to define the initial
*       pulse parameters.
*    4. Enter the Signal type, Frequency Duty Cycle Min and Duty
*       Cycle Max to define the pulse train generated.
*    Note: Use the Measure Period example to verify you are
*          outputting the pulse train on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Output channel to produce a pulse train in
*       terms of frequency and duty cycle. If the Idle State of the
*       pulse train is set to low, the state of the line will begin
*       low and remain low after the generation is stopped.

*    Note: For a sample clocked buffered output the frequency and
*          duty cycle set in the ArtDAQ Create channel will be
*          generated after the counter starts and before the first
*          sample clock.
*    3. Call the timing function (Sample Clock) to configure Sample
*       Clock source and update rate of the pulse generation.
*    4. Write the array of Frequency and Duty Cycle specifications to
*       the output buffer.
*    5. Call the Start function to arm the counter and begin the
*       pulse train generation.
*    6. For continuous generation, the counter will continually
*       generate the pulse train until stop button is pressed.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    The counter will output the pulse train on the output terminal
*    of the counter specified in the Physical Channel I/O control.
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

#define ArtDAQ_ErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

#define DUTY_MIN	0.50
#define DUTY_MAX	0.80

int main(void)
{
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");
	
    int         error=0;
    TaskHandle  taskHandle=0;
    int32       i;
    float64     dataFreq[1000]={0},dataDC[1000]={0},dutyStep=0;
    char        errBuff[2048]={'\0'};
	char        strChannelName[100]={'\0'};
	char        strExternalClock[100]={'\0'};

    // Generating data for duty cycle from 0.50 to 0.80 in 1000 steps
    dutyStep = (DUTY_MAX-DUTY_MIN)/1000;
    for(i=0;i<1000;++i) {
        dataFreq[i] = 1000.0;
        dataDC[i] = DUTY_MIN + dutyStep*i;
    }

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
	printf("Please enter channal number,such as Dev1/ctr0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter external clock source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strExternalClock);
    ArtDAQ_ErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQ_ErrChk (ArtDAQ_CreateCOPulseChanFreq(taskHandle,strChannelName,"",ArtDAQ_Val_Hz,ArtDAQ_Val_Low,0.0,1.00,0.50));
    ArtDAQ_ErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,strExternalClock,200,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

    ArtDAQ_ErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

    ArtDAQ_ErrChk (ArtDAQ_WriteCtrFreq(taskHandle,1000,0,10.0,dataFreq,dataDC,NULL,NULL));
    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQ_ErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Generating pulse train. Press Enter to interrupt\n");
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
        printf("ArtDAQ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getch();
    return 0;
}

int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
    int32   error=0;
    char    errBuff[2048]={'\0'};

    // Check to see if an error stopped the task.
    ArtDAQ_ErrChk (status);

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        ArtDAQ_ClearTask(taskHandle);
        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}
