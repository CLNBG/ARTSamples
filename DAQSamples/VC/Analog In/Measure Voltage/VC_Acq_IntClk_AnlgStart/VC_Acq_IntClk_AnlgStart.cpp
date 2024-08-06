/*********************************************************************
*
* ANSI C Example program:
*    Acq-IntClk-AnlgStart.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire a finite amount of data
*    using the DAQ device's internal clock, started by an analog edge
*    condition.
*
* Instructions for Running:
*    1. Select the physical channel to correspond to where your
*       signal is input on the DAQ device.
*    2. Enter the minimum and maximum voltage range.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the number of samples to acquire per channel.
*    4. Set the rate of the acquisiton.
*    Note: The rate should be AT LEAST twice as fast as the maximum
*          frequency component of the signal being acquired.
*    5. Set the source of the start trigger. By default this is
*       analog input channel 0.
*    6. Set the slope and level of desired analog edge condition.
*    7. Set the trigger delay.
*    8. Set the trigger DigFltrPulseWidth.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Set the rate for the sample clock. Additionally, define the
*       sample mode to be finite.
*    4. Define the parameters for an Analog Slope Start Trigger.
*    5. Call the Start function to start the acquistion.
*    6. Read all of the waveform data.
*    7. Call the Clear Task function to stop the acquistion.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O Control. In this case wire your signal to the ai0
*    pin on your DAQ Device. By default, this will also be the signal
*    used as the analog start trigger.
*
*********************************************************************/

#include <stdio.h>
#include <Art_DAQ.h>

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
    int32       error=0;
    TaskHandle  taskHandle=0;
    int32       read;
    float64     data[1000];
    char        errBuff[2048]={'\0'};

    /*********************************************/
    // ArtDAQ_ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAIVoltageChan(taskHandle,"Dev1/ai0","",ArtDAQ_Val_Cfg_Default,-10.0,10.0,ArtDAQ_Val_Volts,NULL));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",10000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_FiniteSamps,1000));
    ArtDAQErrChk (ArtDAQ_CfgAnlgEdgeStartTrig(taskHandle,"Dev1/ai0",ArtDAQ_Val_Rising,1.0));
    ArtDAQErrChk (ArtDAQ_SetStartTrigDelayUnits(taskHandle, ArtDAQ_Val_SampClkPeriods));
    ArtDAQErrChk (ArtDAQ_SetStartTrigDelay(taskHandle, 100)); // 100 points
    ArtDAQErrChk (ArtDAQ_SetStartTrigDigFltrMinPulseWidth(taskHandle, 0.0001));// 0.1ms

    /*********************************************/
    // ArtDAQ_ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    /*********************************************/
    // ArtDAQ_ Read Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_ReadAnalogF64(taskHandle,-1,10.0,ArtDAQ_Val_GroupByChannel,data,1000,&read,NULL));

    if( read>1 )
        printf("Acquired %d samples\n",(int)read);

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
    return 0;
}