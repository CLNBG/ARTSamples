/*********************************************************************
*
* ANSI C Example program:
*    VC_MultVoltUpdates_SWTimed.c
*
* Example Category:
*    AO
*
* Description:
*    This example demonstrates how to output multiple Voltage Updates
*    (Samples) to an Analog Output Channel in a software timed loop.
*
* Instructions for Running:
*    1. Select the Physical Channel to correspond to where your
*       signal is output on the DAQ device.
*    2. Enter the Minimum and Maximum Voltage Ranges.
*    Note: Use the Cont Acq Multiple Samples example to verify you
*          are generating the correct output on the DAQ device.
*    3. Set the rate.
*    4. Run the function.
*    5. Stop the function when desired.
*
* Steps:
*    1. Create a task.
*    2. Create an Analog Output Voltage channel.
*    3. Create a sinewave with 1000 points and put the data in an
*       array.
*    4. Call the Start function.
*    5. Write one data point from the array (modulo indexed) until
*       the user hits the stop button or an error occurs. The rate is
*       settable to 1 millisecond.
*    6. Call the Clear Task function to clear the Task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal output terminal matches the Physical
*    Channel I/O Control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/
#include <QCoreApplication>
#include <windows.h>
#include "conio.h"
#include "Art_DAQ.h"

#define ArtDAQErrChk(functionCall) if( ArtDAQFailed(error=(functionCall)) ) goto Error;
#define PI	3.1415926535

int main(void)
{
    int32       error=0;
    TaskHandle  taskHandle=0;
    float64     data[1000];
    char        errBuff[2048]={'\0'};
    uInt32      i=0;

    for(;i<1000;i++)
        data[i] = 9.95*sin((double)i*2.0*PI/1000.0);
    i = 0;

    /*********************************************/
    // ArtDAQ Configure Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_CreateTask("",&taskHandle));
    ArtDAQErrChk (ArtDAQ_CreateAOVoltageChan(taskHandle,"Dev1/ao0","",-10.0,10.0,ArtDAQ_Val_Volts,""));

    /*********************************************/
    // ArtDAQ Start Code
    /*********************************************/
    ArtDAQErrChk (ArtDAQ_StartTask(taskHandle));

    printf("Generating samples continuously. Press Ctrl+C to interrupt\n");
    while( 1 ) {
        Sleep(1);

        /*********************************************/
        // ArtDAQ Write Code
        /*********************************************/
        ArtDAQErrChk (ArtDAQ_WriteAnalogF64(taskHandle,1,0,10.0,ArtDAQ_Val_GroupByChannel,&data[i],NULL,NULL));
        if( ++i>=1000 )
            i = 0;
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
        printf("ArtDAQ_ Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
    return 0;
}
