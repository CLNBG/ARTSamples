#include "stdafx.h"
#include "Sys.h"
#include "TransData.h"
#include "ADWaveView.h"

// ��Ҫ���ھ��ȷ��������Ǳ�׼���ݸ�ʽת��Ϊ��׼���ݸ�ʽ(�����Ը�ʽ),���ذ���ֵ(��16λ����32768, 14λΪ8192)
int TransToStdDataAD(SHORT DataDestBuffer[], SHORT DataSrcBuffer[], int iMiddleLsb, int iTriggerLsb, int Size) 
{	
	for (int i = 0; i < Size; i ++)
	{		
		DataDestBuffer[i] = (int)(DataSrcBuffer[i]-AD_LSB_HALF - iMiddleLsb);
		if (DataDestBuffer[i] > iTriggerLsb)
		{
			gl_OverLimitCount++;
		}
		if (DataDestBuffer[i] < (-iTriggerLsb))
		{
			gl_OverLimitCount++;
		}
	}
	return AD_LSB_HALF;
}

