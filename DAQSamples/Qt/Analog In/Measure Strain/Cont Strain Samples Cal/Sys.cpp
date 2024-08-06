/*********************************************************************
*
* ANSI C Example program:
*    ContAcq-IntClk
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire a continuous amount of
*    data using the DAQ device's internal clock.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltage range.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the rate of the acquisition. Also set the Samples per
*       Channel control. This will determine how many samples are
*       read at a time. This also determines how many points are
*       plotted on the graph each time.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Set the rate for the sample clock. Additionally, define the
*       sample mode to be continuous.
*    4. Call the Start function to start the acquistion.
*    5. Read the data in the EveryNCallback function until the stop
*       button is pressed or an error occurs.
*    6. Call the Clear Task function to clear the task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control. For further connection information, refer
*    to your hardware reference manual.
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

        /*********************************************/
        // ArtDAQ Configure Code
        /*********************************************/
        ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
        ArtDAQErrChk (ArtDAQ_CreateAIStrainGageChan(taskHandle,"","",-0.001,0.001,ArtDAQ_Val_Strain,ArtDAQ_Val_FullBridgeI,ArtDAQ_Val_Internal,2.50,2.0,0.0,120.0,0.285,0.0,""));
        ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",10.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

        ArtDAQErrChk (ArtDAQ_PerformBridgeOffsetNullingCal(taskHandle,""));
        ArtDAQErrChk (ArtDAQ_PerformStrainShuntCal(taskHandle,"",100000,0,0));

        ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(taskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
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
            printf("ArtDAQ_ Error: %s\n",errBuff);
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
