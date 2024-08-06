/*********************************************************************
*
* ANSI C Example program:
*    VC_Synch_AI_AO.c
*
* Example Category:
*    Sync
*
* Description:
*    This example demonstrates how to continuously acquire and
*    generate data at the same time, synchronized with one another.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device. Also, select the
*       corresponding channel for where your signal is being
*       generated.
*    2. Enter the minimum and maximum voltage ranges.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the sample rate of the acquisition.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*    4. Select the rate for the generation.
*    5. Select what type of signal to generate and the amplitude.
*    Note: This example requires two DMA channels to run. If your
*          hardware does not support two DMA channels, you need to
*          set the Data Transfer Mechanism attribute for the Analog
*          Output Task to use "Interrupts".
*
*    Refer to your device documentation to determine how many DMA
*    channels are supported for your hardware.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel. Also, create a analog
*       output channel.
*    3. Set the rate for the sample clocks. Additionally, define the
*       sample modes to be continuous. Also, set the sample clock
*       rate for the signal generation.
*    4. Define the parameters for same digital edge start trigger.
*    5. Synthesize a standard waveform (sine, square, or triangle)
*       and load this data into the output RAM buffer.
*    6. Call the start function to arm the two tasks. Make sure the
*       analog output is armed before the analog input. This will
*       ensure both will start at the same time.
*    7. Read the waveform data continuously until the user hits the
*       stop button or an error occurs.
*    8. Call the Stop function to stop the acquisition.
*    9. Call the Clear Task function to clear the task.
*    10. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminals match the Physical Channel
*    I/O controls.
*
*********************************************************************/
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

static TaskHandle  AItaskHandle=0,AOtaskHandle=0;
#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;
int GenSineWave(int numElements, double amplitude, double frequency, double *phase, double sineWave[]);
#define PI	3.1415926535
int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
    int32   error=0;
    char    errBuff[2048]={'\0'};
    float64	AOdata[1000];
    float64	phase=0.0;

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/

    // All
    ArtDAQErrChk (ArtDAQ_CreateTask("",&AItaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(AItaskHandle,"Dev1/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(AItaskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));
    ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(AItaskHandle,"/Dev1/PFI1",ArtDAQ_Val_Rising));
    ArtDAQErrChk (ArtDAQ_CreateTask("",&AOtaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAOVoltageChan(AOtaskHandle,"Dev1/ao0","",-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(AOtaskHandle,"",5000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));
    ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(AOtaskHandle,"/Dev1/PFI1",ArtDAQ_Val_Rising));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(AItaskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(AItaskHandle,0,DoneCallback,NULL));

    GenSineWave(1000,1.0,1.0/1000,&phase,AOdata);

    ArtDAQErrChk (ArtDAQ_WriteAnalogF64(AOtaskHandle, 1000, FALSE, 10.0, ArtDAQ_Val_GroupByChannel, AOdata, NULL, NULL));

    /*********************************************/
    // ArtDAQ_ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(AOtaskHandle));
    //ArtDAQErrChk (ArtDAQ_StartTask(AItaskHandle));

    printf("Acquiring samples continuously. Press Enter to interrupt\n");
    printf("\nRead:\tAI\tTotal:\tAI\n");
    getchar();

Error:
    if( ArtDAQFailed(error) )
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
    if( AItaskHandle ) {
        /*********************************************/
        // ArtDAQ_ Stop Code
        /*********************************************/
        ArtDAQ_StopTask(AItaskHandle);
        ArtDAQ_ClearTask(AItaskHandle);
        AItaskHandle = 0;
    }
    if( AOtaskHandle ) {
        /*********************************************/
        // ArtDAQ_ Stop Code
        /*********************************************/
        ArtDAQ_StopTask(AOtaskHandle);
        ArtDAQ_ClearTask(AOtaskHandle);
        AOtaskHandle = 0;
    }
    if( ArtDAQFailed(error) )
        printf("ArtDAQ_ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
    return 0;
}

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    int32       error=0;
    char        errBuff[2048]={'\0'};
    static int  totalAI=0;
    int32       readAI;
    float64     AIdata[1000];

    /*********************************************/
    // ArtDAQ_ Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(AItaskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,AIdata,1000,&readAI,NULL));

    printf("\t%d\t\t%d\r",(int)readAI,(int)(totalAI+=readAI));
    fflush(stdout);

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        /*********************************************/
        // ArtDAQ_ Stop Code
        /*********************************************/
        if( AItaskHandle ) {
            ArtDAQ_StopTask(AItaskHandle);
            ArtDAQ_ClearTask(AItaskHandle);
            AItaskHandle = 0;
        }
        if( AOtaskHandle ) {
            ArtDAQ_StopTask(AOtaskHandle);
            ArtDAQ_ClearTask(AOtaskHandle);
            AOtaskHandle = 0;
        }
        printf("ArtDAQ_ Error: %s\n",errBuff);
    }
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
        if( AItaskHandle ) {
            ArtDAQ_StopTask(AItaskHandle);
            ArtDAQ_ClearTask(AItaskHandle);
            AItaskHandle = 0;
        }
        if( AOtaskHandle ) {
            ArtDAQ_StopTask(AOtaskHandle);
            ArtDAQ_ClearTask(AOtaskHandle);
            AOtaskHandle = 0;
        }
        printf("ArtDAQ_ Error: %s\n",errBuff);
    }
    return 0;
}

int GenSineWave(int numElements, double amplitude, double frequency, double *phase, double sineWave[])
{
    int i=0;

    for(;i<numElements;++i)
        sineWave[i] = amplitude*sin(PI/180.0*(*phase+360.0*frequency*i));
    *phase = fmod(*phase+frequency*360.0*numElements,360.0);
    return 0;
}
