/********************************************************************************
/  Synchronization Example
/*******************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <ART_SCOPE.h>

int GetDeviceNameFromConsole (char* resourceName);
int GetSlaveDeviceNameFromConsole(char* resourceName);
int GetParametersFromConsole (char* channel,int32* verticalRange,double* minSampleRate,int32* minNumPts,double* refPosotion);
int DisplayErrorMessageInConsole (int32 error,const char* errorMessage);

DWORD WINAPI ReadThread_Master(LPVOID lpParam);
DWORD WINAPI ReadThread_Slave(LPVOID lpParam);
void WaitForThreadExit(HANDLE handleArr[]);

struct ThreadInfo
{
	TaskHandle hDevice;
	uInt32 nSamplesLength;
	uInt32 nWaveNum;
	double fTimeout;
	unsigned short* dataBuffer;
	ArtScope_wfmInfo* pwfmInfo;
};

int main()
{
	printf("This example will fetch until any key is pressed.\n");
	printf("The screen will be updated everysecond.\n");

	// Call the generic function to perform a continuous acquisition fetching in chunks
	HANDLE hReadThread_Master = INVALID_HANDLE_VALUE;
	HANDLE hReadThread_Slave = INVALID_HANDLE_VALUE;
	HANDLE hThreadArray[2];
	DWORD dwThreadID_Master = 0, dwThreadID_Slave = 0;
	struct ThreadInfo threadInfo_master, threadinfo_slave;

	int32 error = ArtSCOPESuccess;
	char   errorSource[MAX_FUNCTION_NAME_SIZE];
	char   errorMessage[MAX_ERROR_DESCRIPTION] = " ";

	TaskHandle taskHandle_master = NULL;
	TaskHandle taskHandle_slave = NULL;

	char resourceName_master[_MAX_PATH];
	char resourceName_slave[_MAX_PATH];
	char channelName[_MAX_PATH];

	// acquisition mode
	int32 acquisitionMode = ARTSCOPE_SAMPMODE_FINITE;

	// vertical parameters
	int32 verticalRange = ARTSCOPE_RANGE_10Vpp;
	int32 verticalOffset = 0;
	int32 verticalCoupling = ARTSCOPE_VAL_DC;

	// horizontal parameters
	double minSampleRate = 0;
	int32  minRecordLength = 1;
	double refPosition = 0;

	// sychronous trigger source
	char triggerSyncPulseSouce[_MAX_PATH];

	// trigger parameters
	float triggerLevel = 0;
	int32 triggerCoupling = ARTSCOPE_VAL_TRIGCOUP_DC;
	char triggerSource_master[_MAX_PATH];
	char triggerSource_slave[_MAX_PATH];
	int32 triggerSlope = ARTSCOPE_VAL_TRIGDIR_NEGATIVE;
	int32 triggerCount = 1;
	int32 triggerSensitivity = 0;

	// temporary variables
	uInt32 numWfms_master = 0, numWfms_slave = 0;
	uInt32 actualRecordLength_master = 0, actualRecordLength_slave = 0;

	double timeout = 5.0;
	double fVolt = 0;

	int32 pointsFetched = 0;
	int32 chunksFetched = 0;
	
	// Waveforms
	ArtScope_wfmInfo wfmInfo_master, wfmInfo_slave;
	unsigned short *waveformPtr_master = NULL, *waveformPtr_slave = NULL;
	int32 stop = FALSE;

	uInt32 k = 0;
	int p =0;

	memset(triggerSource_master, 0, sizeof(triggerSource_master));
	strcpy(triggerSource_master, ARTSCOPE_VAL_TRIGSRC_CH0);

	memset(triggerSource_slave, 0, sizeof(triggerSource_slave));
	strcpy(triggerSource_slave, ARTSCOPE_VAL_TRIGSRC_SYNC);

	memset(triggerSyncPulseSouce, 0, sizeof(triggerSyncPulseSouce));
	strcpy(triggerSyncPulseSouce, ARTSCOPE_VAL_TRIG_SYNC_0);

	GetDeviceNameFromConsole(resourceName_master);
	GetSlaveDeviceNameFromConsole(resourceName_slave);

	// initialize master card
 	handleErr(ArtScope_init(resourceName_master, &taskHandle_master));
	
	GetParametersFromConsole(channelName, &verticalRange, &minSampleRate, &minRecordLength, &refPosition);

	handleErr(ArtScope_ConfigureAcquisitionMode(taskHandle_master, acquisitionMode));

	handleErr(ArtScope_ConfigureVertical(taskHandle_master, channelName, verticalRange, verticalOffset, verticalCoupling, 1, TRUE));

	handleErr(ArtScope_ConfigureHorizontalTiming(taskHandle_master, minSampleRate, minRecordLength, refPosition));

	handleErr(ArtScope_ConfigureTriggerSyncSrc(taskHandle_master, triggerSyncPulseSouce));
	
	handleErr(ArtScope_ConfigureTriggerEdge(taskHandle_master, triggerSource_master, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity));

	handleErr(ArtScope_ConfigureClock(taskHandle_master, ARTSCOPE_VAL_CLK_IN, 1, 1));

	handleErr(ArtScope_ActualNumWfms(taskHandle_master, &numWfms_master));

	handleErr(ArtScope_InitiateAcquisition(taskHandle_master))
		
	handleErr(ArtScope_ActualRecordLength(taskHandle_master, &actualRecordLength_master))
	if(waveformPtr_master == NULL)
	{
		waveformPtr_master = (unsigned short*)malloc(sizeof(unsigned short) * actualRecordLength_master* numWfms_master);
		if (waveformPtr_master == NULL)
			handleErr (ArtSCOPError_AllocateMemoryError);
	}

	// initialize slave card
	handleErr(ArtScope_init(resourceName_slave, &taskHandle_slave));
	
	handleErr(ArtScope_ConfigureAcquisitionMode(taskHandle_slave, acquisitionMode));
	
	handleErr(ArtScope_ConfigureVertical(taskHandle_slave, channelName, verticalRange, verticalOffset, verticalCoupling, 1, TRUE));
	
	handleErr(ArtScope_ConfigureHorizontalTiming(taskHandle_slave, minSampleRate, minRecordLength, refPosition));
	
	handleErr(ArtScope_ConfigureTriggerSyncSrc(taskHandle_master, triggerSyncPulseSouce));

	handleErr(ArtScope_ConfigureTriggerEdge(taskHandle_slave, triggerSource_slave, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity));
	
	handleErr(ArtScope_ConfigureClock(taskHandle_slave, ARTSCOPE_VAL_CLK_IN, 1, 0));
	
	handleErr(ArtScope_ActualNumWfms(taskHandle_slave, &numWfms_slave));

	handleErr(ArtScope_InitiateAcquisition(taskHandle_slave))

	handleErr(ArtScope_ActualRecordLength(taskHandle_slave, &actualRecordLength_slave))
	if(waveformPtr_slave == NULL)
	{
		waveformPtr_slave = (unsigned short*)malloc(sizeof(unsigned short) * actualRecordLength_slave* numWfms_slave);
		if (waveformPtr_slave == NULL)
			handleErr (ArtSCOPError_AllocateMemoryError);
	}

	// first start slave card
	handleErr(ArtScope_StartAcquisition(taskHandle_slave));
	threadinfo_slave.hDevice = taskHandle_slave;
	threadinfo_slave.nSamplesLength = actualRecordLength_slave;
	threadinfo_slave.nWaveNum = numWfms_slave;
	threadinfo_slave.fTimeout = timeout;
	threadinfo_slave.dataBuffer = waveformPtr_slave;
	threadinfo_slave.pwfmInfo = &wfmInfo_slave;
	// create thread to read data of slave card
	hReadThread_Slave = CreateThread(NULL, 0, ReadThread_Slave, &threadinfo_slave, 0, &dwThreadID_Slave);

	// then start master card
	handleErr(ArtScope_StartAcquisition(taskHandle_master));
	threadInfo_master.hDevice = taskHandle_master;
	threadInfo_master.nSamplesLength = actualRecordLength_master;
	threadInfo_master.nWaveNum = numWfms_master;
	threadInfo_master.fTimeout = timeout;
	threadInfo_master.dataBuffer = waveformPtr_master;
	threadInfo_master.pwfmInfo = &wfmInfo_master;
	// create thread to read data of master card
	hReadThread_Master = CreateThread(NULL, 0, ReadThread_Master, &threadInfo_master, 0, &dwThreadID_Master);

	hThreadArray[0] = hReadThread_Master;
	hThreadArray[1] = hReadThread_Slave;
	
	WaitForThreadExit(hThreadArray);
	
Error:
	if(waveformPtr_master)
	{
		free(waveformPtr_master);
		waveformPtr_master = NULL;
	}
	if(waveformPtr_slave)
	{
		free(waveformPtr_slave);
		waveformPtr_slave = NULL;
	}
	// Wait and exit
	printf("Press any key to exit.\n");
	_getch();
	return 0;
}

void WaitForThreadExit(HANDLE handleArr[])
{
	DWORD dwRet;
	int nIndex = 0;
	DWORD dErrCode;
	MSG msg;
	int wait_count = 2;
	int nExitThreadCount = 0;

	while(1)
	{
		dwRet = MsgWaitForMultipleObjects(wait_count, handleArr, FALSE, INFINITE, QS_ALLINPUT);
		if(dwRet == WAIT_OBJECT_0 + wait_count)
		{
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT || msg.message == WM_CLOSE)
				{
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if(dwRet >= WAIT_OBJECT_0 && dwRet < WAIT_OBJECT_0 + wait_count)
		{
			nExitThreadCount++;
			if(nExitThreadCount < 2)
			{
				printf("one thread exit.\n");
				nIndex = dwRet - WAIT_OBJECT_0;
				handleArr[nIndex] = handleArr[wait_count-1];
				handleArr[wait_count - 1] = NULL;
				wait_count--;
			}
			else
			{
				printf("two threads exit.\n");
				break;
			}
		}
		else
		{
			dErrCode = GetLastError();
			break;
		}
	}
}

DWORD WINAPI ReadThread_Slave(LPVOID lpParam)
{
	int k = 0;
	double fVolt = 0.0;
	struct ThreadInfo *threadParas = (struct ThreadInfo*)lpParam;
	if(NULL == threadParas)
	{
		return 0;
	}
	while(1)
	{
		if(ArtSCOPESuccess != ArtScope_FetchBinary16(threadParas->hDevice, threadParas->fTimeout, threadParas->nSamplesLength, threadParas->dataBuffer, threadParas->pwfmInfo))
		{
			printf("slave card read data error!\n");
			break;
		}
		else
		{
			printf("slave data:\n");
			for(k=0;k<threadParas->nWaveNum;k++)
			{
				printf("%8d\t", threadParas->dataBuffer[k]);
			}
			printf("\n");
			break;
		}
	}
	ArtScope_StopAcquisition(threadParas->hDevice);
	ArtScope_ReleaseAcquisition(threadParas->hDevice);
	ArtScope_Close (threadParas->hDevice);
}

DWORD WINAPI ReadThread_Master(LPVOID lpParam)
{
	int k = 0;
	double fVolt = 0.0;
	struct ThreadInfo *threadParas = (struct ThreadInfo*)lpParam;
	if(NULL == threadParas)
	{
		return 0;
	}
	while(1)
	{
		if(ArtSCOPESuccess != ArtScope_FetchBinary16(threadParas->hDevice, threadParas->fTimeout, threadParas->nSamplesLength, threadParas->dataBuffer, threadParas->pwfmInfo))
		{
			printf("master card read data error!\n");
			break;
		}
		else
		{
			printf("master data:\n");
			for(k=0;k<threadParas->nWaveNum;k++)
			{
				printf("%8d\t", threadParas->dataBuffer[k]);
			}
			printf("\n");
			break;
		}
	}
	ArtScope_StopAcquisition(threadParas->hDevice);
	ArtScope_ReleaseAcquisition(threadParas->hDevice);
	ArtScope_Close (threadParas->hDevice);
}


// Obtain the resource name of the device from the user
int GetDeviceNameFromConsole (char* resourceName)
{
   //Get the device name from the user
   printf("Type the master device name (e.g., Dev1) from DMC: ");
   scanf("%s", resourceName);
   
   return 0;
}

int GetSlaveDeviceNameFromConsole(char* resourceName)
{
	//Get the device name from the user
	printf("Type the slave device name (e.g., Dev1) from DMC: ");
	scanf("%s", resourceName);
	
   return 0;
}

// Obtain the necessary parameters
int GetParametersFromConsole (char* channel,
						int32* verticalRange,
						double* minSampleRate,
						int32* minNumPts,
						double* refPosition)
{
	double	sampleRate;
	strcpy(channel, "0"); // Channel 0
	*verticalRange = 0;

	printf("Type the device sample rate : ");
	scanf("%lf", &sampleRate);
	*minSampleRate = sampleRate;

	printf("Type the min sample number of points : ");
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
