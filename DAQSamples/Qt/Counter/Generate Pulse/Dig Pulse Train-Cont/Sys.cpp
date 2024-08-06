/*********************************************************************
*
* ANSI C Example program:
*    DigPulseTrain-Cont
*
* Example Category:
*    CO
*
* Description:
*    This example demonstrates how to generate a continuous digital
*    pulse train from a Counter Output Channel. The Frequency, Duty
*    Cycle, and Idle State are all configurable.
*
*    This example shows how to configure the pulse in terms of
*    Frequency/Duty Cycle, but can easily be modified to generate a
*    pulse in terms of Time or Ticks.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to output your signal to on the DAQ device.
*    2. Enter the Frequency and Duty Cycle to define the pulse
*       parameters. You can also change the Idle State to determine
*       the beginning and end states of the output. If the Idle State
*       is High, the generation will use inverted logic.
*       Additionally, you can set the Initial Delay (in seconds)
*       which will delay the beginning of the pulse train from the
*       start call; this is currently set to 0.0 in the code.

*    Note: Use the Measure Period example to verify you are
*          outputting the pulse train on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Output channel to produce a Pulse in terms
*       of Frequency. If the Idle State of the pulse is set to low,
*       the state of the line will begin low and remain low after the
*       generation is stopped.
*    3. Call the timing function to configure the duration of the
*       pulse generation.
*    4. Call the Start function to arm the counter and begin the
*       pulse
*    train generation.
*    5. For continuous generation, the counter will continually
*       generate the pulse train until stop button is pressed.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
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
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

#define ArtDAQ_ErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
    int         error=0;
    TaskHandle  taskHandle=0;
    char        errBuff[2048]={'\0'};
    char        strChannelName[100]={'\0'};

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    printf("Please enter channal number,such as Dev1/ctr0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    ArtDAQ_ErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQ_ErrChk (ArtDAQ_CreateCOPulseChanFreq(taskHandle,strChannelName,"",ArtDAQ_Val_Hz,ArtDAQ_Val_Low,0.0,1.00,0.50));
    ArtDAQ_ErrChk (ArtDAQ_CfgImplicitTiming(taskHandle,ArtDAQ_Val_ContSamps,1000));

    ArtDAQ_ErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQ_ErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Generating pulse train. Press Enter to interrupt\n");
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
        printf("ArtDAQ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
    getchar();
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
