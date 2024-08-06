/********************************************************************************
/  Multi Record Example, uses GenericBinaryAcquisition.h generic file
/*******************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <ART_SCOPE.h>

int GetDeviceNameFromConsole (char* resourceName);
int GetParametersFromConsole (char* channel, int32* verticalRange, double* minSampleRate, int32* minNumPts, double* refPosotion, char* triggerSource, char* filename, int32* bsavefile, int32* acquisitionMode);
int DisplayErrorMessageInConsole (int32 error, const char* errorMessage);

int main(void)
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
	char fileName[256];

	int32 bSaveFile = FALSE;

	// acquisition mode
	int32 acquisitionMode = ARTSCOPE_SAMPMODE_FINITE;

	// vertical parameters
	int32 verticalRange = ARTSCOPE_RANGE_10Vpp;
	int32 verticalCoupling = ARTSCOPE_VAL_DC;
	int32 verticalOffset = 0;

	// horizontal parameters
	double minSampleRate = 0;
	int32  minRecordLength = 1000;
	double refPosition = 0.0;

	// trigger parameters
	float triggerLevel = 0;
	char triggerSource[_MAX_PATH];	
	int32 triggerSlope = ARTSCOPE_VAL_TRIGDIR_NEGATIVE;
	int32 triggerCoupling = ARTSCOPE_VAL_TRIGCOUP_DC;
	int32 triggerCount = 1;
	int32 triggerSensitivity = 0;

	// temporary variables
	uInt32 numWfms;
	uInt32 actualRecordLength = 0;

	double timeout = 5.0;
	double fVolt = 0;

	int32 pointsFetched = 0;
	int32 chunksFetched = 0;
	
	// Waveforms
	ArtScope_wfmInfo wfmInfo;
	ArtScope_wfmInfo filewfmInfo;
	unsigned short *waveformPtr = NULL;
	unsigned char *waveformPtr8 = NULL;

	int32 stop = FALSE;
	int32 ret = 0;
	int32 bFileHeaderSize = 0;
	int32 byteSize = 0;
	int32 historyFileLen = 0;
	unsigned short fileBuf[1024];
	unsigned char fileBuf8[1024];
	double voltData[1024];

	FILE *fp = NULL;

	int k = 0;
	int p =0;
	
	memset(&wfmInfo, 0, sizeof(wfmInfo));
	memset(&filewfmInfo, 0, sizeof(filewfmInfo));

	memset(triggerSource, 0, sizeof(triggerSource));
	strcpy(triggerSource, ARTSCOPE_VAL_TRIGSRC_CH0);

	GetDeviceNameFromConsole(resourceName);

	handleErr(ArtScope_init(resourceName, &taskHandle));
	
	GetParametersFromConsole(channelName, &verticalRange, 
							&minSampleRate, &minRecordLength, &refPosition,
							triggerSource, fileName, &bSaveFile, &acquisitionMode);

	handleErr(ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode));

	handleErr(ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, TRUE));

	handleErr(ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minRecordLength, refPosition));
	
	handleErr(ArtScope_ConfigureTriggerEdge(taskHandle, triggerSource, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity));

	handleErr(ArtScope_AutoSaveFile(taskHandle, bSaveFile, fileName));

	handleErr(ArtScope_ActualNumWfms(taskHandle, &numWfms));

	chunksFetched = 0;
	stop = FALSE;

	handleErr(ArtScope_InitiateAcquisition(taskHandle));
	handleErr(ArtScope_ActualRecordLength(taskHandle, &actualRecordLength))

	if(waveformPtr == NULL)
	{
		waveformPtr = (unsigned short*)malloc(sizeof(unsigned short) * actualRecordLength * numWfms);
		if (waveformPtr == NULL)
			handleErr (ArtSCOPError_AllocateMemoryError);
		/*waveformPtr8 = (unsigned char *)malloc(sizeof(unsigned char)*actualRecordLength * numWfms);
		if(waveformPtr8 == NULL)
			handleErr (ArtSCOPError_AllocateMemoryError);*/
	}

	handleErr(ArtScope_StartAcquisition(taskHandle));

	handleErr(ArtScope_FetchBinary16(taskHandle, timeout, actualRecordLength, waveformPtr, &wfmInfo));
	//handleErr(ArtScope_FetchBinary8(taskHandle, timeout, actualRecordLength, waveformPtr8, &wfmInfo));

	pointsFetched += wfmInfo.actualSamples;
	chunksFetched++;

	for(k=0;k<numWfms;k++)
	{
		printf("%8d\t", waveformPtr[k]);
		//printf("%8d\t", waveformPtr8[k]);
	}
	printf("\n");

	ArtScope_StopAcquisition(taskHandle);

Error:
	// Free all the allocated memory
	if (waveformPtr)
		free (waveformPtr);
	
	// Display messages
	if (error == -1)
		strcpy (errorMessage, "This example only supports 1 channel.");
	else if (error == -2)
		strcpy (errorMessage, "not all data be acquired.");
 	else if (error != ArtSCOPESuccess)
 		ArtScope_GetErrorString (error, errorMessage, sizeof(errorMessage));   
	else
		strcpy (errorMessage, "Acquisition successful!");
	
	DisplayErrorMessageInConsole (error, errorMessage);
	
	// Close the session
	if (taskHandle)
	{
		ArtScope_StopAcquisition(taskHandle);
		ArtScope_ReleaseAcquisition(taskHandle);
		ArtScope_Close (taskHandle);
	}

	if(error < 0)
	{
		printf("Press any key to exit.\n");
		_getch();
		return 0;
	}

	// Analyze the auto-saved file
	// Use this function to get the size of file header,and some information used to convert binary data to voltage data
	ret = ArtScope_GetInfoFromAutoSaveFile(fileName, &bFileHeaderSize, &filewfmInfo);

	// Get the data count in the file
	fp = fopen(fileName, "rb");
	if(fp == NULL)
	{
		printf("Open file error.\n");
		_getch();
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	byteSize = ftell(fp);
	historyFileLen = (byteSize - bFileHeaderSize) / 2;
	//historyFileLen = byteSize - bFileHeaderSize;

	fseek(fp, bFileHeaderSize, SEEK_SET);
	while(historyFileLen > 0)
	{
		if(historyFileLen >= 1024)
		{
			fread(fileBuf, 2, 1024, fp);
			//fread(fileBuf8, 1, 1024, fp);
			for(k=0; k<1024;k++)
			{
				voltData[k] = (filewfmInfo.fLsbs[k%filewfmInfo.channelCount]*(fileBuf[k]&filewfmInfo.wMaxLSB)-filewfmInfo.rangevalue[k%filewfmInfo.channelCount])/1000.0;
			}
			for(k=0;k<1024;k++)
			{
				if(k>0 && (k % filewfmInfo.channelCount == 0))
				{
					printf("\n");
				}
				printf("%8.4f\t",voltData[k]);
			}
			printf("\n");
		}
		else
		{
			fread(fileBuf, 2, historyFileLen, fp);
			//fread(fileBuf8, 1, historyFileLen, fp);
			for(k=0; k<historyFileLen;k++)
			{
				voltData[k] = (filewfmInfo.fLsbs[k%filewfmInfo.channelCount]*(fileBuf[k]&filewfmInfo.wMaxLSB)-filewfmInfo.rangevalue[k%filewfmInfo.channelCount])/1000.0;
			}
			for(k=0;k<historyFileLen;k++)
			{
				if(k>0 && (k % filewfmInfo.channelCount == 0))
				{
					printf("\n");
				}
				printf("%8.4f\t",voltData[k]);
			}
			printf("\n");
		}
		break;
		//historyFileLen = historyFileLen - 1024;
	}
	fclose(fp);

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
						int32* minNumPts,
						double* refPosition,
 						char* triggerSource,
						char* filename,
						int32* bsavefile,
						int32* acquisitionMode)
{
	double	sampleRate;
	int32 samplemode = 0;
	int32 savefile = 0;
	printf("Type the channel name(like 0 or 0,1 or 0-1 or 0:1)\n");
	scanf("%s", channel);

	printf("Type the device vertical range:\n0-20Vpp\n1-10Vpp\n2-2Vpp\n3-40Vpp\n4-16Vpp\n5-8Vpp\n6-4Vpp\n7-1600mVpp\n8-800mVpp\n9-400mVpp\n10-160mVpp\n11-80mVpp\n12-40mVpp:\n");
	scanf("%d", verticalRange);

	printf("Type the device sample rate :");
	scanf("%lf", &sampleRate);
	*minSampleRate = sampleRate;

	//printf("Type the acquisition mode(0-finit 1-continuous):");
	//scanf("%d", &samplemode);
	*acquisitionMode = 0;

	printf("Is Auto Save File?(1-Yes 0-No):");
	scanf("%d", &savefile);
	if(savefile == 1)
	{
		*bsavefile = TRUE;
		printf("Please input the save file path(like C:\\1.pci):");
		scanf("%s", filename);
	}
	else
	{
		*bsavefile = FALSE;
	}

	printf("Type the min sample number points : ");
	scanf("%d", minNumPts);
	
	printf("Type the refPosition(0.0~100.0) : ");
	scanf("%d", refPosition);

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
