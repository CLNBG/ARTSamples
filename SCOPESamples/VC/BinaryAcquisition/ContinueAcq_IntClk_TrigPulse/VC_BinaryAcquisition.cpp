/********************************************************************************
/  Continuous Binary Samples Example
/*******************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <ART_SCOPE.h>

int ProcessEvent (int32 *stop);
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
	int32 acquisitionMode = ARTSCOPE_SAMPMODE_COUPLING_CONTINUOUS;	

	// vertical parameters
	int32 verticalRange = ARTSCOPE_RANGE_20Vpp;
	int32 verticalOffset = 0;
	int32 verticalCoupling = ARTSCOPE_VAL_DC;

	// horizontal parameters
	double minSampleRate = 0.0;
	int32 minNumPts = 1000;
	double refPosition = 0.0;

	// pulse trigger parameters
	float triggerLevel = 0;
	char triggerSource[_MAX_PATH];
	int32 triggerSlope = ARTSCOPE_VAL_TRIGDIR_PULSE_GREATER;
	int32 triggerCoupling = ARTSCOPE_VAL_TRIGCOUP_DC;
	int32 triggerPolarity = ARTSCOPE_VAL_TRIGPOLAR_NEGATIVE;
	int32 triggerCount = 1;
	int32 triggerSensitivity = 0;
	int32 pulseWidth = 100;

	// temporary variables
	uInt32 numWfms = 0;
	uInt32 k = 0;
	double timeout = 1.0;

	int32 pointsFetched = 0;
	int32 chunksFetched = 0;

	uInt32  perRecordLength = 0;
	
	ArtScope_wfmInfo wfmInfo;
	unsigned short *waveformPtr = NULL;
	int32 stop = FALSE;
	

	memset(triggerSource, 0, sizeof(triggerSource));
	strcpy(triggerSource, ARTSCOPE_VAL_TRIGSRC_CH0);

	GetDeviceNameFromConsole(resourceName);

	GetParametersFromConsole(channelName, &verticalRange, &minSampleRate, &minNumPts, &refPosition);
	
	handleErr(ArtScope_init(resourceName, &taskHandle));

	handleErr(ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode));

	handleErr(ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, TRUE));

	handleErr(ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minNumPts, refPosition));
	
	handleErr(ArtScope_ConfigureTriggerPulse(taskHandle, triggerSource, triggerLevel, triggerPolarity, triggerSlope, pulseWidth, triggerCoupling, triggerCount, triggerSensitivity));

	handleErr(ArtScope_ActualNumWfms(taskHandle, &numWfms));

	chunksFetched = 0;
	stop = FALSE;

	handleErr(ArtScope_InitiateAcquisition(taskHandle));
	handleErr(ArtScope_ActualRecordLength(taskHandle, &perRecordLength));
	waveformPtr = (unsigned short*)malloc(sizeof(unsigned short) * perRecordLength * numWfms);
	if (waveformPtr == NULL)
		handleErr (ArtSCOPError_AllocateMemoryError);

	handleErr(ArtScope_StartAcquisition(taskHandle));

	while(stop == FALSE )
	{
		handleErr(ArtScope_FetchBinary16(taskHandle, timeout, perRecordLength, waveformPtr, &wfmInfo));

		pointsFetched += wfmInfo.actualSamples;
		chunksFetched++;
		
		printf("Chunks Fetched : %d\tPoints Fetched : %d\n", chunksFetched, pointsFetched);
		for(k=0;k<numWfms;k++)
		{
			printf("%8d\t", waveformPtr[k]);
		}
		printf("\n");

		ProcessEvent(&stop);
	}

Error:
	if (waveformPtr)
		free (waveformPtr);
	
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
		ArtScope_StopAcquisition(taskHandle);
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

	printf("Input range:\n0-20Vpp\n1-10Vpp\n2-2Vpp\n3-40Vpp\n4-16Vpp\n5-8Vpp\n6-4Vpp\n7-1600mVpp\n8-800mVpp\n9-400mVpp\n10-160mVpp\n11-80mVpp\n12-40mVpp\n");
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

int ProcessEvent (int32 *stopPtr)
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
