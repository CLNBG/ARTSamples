/********************************************************************************
/  Voltage Acquisition Example
/*******************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <SCOPE/ART_SCOPE.h>

int ProcessEvent(int32* stop);
int GetDeviceNameFromConsole(char* resourceName);
int GetParametersFromConsole(char* channel, int32* verticalRange, double* minSampleRate, int32* perRecordLength, double* refPosotion);
int DisplayErrorMessageInConsole(int32 error, const char* errorMessage);

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

	int32 acquisitionMode = ARTSCOPE_SAMPMODE_FINITE;

	int32 verticalRange = ARTSCOPE_RANGE_10Vpp;
	int32 verticalOffset = 0;
	int32 verticalCoupling = ARTSCOPE_VAL_DC;

	double minSampleRate = 0.0;
	int32 minNumPts = 1000;
	double refPosition = 0.0;

	char inputClockSrc[_MAX_PATH];
	int32 freqDivision = 1;

	uInt32 numWfms = 0;
	uInt32 k = 0;
	double timeout = 1.0;

	int32 pointsFetched = 0;
	int32 chunksFetched = 0;

	uInt32  perRecordLength = 0;

	ArtScope_wfmInfo wfmInfo;
	double* waveformPtr = NULL;
	int32 stop = FALSE;

	memset(inputClockSrc, 0, _MAX_PATH);
	strcpy(inputClockSrc, ARTSCOPE_VAL_CLK_IN);

	GetDeviceNameFromConsole(resourceName);

	GetParametersFromConsole(channelName, &verticalRange, &minSampleRate, &minNumPts, &refPosition);

	handleErr(ArtScope_init(resourceName, &taskHandle));

	handleErr(ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode));

	handleErr(ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, TRUE));

	handleErr(ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minNumPts, refPosition));

	handleErr(ArtScope_ConfigureClock(taskHandle, inputClockSrc, freqDivision, 0));

	handleErr(ArtScope_ConfigureTriggerSoftWare(taskHandle));

	handleErr(ArtScope_ActualNumWfms(taskHandle, &numWfms));

	chunksFetched = 0;
	stop = FALSE;

	handleErr(ArtScope_InitiateAcquisition(taskHandle));
	handleErr(ArtScope_ActualRecordLength(taskHandle, &perRecordLength));
	waveformPtr = (double*)malloc(sizeof(double) * perRecordLength * numWfms);
	if (waveformPtr == NULL)
		handleErr(ArtSCOPError_AllocateMemoryError);

	handleErr(ArtScope_StartAcquisition(taskHandle));

	handleErr(ArtScope_FetchVoltage(taskHandle, timeout, perRecordLength, waveformPtr, &wfmInfo));

	pointsFetched += wfmInfo.actualSamples;
	chunksFetched++;

	printf("Chunks Fetched : %d\tPoints Fetched : %d\n", chunksFetched, pointsFetched);
	for (k = 0; k < numWfms; k++)
	{
		printf("%.4f(mV)\t", waveformPtr[k]);
	}
	printf("\n");

	ArtScope_StopAcquisition(taskHandle);

Error:
	if (waveformPtr)
		free(waveformPtr);

	// get error information
	if (error == -1)
		strcpy(errorMessage, "This example only supports 1 channel.");
	else if (error != ArtSCOPESuccess)
		ArtScope_GetErrorString(error, errorMessage, sizeof(errorMessage));
	else
		strcpy(errorMessage, "Acquisition successful!");

	DisplayErrorMessageInConsole(error, errorMessage);

	// stop and release task
	if (taskHandle)
	{
		ArtScope_ReleaseAcquisition(taskHandle);
		ArtScope_Close(taskHandle);
	}

	// Wait and exit
	printf("Press any key to exit.\n");
	_getch();
	return 0;
}


// Obtain the resource name of the device from the user
int GetDeviceNameFromConsole(char* resourceName)
{
	//Get the device name from the user
	printf("Type the device name (e.g., Dev1) from DMC: ");
	scanf("%s", resourceName);

	return 0;
}

// Obtain the necessary parameters
int GetParametersFromConsole(char* channel,
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
int DisplayErrorMessageInConsole(int32 error,
	const char* errorMessage)
{
	printf("%s\n", errorMessage);
	return 0;
}

int ProcessEvent(int32* stopPtr)
{
	// Stop when keyboard is pressed
	if (_kbhit())
	{
		*stopPtr = TRUE;
		_getch();
	}
	else
		*stopPtr = FALSE;
	return 0;
}


/*************************************************************************************\

							  End of example

\*************************************************************************************/
