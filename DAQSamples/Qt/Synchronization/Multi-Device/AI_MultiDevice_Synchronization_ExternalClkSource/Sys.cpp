/*********************************************************************
*
* ANSI C Example program:
*    VC_AI_Sync_ExternalClkSource.c
*
* Example Category:
*    Sync
*
* Description:
*    This example demonstrates how to acquire a continuous amount of
*    data using the DAQ device's internal clock. It also shows how to
*    synchronize two devices, to simultaneously acquire the data.
*
*    Note: For Non-Synchronous(MUX) device, the Convert Clock is used
*		   for all sample channel, so
*	       ConvertClock = SampleClock * TotalSampleChannelNumber
*	       In order to keep each device's Sample Clock same, the
*		   TotalSampleChannelNumber must also keep same.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltage range.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the rate of the acquisition.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*    4. Set the number of samples to acquire per channel.
*    5. Choose which type of devices you are trying to synchronize.
*       This will select the correct synchronization method to use.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel for both the Master
*       and Slave devices.
*    3. Set timing parameters. Note that sample mode is set to
*       Continuous Samples. In this example, the Rate and the Samples
*		per Channel is set the same for both devices. Set the Sample
*		clock to internal clock for Master device, and external clock
*		for Slave device.
*    4. Export the Convert clock of Master device.
*    5. Call the Start function to start the Slave device acquisition first.
*    6. Call the Start function to start the Mater device acquisition.
*    7. Read all of the data continuously. The 'Samples per Channel'
*       control will specify how many samples per channel are read
*       each time. If either device reports an error or the user
*       presses the 'Stop' button, the acquisition will stop.
*    8. Call the Clear Task function to clear the task.
*    9. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

static TaskHandle masterTaskHandle=0,slaveTaskHandle=0;
#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;
int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
    int32       error=0;
    char        errBuff[2048]={'\0'};

    /*********************************************/
    // ArtDAQ_ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&masterTaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(masterTaskHandle,"Dev1/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    // Master device use external clock
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(masterTaskHandle,"/Dev1/PFI2",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,10000));

    ArtDAQErrChk (ArtDAQ_CreateTask("",&slaveTaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(slaveTaskHandle,"Dev2/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    // Slave device use external clock
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(slaveTaskHandle,"/Dev2/PFI2",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,10000));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(masterTaskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(masterTaskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ_ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(slaveTaskHandle));
    ArtDAQErrChk (ArtDAQ_StartTask(masterTaskHandle));

    printf("Acquiring samples continuously. Press Enter to interrupt\n");
    printf("\nRead:\tMaster\tSlave\tTotal:\tMaster\tSlave\n");
    getchar();

Error:
    if( ArtDAQFailed(error) )
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
    /*********************************************/
    // ArtDAQ_ Stop Code
    /*********************************************/
    if( masterTaskHandle ) {
        ArtDAQ_StopTask(masterTaskHandle);
        ArtDAQ_ClearTask(masterTaskHandle);
        masterTaskHandle = 0;
    }
    if( slaveTaskHandle ) {
        ArtDAQ_StopTask(slaveTaskHandle);
        ArtDAQ_ClearTask(slaveTaskHandle);
        slaveTaskHandle = 0;
    }

    if( ArtDAQFailed(error))
        printf("ArtDAQ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
    return 0;
}

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    int32           error=0;
    char            errBuff[2048]={'\0'};
    static int32    masterTotal=0,slaveTotal=0;
    int32           masterRead,slaveRead;
    //float64         masterData[1000],slaveData[1000];
    float64*         masterData = new float64[1000];
    float64*         slaveData = new float64[1000];

    /*********************************************/
    // DAQmx Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(masterTaskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,masterData,1000,&masterRead,NULL));
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(slaveTaskHandle,1000,10.0,ArtDAQ_Val_GroupByChannel,slaveData,1000,&slaveRead,NULL));

    if( masterRead>0 )
        masterTotal += masterRead;
    if( slaveRead>0 )
        slaveTotal += slaveRead;
    printf("\t%d\t%d\t\t%d\t%d\r",(int)masterRead,(int)slaveRead,(int)masterTotal,(int)slaveTotal);
    fflush(stdout);

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        /*********************************************/
        // DAQmx Stop Code
        /*********************************************/
        if( masterTaskHandle ) {
            ArtDAQ_StopTask(masterTaskHandle);
            ArtDAQ_ClearTask(masterTaskHandle);
        }
        if( slaveTaskHandle ) {
            ArtDAQ_StopTask(slaveTaskHandle);
            ArtDAQ_ClearTask(slaveTaskHandle);
        }
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
        if( slaveTaskHandle ) {
            ArtDAQ_StopTask(slaveTaskHandle);
            ArtDAQ_ClearTask(slaveTaskHandle);
        }
        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}
