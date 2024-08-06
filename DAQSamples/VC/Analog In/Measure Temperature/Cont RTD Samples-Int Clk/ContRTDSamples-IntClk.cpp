/*********************************************************************
*
* ANSI C Example program:
*    ContRTDSamples-IntClk.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire temperature from an RTD
*    using the internal clock of the DAQ device.
*
* Instructions for Running:
*    1. Select the Physical Channel(s) to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the Minimum and Maximum Temperature Ranges.
*    Note: For better accuracy try to match the Input Ranges to the
*          expected temperature level of the measured signal.
*    3. Enter the acquisition rate.
*    4. Enter the RTD Type and r0 (resistance at 0 degrees C).
*    Note: If you select "Custom" as your RTD type, you need to
*          modify this example in order to provide the A, B, and C
*          coefficients of the Callendar-Van Dusen equation. The
*          coefficients are specified using the DAQmx Set Channel
*          Attribute function.
*    5. Enter the Resistance Configuration, the current excitation
*       source, and the excitation value in Amps.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Input Temperature RTD Channel.
*    3. Set the rate for the sample clock. Additionally, define the
*       sample mode to be continuous.
*    4. Call the Start function to start acquiring samples.
*    5. Read the waveform data in the EveryNCallback function until
*       the user hits the stop button or an error occurs.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control.
*
*********************************************************************/
#include <stdio.h>
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
    int32       error=0;
    TaskHandle  taskHandle=0;
    char        errBuff[2048]={'\0'};

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIRTDChan(taskHandle,"","",0.0,100.0,ArtDAQ_Val_DegC,ArtDAQ_Val_Pt3750,ArtDAQ_Val_4Wire,ArtDAQ_Val_Internal,0.004,100.0));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",50.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(taskHandle,ArtDAQ_Val_Acquired_Into_Buffer,50,0,EveryNCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Acquiring samples continuously. Press Enter to interrupt\n");
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

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    int32       error=0;
    char        errBuff[2048]={'\0'};
    static int  totalRead=0;
    int32       read=0;
    float64     data[1000];

    /*********************************************/
    // ArtDAQ Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(taskHandle,-1,10.0,ArtDAQ_Val_GroupByScanNumber,data,1000,&read,NULL));
    if( read>0 ) {
        printf("Acquired %d samples. Total %d\r",(int)read,(int)(totalRead+=read));
        fflush(stdout);
    }

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        /*********************************************/
        // ArtDAQ Stop Code
        /*********************************************/
        ArtDAQ_StopTask(taskHandle);
        ArtDAQ_ClearTask(taskHandle);
        printf("ArtDAQ Error: %s\n",errBuff);
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
        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}