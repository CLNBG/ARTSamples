/*********************************************************************
*
* ANSI C Example program:
*    QT_ContWriteDigChanDigPause
*
* Example Category:
*    DO
*
* Description:
*    This example demonstrates how to stop output a continuous digital
*    waveform using Digital Pause Trigger.
*
* Instructions for Running:
*    1. Select the digital lines on the DAQ device to be written.
*    2. Select the OnBoard Clock Source for the generation.
*    3. Specify the Rate of the output Waveform.
*    4. Configure the pause trigger.
*    5. Enter the Waveform Information.
*
* Steps:
*    1. Create a task.
*    2. Create a Digital Output Channel.
*    3. Call the ArtDAQ_CfgSampClkTiming function which sets the sample
*       clock rate. Additionally, set the sample mode to Continuous.
*    4. Configure the pause trigger.
*    5. Write the waveform to the output buffer and default to regeneration.
*    6. Call the Start function to start the task.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. Also, make sure your external clock
*    terminal matches the Clock Source Control. For further
*    connection information, refer to your hardware reference manual.
*
*********************************************************************/
#include <QCoreApplication>
#include "conio.h"
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;

int main(void)
{
    int32       error=0;
    TaskHandle  taskHandle=0;
    uInt8       data[1000];
    uInt32      i=0;
    char        errBuff[2048]={'\0'};
    char        strChannelName[100]={'\0'};
    char        strTriggerSource[100]={'\0'};

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    printf("Please enter channal number,such as Dev1/port0/line0,Dev1 is the name identified in the DMC:");
    scanf("%s", strChannelName);
    printf("Please enter pause trigger source name,such as /Dev1/PFI0,Dev1 is the name identified in the DMC:");
    scanf("%s", strTriggerSource);
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateDOChan(taskHandle,strChannelName,"",ArtDAQ_Val_ChanPerLine));
    ArtDAQErrChk (ArtDAQ_CfgSampClkTiming(taskHandle,"",1000.0,ArtDAQ_Val_Rising,ArtDAQ_Val_ContSamps,1000));
    ArtDAQErrChk (ArtDAQ_CfgDigLvlPauseTrig(taskHandle, strTriggerSource, ArtDAQ_Val_High));

    for(;i<1000;i++)
        data[i] = (uInt8)(i%2);

    /*********************************************/
    // ArtDAQ Write Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_WriteDigitalLines(taskHandle,1000,0,10.0,ArtDAQ_Val_GroupByChannel,data,NULL,NULL));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Generating voltage continuously. Press Enter to interrupt\n");
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
    getchar();
    return 0;
}
