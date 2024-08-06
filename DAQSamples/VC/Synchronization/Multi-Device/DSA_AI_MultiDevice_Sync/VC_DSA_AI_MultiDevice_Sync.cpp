/*********************************************************************
*
* ANSI C Example program:
*    VC_DSA_AI_MultiDevice_Sync.cpp
*
* Example Category:
*    Sync
*
* Description:
*    This example configures a total of four tasks on two Dynamic
*    Signal Acquistion (DSA) devices. Each device runs a task for
*    analog input.
*
* Instructions for Running:
*    1. Select which type of Syncrhonization method you want to use.
*    2. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    3. Enter the minimum and maximum expected voltage for the input
*       and output operations on each device.
*    Note: For optimal accuracy, match the input range to the
*          expected voltage level of the measured signal (for input)
*          and generated signal (for output).
*    4. Set the number of samples to acquire per channel. This
*       parameter determines how many points will be read and
*       generated with each iteration.
*    5. Set the rate of the acquisition and generation. The same
*       sampling rate is employed for input and output operations on
*       each device.
*    Note: The sampling rate should be at least 2.2 times the maximum
*          frequency component of the signal being acquired on analog
*          input and the signal being generated on analog output.
*          Frequency components beyond about 0.47 times the sampling
*          rate will be eliminated by the alias and imaging
*          protection on the DSA device.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input channel for both the master and slave 
*       devices.
*    3. Set timing parameters for a continuous acquisition. The
*       sample rate and block size are set to the same values for
*       each device.
*    4. There are two types of synchronization available on DSA
*       devices. The first method shares the Sample Clock Timebase
*       across the PXI_Star bus. It also uses the Sync Pulse which is
*       shared across the RTSI bus. The second method uses
*       the Sync Pulse in conjunction with the PXI Reference clock 10M
*       on the PXI backplane or External 10M Clock input scross the RTSI 
*       bus.
*    5. Call the Export function of Sample Clock Timebase and Sync Pulse. 
*       This signal is then used as an output generation trigger on the 
*       master as well as and acquisition start trigger and generation start
*       trigger on the slave. The signal is shared along the PXI_Trig
*       / RTSI bus.
*    6. Call the Start function to start the acquisition.
*    7. Read all of the data continuously. The 'Samples per Channel'
*       control will specify how many samples per channel are read
*       each time. If either device reports an error or the user
*       presses the 'Stop' button, the acquisition will stop.
*    8. Call the Clear Task function to clear the task.
*    9. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input and output terminals matches the
*    Physical Channel I/O controls.
*
*********************************************************************/

#include <stdio.h>
#include "Art_DAQ.h"

static TaskHandle gMasterAItaskHandle=0,gSlaveAItaskHandle=0;

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 ART_CALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

int main(void)
{
	int32       error=0;
    char        errBuff[2048]={'\0'};
    int         synchronizationType=0;	/* 0 = Sample Clock; 1 = Reference Clock */

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&gMasterAItaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(gMasterAItaskHandle,"Dev1/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(gMasterAItaskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

    ArtDAQErrChk (ArtDAQ_CreateTask("",&gSlaveAItaskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(gSlaveAItaskHandle,"Dev2/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(gSlaveAItaskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));

    if( synchronizationType == 1) 
    {
		// 选择参考时钟同步，主从卡都需要从板卡指定引脚(PCI卡指定RTSI9，PXI卡指定PXI_10M)
        ArtDAQErrChk (ArtDAQ_SetRefClkSrc(gMasterAItaskHandle,"RTSI9"));
        ArtDAQErrChk (ArtDAQ_SetRefClkSrc(gSlaveAItaskHandle,"RTSI9"));
    }
    else
    {
		// 主卡导出主采样时钟时基，从卡输入采样时钟时基
         ArtDAQErrChk (ArtDAQ_SetExportedSampClkTimebaseOutputTerm(gMasterAItaskHandle,"RTSI8"));
         ArtDAQErrChk (ArtDAQ_SetSampClkTimebaseSrc(gSlaveAItaskHandle,"RTSI8"));
     }

	 // 主卡输出同步脉冲，从卡输入同步脉冲，用来同步DDC
     ArtDAQErrChk (ArtDAQ_SetExportedSyncPulseEventOutputTerm(gMasterAItaskHandle,"RTSI0"));
     ArtDAQErrChk (ArtDAQ_SetSyncPulseSrc(gSlaveAItaskHandle,"RTSI0"));

    /*********************************************/
    // ArtDAQ Configure Start Trigger Code
    /*********************************************/
	// 主卡可以选择外部触发，并将触发导出；也可以不配置触发，导出内部的触发信号来同步采集
    //ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(gMasterAItaskHandle,"PFI0",ArtDAQ_Val_Rising));
    ArtDAQErrChk (ArtDAQ_ExportSignal(gMasterAItaskHandle,ArtDAQ_Val_StartTrigger, "RTSI1"));
    ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(gSlaveAItaskHandle,"RTSI1",ArtDAQ_Val_Rising));

    ArtDAQErrChk (ArtDAQ_RegisterEveryNSamplesEvent(gMasterAItaskHandle,ArtDAQ_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(gMasterAItaskHandle,0,DoneCallback,NULL));
    ArtDAQErrChk (ArtDAQ_RegisterDoneEvent(gSlaveAItaskHandle,0,DoneCallback,NULL));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(gSlaveAItaskHandle));
    ArtDAQErrChk (ArtDAQ_StartTask(gMasterAItaskHandle));

    printf("Acquiring samples continuously. Press Enter to interrupt\n");
    printf("\nRead:\tMaster\tSlave\tTotal:\tMaster\tSlave\n");
    getchar();

Error:
    if( ArtDAQFailed(error) )
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);

    /*********************************************/
    // ArtDAQ Cleanup Code
    /*********************************************/
    if( gMasterAItaskHandle ) {
        ArtDAQ_StopTask(gMasterAItaskHandle);
        ArtDAQ_ClearTask(gMasterAItaskHandle);
        gMasterAItaskHandle = 0;
    }

    if( gSlaveAItaskHandle ) {
        ArtDAQ_StopTask(gSlaveAItaskHandle);
        ArtDAQ_ClearTask(gSlaveAItaskHandle);
        gSlaveAItaskHandle = 0;
    }

    if (ArtDAQFailed(error))
        printf("ArtDAQ Error: %s\n", errBuff);
    printf("End of program, press Enter key to quit");
    getchar();
    return 0;
}

int32 ART_CALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
    int32           error=0;
    char            errBuff[2048]={'\0'};
    static int32    masterTotal=0,slaveTotal=0;
    int32           masterRead,slaveRead=0;
    float64         masterData[2000],slaveData[2000];


    /*********************************************/
    // DAQmx Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(gMasterAItaskHandle,2000,10.0,ArtDAQ_Val_GroupByChannel,masterData,2000,&masterRead,NULL));
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(gSlaveAItaskHandle,2000,10.0,ArtDAQ_Val_GroupByChannel,slaveData,2000,&slaveRead,NULL));

    if( masterRead>0 )
        masterTotal += masterRead;
    if( slaveRead>0 )
        slaveTotal += slaveRead;
    printf("\t%d\t%d\t\t%d\t%d\r",(int)masterRead,(int)slaveRead,(int)masterTotal,(int)slaveTotal);

Error:
    if( ArtDAQFailed(error) ) {
        ArtDAQ_GetExtendedErrorInfo(errBuff,2048);
        /*********************************************/
        // ArtDAQ Stop Code
        /*********************************************/
        if( gMasterAItaskHandle ) {
            ArtDAQ_StopTask(gMasterAItaskHandle);
            ArtDAQ_ClearTask(gMasterAItaskHandle);
            gMasterAItaskHandle = 0;
        }

        if( gSlaveAItaskHandle ) {
            ArtDAQ_StopTask(gSlaveAItaskHandle);
            ArtDAQ_ClearTask(gSlaveAItaskHandle);
            gSlaveAItaskHandle = 0;
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

        if( gMasterAItaskHandle ) {
            ArtDAQ_StopTask(gMasterAItaskHandle);
            ArtDAQ_ClearTask(gMasterAItaskHandle);
            gMasterAItaskHandle = 0;
        }

        if( gSlaveAItaskHandle ) {
            ArtDAQ_StopTask(gSlaveAItaskHandle);
            ArtDAQ_ClearTask(gSlaveAItaskHandle);
            gSlaveAItaskHandle = 0;
        }

        printf("ArtDAQ Error: %s\n",errBuff);
    }
    return 0;
}
