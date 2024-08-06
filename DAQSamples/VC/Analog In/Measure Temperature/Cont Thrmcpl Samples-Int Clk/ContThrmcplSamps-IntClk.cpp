/*********************************************************************
*
* ANSI C Example program:
*    ContThrmcplSamps-IntClk.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to make continuous, hardware-timed
*    temperature measurement using a thermocouple.
*
* Instructions for Running:
*    1. Specify the Physical Channel where you have connected the
*       thermocouple.
*    2. Enter the Minimum and Maximum temperature values in degrees C
*       you expect to measure. A smaller range will allow a more
*       accurate measurement.
*    3. Enter the scan rate at which you want to run the acquisition.
*    4. Specify the type of thermocouple you are using.
*    5. Thermocouple measurements require cold-junction compensation
*       (CJC) to correctly scale them. Specify the source of your
*       cold-junction compensation.
*    6. If your CJC source is "Internal", skip the rest of the steps.
*    7. If your CJC source is "Constant Value", specify the value
*       (usually room temperature) in degrees C.
*    8. If your CJC source is "Channel", specify the CJC Channel
*       name.
*    9. Specify the appropriate Auto Zero Mode. See your device's
*       hardware manual to find out if your device supports this
*       attribute.
*
* Steps:
*    1. Create a task.
*    2. Create a Thermocouple (TC) temperature measurement channel.
*    3. If your device supports Auto Zero Mode, set the AutoZero
*       attribute for all channels in the task.
*    4. Call the Timing function to specify the hardware timing
*       parameters. Use device's internal clock, continuous mode
*       acquisition and the sample rate specified by the user.
*    5. Call the Start function to program and start the acquisition.
*    6. Read N samples and plot it. By default, the Read function
*       reads all available samples, but you can specify how many
*       samples to read at a time and the timeout value. Continue
*       reading data until the stop button is pressed or an error
*       occurs.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Connect your thermocouple to the terminals corresponding to the
*    Physical Channel I/O Control value.
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
    ArtDAQErrChk (ArtDAQ_CreateAIThrmcplChan(taskHandle,"Dev1/ai0","",0.0,100.0,ArtDAQ_Val_DegC,ArtDAQ_Val_J_Type_TC,ArtDAQ_Val_BuiltIn,25.0,""));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",100.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(taskHandle,ArtDAQ_Val_Acquired_Into_Buffer,100,0,EveryNCallback,NULL));
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