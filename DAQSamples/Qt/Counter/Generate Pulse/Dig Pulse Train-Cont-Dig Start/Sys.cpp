/*********************************************************************
*
* ANSI C Example program:
*    DigPulseTrain-Cont-DigStart
*
* Example Category:
*    CO
*
* Description:
*    This example demonstrates how to generate a continuous digital
*    pulse train from a Counter Output Channel started by an external
*    digital trigger. The Frequency, Duty Cycle, and Idle State are
*    all configurable.
*
*    This example shows how to configure the pulse in terms of
*    Frequency/Duty Cycle, but can easily be modified to generate a
*    pulse in terms of Time or Ticks.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to output your signal to on the DAQ device.
*    2. Enter the Frequency and Duty Cycle to define the pulse
*       parameters. You can also change the Idle State to set the
*       state the line will remain in after the generation is
*       stopped. Additionally, you can set the Initial Delay (in
*       seconds) which will delay the beginning of the pulse train
*       from the start call; this is currently set to 0.0 by default.
*    3. Setup the Trigger Parameters. In this example the program is
*       expecting a Rising Edge Digital Trigger on PFI9.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Output channel to produce a Pulse in terms
*       of Frequency. If the Idle State of the pulse is set to low
*       the first transition of the generated signal is from low to
*       high.
*    3. Configure a digital edge trigger.
*    4. Call the Timing function (Implicit) to configure the duration
*       of the pulse generation.
*    5. Call the Start function to arm the counter and begin the
*       pulse train generation.
*    6. For continuous generation, the counter will continually write
*       data until Stop button is pressed.
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
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

#define ArtDAQ_ErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

#define DUTY_MIN	0.50
#define DUTY_MAX	0.80

int main(void)
{
    int         error=0;
    TaskHandle  taskHandle=0;
    char        errBuff[2048]={'\0'};
    char        strChannelName[100]={'\0'};
    char        strTriggerSource[100]={'\0'};

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    printf("Please enter channal number,such as Dev1/ctr0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter trigger source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strTriggerSource);
    ArtDAQ_ErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQ_ErrChk (ArtDAQ_CreateCOPulseChanFreq(taskHandle,strChannelName,"",ArtDAQ_Val_Hz,ArtDAQ_Val_Low,0.0,1.0,0.5));
    ArtDAQ_ErrChk (ArtDAQ_CfgDigEdgeStartTrig(taskHandle,strTriggerSource,ArtDAQ_Val_Rising));
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
