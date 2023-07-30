 #include "StdAfx.h"

////////////////////////////////////////////////////////////////////////
CADStatusView*	gl_pADStatusView = NULL;
CADParaCfgView* gl_pParaCfgView = NULL;
LONG ADBuffer[MAX_SEGMENT_COUNT][MAX_SEGMENT_SIZE/2];	// 缓冲队列1024*64
LONG  ADZeroLsb[4][MAX_CHANNEL_COUNT] = {0};					// 零点校准值
SHORT ADCHZeroLsb[MAX_CHANNEL_COUNT];					// 各通道不同量程的零点校准值
ULONG gl_ReadSizeWords;									// 读入的数据长度

BOOL gl_bDeviceADRun = FALSE;
PCI9757_PARA_AD ADPara;
PCI9757_STATUS_AD ADStatus;

int		gl_nSampleMode = 2;					// 采集模式(1非空 2、查询，3、中断， 4、DMA)
BOOL	gl_bCreateFile = FALSE;
int		gl_ScreenVolume;							// 设置屏幕显示的量程
int		gl_InputRange[MAX_CHANNEL_COUNT];			// 各通道设置的电压量程范围
int		gl_TopTriggerVolt;							// 上限电压
int		gl_BottomTriggerVolt;						// 下限电压
int		gl_MiddleLsb[MAX_CHANNEL_COUNT] = {0};		// 求平移电压时的中间值
int		gl_HistMiddleLsb[MAX_CHANNEL_COUNT] = {0};	// 求平移电压时的中间值
int		gl_nChannelCount = MAX_CHANNEL_COUNT;
float	gl_PerLsbVolt[MAX_CHANNEL_COUNT];			// 单位LSB的电压值
float	gl_AnalyzeAllCount;
UINT	gl_OverLimitCount;
BOOL	gl_bProcessMoveVolt;		// 为1时, 平移电压
BOOL	gl_bHistProcessMoveVolt;	// 为1时, 平移电压
BOOL	gl_bCreateDevice = FALSE;
BOOL	gl_bProgress = FALSE;		// 是否更新进度条
int		gl_nProcessMode;			// 数据处理方式 1：数字显示  2：波形显示  3：数据存盘
HANDLE	gl_hEvent;					// 采集线程与绘制线程的同步信号
int		gl_DigitShowMode;			// 数字窗口显示模式
CFile	g_FileOut;
BOOL	gl_bCloseFile;
HANDLE	gl_hExitEvent = NULL;
ULONGLONG gl_FileLenghtWords;
int		gl_Offset=0;				// 当前缓冲段内偏移
int		gl_nDrawIndex = 0;			// 绘图段索引
int		gl_nReadIndex = 0;			// 读数据索引

BOOL	gl_bDataProcessing = FALSE;
CString g_strFileFullName;
BOOL	gl_FirstScreenStop = FALSE;
BOOL gl_bTileWave = TRUE;
int  gl_nProcMode = PROC_MODE_DIGIT;


WORD ISHALF_LSB=0;

