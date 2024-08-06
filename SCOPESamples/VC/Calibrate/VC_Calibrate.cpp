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
	
	// task name
	char resourceName[_MAX_PATH];
	
	GetDeviceNameFromConsole(resourceName);
	
	handleErr(ArtScope_init(resourceName, &taskHandle));
	
	handleErr(ArtScope_Calibration(taskHandle));
	
Error:
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
