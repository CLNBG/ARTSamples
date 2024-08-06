/*********************************************************************
*
* ANSI C Example program:
*    VC_MultVoltUpdates_IntClk_DigStart.c
*
* Example Category:
*    AO
*
* Description:
*    This example demonstrates how to output a finite number of
*    voltage samples to an Analog Output Channel using an internal
*    sample clock and a digital start trigger.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is output on the DAQ device.
*    2. Enter the Minimum and Maximum Voltage Ranges.
*    3. Select the Sample Clock source.
*    4. Select the Digital Trigger Source.
*    5. Specify the desired Trigger Edge.
*    6. Select the desired waveform type.
*    7. The rest of the parameters in the Waveform Information
*       section will affect the way the waveform is created, before
*       it's sent to the analog output of the board. Select the
*       number of samples per cycle and the total number of cycles to
*       be used as waveform data.
*    8. Select the desired waveform frequency; this will determine
*       the update rate of the operation.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Output Voltage Channel.
*    3. Setup the Timing for the Measurement. In this example we use
*       the internal DAQ Device's clock to produce a finite number of
*       samples.
*    4. Define the Triggering parameters: Source and Edge.
*    5. Use the Write function to Generate Multiple Samples to 1
*       Channel on the Data Acquisition Card. The auto start
*       parameter is set to False, so the Start function must be
*       explicitly called to begin the Voltage Generation.
*    6. Call the Start function.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. Also, make sure your digital trigger
*    terminal matches the Trigger Source Control. For further
*    connection information, refer to your hardware reference manual.
*
*********************************************************************/
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;

int main(void)
{
    int         error=0;
    TaskHandle  taskHandle=0;
    float64     data[4000];
    char        errBuff[2048]={'\0'};
    int			i=0;
    int32   	written;

    for(;i<4000;i++)
        data[i] = 5.0*(double)i/4000.0;

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAOVoltageChan(taskHandle,"Dev1/ao0","",-10.0,10.0,ArtDAQ_Val_Volts,""));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",1000,ArtDAQ_Val_Rising,ArtDAQ_Val_FiniteSamps,4000));
    ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(taskHandle,"/Dev1/PFI1",ArtDAQ_Val_Rising));

    /*********************************************/
    // ArtDAQ Write Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_WriteAnalogF64(taskHandle,4000,0,10.0,ArtDAQ_Val_GroupByChannel,data,&written,NULL));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    /*********************************************/
    // ArtDAQ_ Wait Code
    /*********************************************/
    ArtDAQErrChk(ArtDAQ_WaitUntilTaskDone(taskHandle, 10.0));

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
