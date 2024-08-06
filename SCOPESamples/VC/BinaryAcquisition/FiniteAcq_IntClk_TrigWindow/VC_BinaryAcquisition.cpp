/********************************************************************************
/  Continuous Binary Samples Example
/*******************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <ART_SCOPE.h>

int GetDeviceNameFromConsole (char* resourceName);
int GetParametersFromConsole (char* channel,int32* verticalRange,double* minSampleRate,int32* perRecordLength,double* refPosotion);
int DisplayErrorMessageInConsole (int32 error,const char* errorMessage);

int main()
{
	printf("This example will fetch until any key is pressed.\n");
	printf("The screen will be updated everysecond.\n");

	// Call the generic function to perform a continuous acquisition fetching in chunks
	int32 error = ArtSCOPESuccess;
	char   errorSource[MAX_FUNCTION_NAME_SIZE];
	char   errorMessage[MAX_ERROR_DESCRIPTION] = " ";
	TaskHandle taskHandle = NULL;

	char resourceName[_MAX_PATH];
	char channelName[_MAX_PATH];

	// acquisition mode
	int32 acquisitionMode = ARTSCOPE_SAMPMODE_FINITE;	

	// vertical parameters
	int32 verticalRange = ARTSCOPE_RANGE_10Vpp;
	int32 verticalOffset = 0;
	int32 verticalCoupling = ARTSCOPE_VAL_DC;

	// horizontal parameters
	double minSampleRate = 0.0;
	int32 minNumPts = 1000;
	double refPosition = 0.0;

	// window trigger parameters
	double triggerHighLevel = 0;
	double triggerLowLevel = 0;
	char triggerSource[_MAX_PATH];
	int32 windowMode = ARTSCOPE_VAL_WINDOW_ENTERING;
	int32 triggerSlope = ARTSCOPE_VAL_TRIGDIR_NEGATIVE;
	int32 triggerCoupling = ARTSCOPE_VAL_TRIGCOUP_DC;
	int32 triggerCount = 1;
	int32 triggerSensitivity = 0;

	// temporary variables
	uInt32 numWfms = 0;
	uInt32 k = 0;
	double timeout = 1.0;

	int32 pointsFetched = 0;
	int32 chunksFetched = 0;

	uInt32  perRecordLength = 0;
	
	ArtScope_wfmInfo wfmInfo;
	unsigned short *waveformPtr = NULL;
	unsigned char *waveformPtr8 = NULL;
	int32 stop = FALSE;

	memset(triggerSource, 0, sizeof(triggerSource));
	strcpy(triggerSource, ARTSCOPE_VAL_TRIGSRC_CH0);

	GetDeviceNameFromConsole(resourceName);

	GetParametersFromConsole(channelName, &verticalRange, &minSampleRate, &minNumPts, &refPosition);
	
	handleErr(ArtScope_init(resourceName, &taskHandle));

	handleErr(ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode));

	handleErr(ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, TRUE));

	handleErr(ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minNumPts, refPosition));
	
	handleErr(ArtScope_ConfigureTriggerWindow(taskHandle, triggerSource, triggerLowLevel, triggerHighLevel, windowMode, triggerCoupling, triggerCount, triggerSensitivity));

	handleErr(ArtScope_ActualNumWfms(taskHandle, &numWfms));

	chunksFetched = 0;
	stop = FALSE;

	handleErr(ArtScope_InitiateAcquisition(taskHandle));
	handleErr(ArtScope_ActualRecordLength(taskHandle, &perRecordLength));

	waveformPtr = (unsigned short*)malloc(sizeof(unsigned short) * perRecordLength * numWfms);
	if (waveformPtr == NULL)
		handleErr (ArtSCOPError_AllocateMemoryError);

	//waveformPtr8 = malloc(sizeof(BYTE) * perRecordLength * numWfms);
	//if (waveformPtr8 == NULL)
	//	handleErr (ArtSCOPError_AllocateMemoryError);

	handleErr(ArtScope_StartAcquisition(taskHandle));

	handleErr(ArtScope_FetchBinary16(taskHandle, timeout, perRecordLength, waveformPtr, &wfmInfo));
	//handleErr(ArtScope_FetchBinary8(taskHandle, timeout, perRecordLength, waveformPtr8, &wfmInfo));

	pointsFetched += wfmInfo.actualSamples;
	chunksFetched++;
	
	printf("Chunks Fetched : %d\tPoints Fetched : %d\n", chunksFetched, pointsFetched);
	for(k=0;k<numWfms;k++)
	{
		printf("%8d\t", waveformPtr[k]);
		//printf("%8d\t", waveformPtr8[k]);
	}
	printf("\n");
	ArtScope_StopAcquisition(taskHandle);

Error:
	if (waveformPtr)
		free (waveformPtr);

	//if (waveformPtr8)
	//	free (waveformPtr8);
	
	// get error information
	if (error == -1)
		strcpy (errorMessage, "This example only supports 1 channel.");
 	else if (error != ArtSCOPESuccess)
 		ArtScope_GetErrorString (error, errorMessage, sizeof(errorMessage));   
	else
		strcpy (errorMessage, "Acquisition successful!");
	
	DisplayErrorMessageInConsole (error, errorMessage);
	
	// stop and release task
	if (taskHandle)
	{
		ArtScope_ReleaseAcquisition(taskHandle);
		ArtScope_Close (taskHandle);
	}

	// Wait and exit
	printf("Press any key to exit.\n");
	_getch();
	return 0;
}


// Obtain the resource name of the device from the user
int GetDeviceNameFromConsole (char* resourceName)
{
   //Get the device name from the user
   printf("Type the device name (e.g., Dev1) from DMC: ");
   scanf("%s", resourceName);
   
   return 0;
}

// Obtain the necessary parameters
int GetParametersFromConsole (char* channel,
						int32* verticalRange,
						double* minSampleRate,
						int32* perRecordLength,
						double* refPosition)
{
	double	sampleRate;

	printf("Enter channel name(like 0 or 0,1): ");
	scanf("%s", channel);

	printf("Input range:(0-20Vpp,1-10Vpp,2-2Vpp,3-40Vpp,4-16Vpp,5-8Vpp,6-4Vpp,7-1600mVpp,8-800mVpp,9-400mVpp,10-160mVpp,11-80mVpp,12-40mVpp): ");
	scanf("%d", verticalRange);

	// Get the sample rate
	printf("Input sample rate: ");
	scanf("%lf", &sampleRate);
	*minSampleRate = sampleRate;

 	printf("Input sample length:");
 	scanf("%d", perRecordLength);

	printf("Input trigger position(0-post triggger;1-pre trigger;2-middle trigger):");
 	scanf("%f", refPosition);

	return 0;
}

// Display message - do a printf
int DisplayErrorMessageInConsole (int32 error,
                              const char* errorMessage)
{
   printf("%s\n",errorMessage);
   return 0;
}


/*************************************************************************************\

                              End of example

\*************************************************************************************/
