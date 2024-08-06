/********************************************************************************
/  Multi Record Example, uses GenericBinaryAcquisition.h generic file
/*******************************************************************************/
#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <WINDOWS.H>
#include <ART_SCOPE.h>

int main(void)
{
	char fileName[256] = "E:\\5V.pci";
	FileHandle hFile = NULL;
	uInt64 m_Count = 0;
	int m_ChanCount = 0;

	uInt32 m_ConvertCnt = 102400;	// convert point for each channel

	// Call the generic function to perform a continuous acquisition fetching in chunks
	int ret = ArtScope_OpenDataFile(fileName, &hFile, &m_Count, &m_ChanCount);
	if(ret != 0)
		return 0;
	uInt32 convertCount = m_ConvertCnt*m_ChanCount;
	uInt32 retConvertCount = 0;
	uInt64 readCnt = 0;
	float* m_DataBuf = NULL;
	double* m_DataBufD = NULL;
	int bCross = FALSE;
	m_DataBuf = new float[convertCount];
	m_DataBufD = new double[convertCount];
	int tmp = 0;
	__int64 count = m_Count;

	unsigned long tick1 = GetTickCount();
	while(count > 0)
	{
		ret = ArtScope_DataConvertFloat32(hFile, readCnt, convertCount, m_DataBuf, &retConvertCount, bCross);
		//ret = ArtScope_DataConvertFloat64(pDlg->m_hFile, readCnt, convertCount, m_DataBufD, &retConvertCount, bCross);
		if(ret != 0)
			break;
		count -= retConvertCount;
		readCnt += retConvertCount;
	}
	unsigned long tick2 = GetTickCount();
	unsigned long chazhi = tick2 - tick1;
	printf("takes %u(ms) readed points %lu\n", chazhi, readCnt);
	ret = ArtScope_CloseDataFile(hFile);
	if(m_DataBuf != NULL)
	{
		delete[] m_DataBuf;
		m_DataBuf = NULL;
	}
	if(m_DataBufD != NULL)
	{
		delete[] m_DataBufD;
		m_DataBufD = NULL;
	}

	// Wait and exit
	printf("Press any key to exit.\n");
	_getch();
	return 0;
}



/*************************************************************************************\

                              End of example

\*************************************************************************************/
