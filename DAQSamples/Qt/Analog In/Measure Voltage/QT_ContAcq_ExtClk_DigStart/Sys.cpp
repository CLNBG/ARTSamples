/*********************************************************************
*
* ANSI C Example program:
*    ContAcq-ExtClk-DigStart
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire a continuous amount of
*    data using an external sample clock, started by a digital edge.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltage ranges.
*    Note: For better accuracy try to match the Input Ranges to the
*          expected voltage level of the measured signal.
*    3. Select a source for the sample clock.
*    4. Set the approximate Rate of the external clock. This allows
*       the internal characteristics of the acquisition to be as
*       efficient as possible. Also set the Samples to Read control.
*       This will determine how many samples are read at a time. This
*       also determines how many points are plotted on the graph each
*       time.
*    5. Select a source for the digital edge start trigger.
*    6. Select the edge, rising or falling, on which to trigger.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Define the parameters for an External Clock Source.
*       Additionally, define the sample mode to be continuous. The
*       external clock rate is given to allow the internal
*       characteristics of the acquisition to be as efficient as
*       possible.
*    4. Set the parameters for a digital edge start trigger.
*    5. Call the Start function to start the acquisition.
*    6. Read the waveform data in the EveryNCallback function until
*       the user hits the stop button or an error occurs.
*    Note: This example reads data from one or more channels and
*          returns an array of data. Use the Index Array function to
*          access an individual channel of data.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control. Also, make sure that your digital trigger
*    signal is connected to the terminal specified in Trigger Source.
*
*********************************************************************/
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
    int32       error=0;
    TaskHandle  taskHandle=0;
    char        errBuff[2048]={'\0'};
    char        strChannelName[100]={'\0'};
    char        strExternalClock[100]={'\0'};
    char        strTriggerSource[100]={'\0'};

    /*********************************************/
    // ArtDAQ_ Configure Code
    /*********************************************/
    printf("Please enter channal number,such as Dev1/ai0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter external clock source name,such as /Dev1/PFI1,Dev1 is the name identified in the DMC:");
    scanf("%s", strExternalClock);
    printf("Please enter digital trigger source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strTriggerSource);
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(taskHandle,strChannelName,"",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,strExternalClock,10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));
    ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(taskHandle,strTriggerSource,ArtDAQ_Val_Rising));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(taskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(taskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ_ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Acquiring samples continuously. Press Enter to interrupt\n");
    getch();

Error:
    if( ArtDAQFailed(error) )
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
    if( taskHandle!=0 ) {
        /*********************************************/
        // ArtDAQ_ Stop Code
        /*********************************************/
        ArtDAQ_StopTask(taskHandle);
        ArtDAQ_ClearTask(taskHandle);
    }
    if( ArtDAQFailed(error) )
        printf("ArtDAQ_ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
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
    // ArtDAQ_ Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(taskHandle,1000,10.0,ArtDAQ_Val_GroupByScanNumber,data,1000,&read,NULL));
    if( read>0 ) {
        printf("Acquired %d samples. Total %d\r",(int)read,(int)(totalRead+=read));
        fflush(stdout);
    }

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        /*********************************************/
        // ArtDAQ_ Stop Code
        /*********************************************/
        ArtDAQ_StopTask(taskHandle);
        ArtDAQ_ClearTask(taskHandle);
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
        printf("ArtDAQ_ Error: %s\n",errBuff);
    }
    return 0;
}
