/*********************************************************************
*
* ANSI C Example program:
*    DigPulseTrain-Cont-Buff-Implicit.c
*
* Example Category:
*    CO
*
* Description:
*    This example demonstrates how to generate a continuous buffered
*    implicit timed digital pulse train from a Counter Output
*    Channel. The Frequency, Duty Cycle, and Idle State are all
*    configurable. The default data generated is a pulse train with a
*    fixed frequency but a duty cycle that varies based on the Duty
*    Cycle Max/Min and the signal type. The duty cycle will update
*    with each sample generated.

*
*    This example shows how to configure the pulse in terms of
*    Frequency/Duty Cycle, but can easily be modified to generate a
*    pulse in terms of Time or Ticks.
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to output your signal to on the DAQ device.
*    2. Enter the samples per channel to configure the timing.
*    3. Enter the Signal type, Frequency Duty Cycle Min and Duty
*       Cycle Max to define the pulse train generated.

*    Note: Use the Measure Period example to verify you are
*          outputting the pulse train on the DAQ device.
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Output channel to produce a Pulse in terms
*       of Frequency. If the Idle State of the pulse is set to low,
*       the state of the line will begin low and remain low after the
*       generation is stopped.
*    Note: For a implicit buffered output the frequency and duty
*          cycle set in the ArtDAQ Create channel will be ignored.
*          Only the samples written to the output buffer in the ArtDAQ_
*          write will be generated.
*    3. Call the timing function (Implicit) to configure the number
*       of samples to generate in the pulse generation.
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

#include <stdio.h>
#include <Art_DAQ.h>

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

#define DUTY_MIN	0.50
#define DUTY_MAX	0.80

int main(void)
{
    int         error=0;
    TaskHandle  taskHandle=0;
    int32       i;
    float64     dataFreq[1000],dataDC[1000],dutyStep;
    char        errBuff[2048]={'\0'};

    // Generating data for duty cycle from 0.50 to 0.80 in 1000 steps
    dutyStep = (DUTY_MAX-DUTY_MIN)/1000;
    for(i=0;i<1000;++i) {
        dataFreq[i] = 1000.0;
        dataDC[i] = DUTY_MIN + dutyStep*i;
    }

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateCOPulseChanFreq(taskHandle,"Dev1/ctr0","",ArtDAQ_Val_Hz,ArtDAQ_Val_Low,0.0,1.00,0.50));
    ArtDAQErrChk (ArtDAQ_CfgImplicitTiming(taskHandle,ArtDAQ_Val_ContSamps,1000));

    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

    ArtDAQErrChk (ArtDAQ_WriteCtrFreq(taskHandle,1000,0,10.0,dataFreq,dataDC,NULL,NULL));
    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

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
