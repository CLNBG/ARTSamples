/*********************************************************************
*
* ANSI C Example program:
*    ExportPositionComparsionEvent.cpp
*
* Example Category:
*    CI
*
* Description:
*    This example demonstrates how to export position comparsion event.
*
*    Preset one or a group of counter values, the hardware in order to 
*    take out the first preset value. When the counter value is equal 
*    to the preset value, generate an event exported to the specified 
*    terminal. Can choose to export pulse or level polarity toggle, 
*    when the pulse is exported, pulse width can follow the counter 
*    source, can also be preset width.
*    
*
* Instructions for Running:
*    1. Select the Physical Channel which corresponds to the counter
*       you want to count edges on the DAQ device.
*    2. Enter the Initial Count, Count Direction, and measurement
*       Edge to specify how you want the counter to count.
*       Additionally, you can change the input terminal where events
*       are counted using the ArtDAQ Channel attributes.
*    3. Export PositionComparsionEvent and configure parameter.
*
*
* Steps:
*    1. Create a task.
*    2. Create a Counter Input channel to Count Events. The Edge
*       parameter is used to determine if the counter will increment
*       on rising or falling edges.
*    3. Call the ArtDAQ_ExportPositionComparsionEvent to configure
*       the terminal to exported, choose to export pulse or 
*       level polarity toggle, and when choose pulse, set pulseWidthMode
*       to ArtDAQ_Val_PulseWidth_Implicit follow the counter source, and 
*       set to ArtDAQ_Val_PulseWidth_PreSet use preset width.
*    4. Write position, and if need preset pulse width, write the pulse width
*       value same.
*    5. Call the Start function to arm the counter and begin
*       counting. The counter will be preloaded with the Initial
*       Count.
*    6. The counter will be continually polled until the Stop button
*       is pressed on the front panel.When PositionComparsionEvent generated, 
*       can export behavior you set.
*    7. Call the Clear Task function to clear the Task.
*    8. Display an error if any.
*
* I/O Connections Overview:
*    The counter will count edges on the input terminal of the
*    counter specified in the Physical Channel I/O control.
*
*    This example uses the default source (or gate) terminal for the
*    counter of your device. 
*
*********************************************************************/

#include <stdio.h>
#include <Art_DAQ.h>

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
    int         error=0;
    TaskHandle  taskHandle=0;
    int32       read;
    uInt32      data[1000];
    uInt32      position[100];
    uInt32      pulseWidth[100];
    int32       written;
    char        errBuff[2048]={'\0'};

    for (int i = 0; i < 100; i++)
    {
        position[i] = 1000 * i;
        pulseWidth[i] = 1000;    // 单位时间，单位100ns
    }

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateCICountEdgesChan(taskHandle,"Dev3/ctr0","",ArtDAQ_Val_Rising,0,ArtDAQ_Val_CountUp));

    ArtDAQErrChk (ArtDAQ_ExportPositionComparsionEvent(taskHandle,"/Dev1/PFI11", ArtDAQ_Val_Pulse, ArtDAQ_Val_ActiveHigh, ArtDAQ_Val_High, ArtDAQ_Val_PulseWidth_Implicit));

    ArtDAQErrChk (ArtDAQ_WritePositionComparsionData(taskHandle, 100, position, pulseWidth, &written, NULL));
    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Continuously reading. Press Ctrl+C to interrupt\n");
    while( 1 ) {
        /*********************************************/
        // ArtDAQ Read Code
        /*********************************************/
        ArtDAQErrChk (ArtDAQ_ReadCounterU32(taskHandle,1000,10.0,data,1000,&read,0));

        printf("Acquired %d samples\n",(int)read);
        fflush(stdout);
    }

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
