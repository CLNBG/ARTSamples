#include "ACTS1000/ACTS1000.h"  // 包含驱动程序接口
#include "ADThread.h"
//#############################################################
// 定义用于控制数据位数及量程的常数
#define VOLT_RANGE 1000.0         // 量程（mV）(注意必须带小数)


// 定义其他常量
#define MAX_CHANNEL_COUNT ACTS1000_AD_MAX_CHANNELS
#define MAX_SEGMENT       16
#define MAX_RAM_SIZE (1024*256) // 每通道RAM的最大容量(点或字)

#define MAX_RAM_SIZE_32M (1024*1024*32) // 每通道RAM的最大容量(点或字)

//#############################################################
// Header define
#define PCI_BUS     0x01
#define USB_BUS     0x02
#define ISA_BUS     0x03
#define PC104_BUS   0x04
#define PXI_BUS     0x05
#define PXIE_BUS     0x2012
#define PCIE_BUS     0x2011

#define PRO_MODE	0x01 // 非空采集
#define INT_MODE	0x02 // 中断采集
#define DMA_MODE	0x03 // DMA采集

#define AD_FILE_TYPE 0x00  // 存储AD数据的文件
#define DA_FILE_TYPE 0x01  // 存储DA数据的文件
#define DIO_FILE_TYPE 0x02 // 存储DIO数据的文件

#define DEFAULT_FILE_TYPE  AD_FILE_TYPE
//#define DEFAULT_BUS_TYPE   PXIE_BUS    // 设备类型(PCI)
#define DEFAULT_DEVICE_NUM 0x6666     // 设备编号,程序编号是否为本程序保存的数据文件

#define DEFAULT_VOLTBOTTOM_RANGE -50000
#define DEFAULT_VOLTTOP_RANGE     5000

#define DEFAULT_DATA_WIDTH 0x14     // 设备的精度(14位)

#define HEAD_END_FLAG  0xAA55AA55


#define PCIE8502D_RID		0xD0

#define DATA_MODE 1 // 数字显示
#define WAVE_MODE 2 // 波形显示 
#define SAVE_MODE 3 // 存盘处理

// 数字窗口显示模式
#define DEC_DATAMODE   0 // 十进制显示
#define HEX_DATAMODE   1 // 十六进制显示
#define VOLT_DATAMODE  2 // 电压值显示

typedef struct _FILE_HEADER
{
	LONG HeadSizeBytes;		// 文件头信息长度
	LONG FileType;
	// 该设备数据文件共有的成员
	LONG BusType;				// 设备总线类型(DEFAULT_BUS_TYPE)
	LONG DeviceNum;				// 该应用程序标志(DEFAULT_DEVICE_NUM)
	LONG DeviceType;			// 该设备的编号8514 8504 8512 8502...
	LONG VoltBottomRange;       // 量程下限(mV)
	LONG VoltTopRange;	        // 量程上限(mV)	

	LONG ChannelCount;          // 通道总数
	LONG DataWidth;             // 设备的精度(分辩率)
	LONG bXorHighBit;           // 是否高位求反(为1则求反)     
	ACTS1000_PARA_AD ADPara;     // 保存硬件参数
	ACTS1000_STATUS_AD ADStatus;     // 保存硬件参数
	ACTS1000_AD_MAIN_INFO ADMainInfo;     // 保存硬件参数
	LONG CrystalFreq;           // 晶振频率
	ULONGLONG CreateFTime;		// 创建文件时间
	ULONG nRevisionID;
	BYTE Reserved0[124];		// 结构体整体大小1024字节
	LONG HeadEndFlag;			// 头信息结束位
} FILE_HEADER, * PFILE_HEADER;

//#############################################################
// external declare
extern WORD* ADBuffer[MAX_SEGMENT]; // 缓冲队列
extern WORD* ADDrawBuffer;
extern BOOL	m_bDrawComp;
extern ACTS1000_PARA_AD ADPara;
extern ACTS1000_PARA_AD OldADPara;
extern ACTS1000_STATUS_AD pADStatus;
extern ACTS1000_AD_MAIN_INFO gl_ADMainInfo;
extern WORD gl_wMaxLSB;
extern float gl_fLsbCount;
extern WORD gl_lLsbHalf;
extern int  gl_ScreenVolume;
extern int  m_TriggerVolt;
extern int WavePointX;
extern int CurrentIndex;
extern HANDLE hDevice;
extern CEdit* pCountEdit;
extern BOOL m_BounceStop;
extern BOOL m_AnalyzeCheck;
extern BOOL m_bCreateDevice;
extern int glOffset;
extern int glBufferID;
extern int m_BounceCount;
extern float m_AnalyzeAllCount;
extern UINT m_OverLimitCount;
extern BOOL m_bProgress;
extern float Center, PerY;
extern CRect ClientRect;
extern float middle1;
extern BOOL bDrawing;
extern int nChannelArrayClient[MAX_CHANNEL_COUNT];
extern int DisplayChannel[MAX_CHANNEL_COUNT];
extern BOOL m_bFirstCreateDeviceFile;
extern int  m_ProcessDigitalMode;
extern int  m_ProcessGraphicMode;
extern int  m_AnalyzeChannel;
extern int m_UpperLimit;
extern int m_LowerLimit;
extern ULONGLONG gl_pulDDR2Length;
extern BOOL gl_bRt;
extern ULONGLONG gl_DDR2Lenth;
extern CEdit* pRangeEdit;
extern CEdit* pRatioEdit;
extern int m_ProcessMoveVolt;
extern int nProcessMode;
extern BOOL bBunch;
extern int m_DigitalDataShowMode;
extern SHORT InUserRegion[MAX_SEGMENT][16384];
extern BOOL gl_bAutoCollect;
extern float OutAnalogValve;
extern int ReadIndex;
extern HANDLE g_ArrFileOut;
extern HANDLE hEvent;
extern HANDLE hExitEvent;
extern BOOL gl_bDeviceADRun;
extern BOOL gl_bAutoTrig;
extern BOOL bCreateDevice;
extern ULONGLONG FileLenghtWords;
extern BOOL gl_Event;
extern BOOL gl_bCollected;
extern BOOL m_bCloseFile;
extern int gl_MiddleLsb[MAX_CHANNEL_COUNT];
extern int gl_nSampleMode;
extern BOOL gl_bTileWave;
extern ULONG gl_nSampleLenth;
extern float gl_PerLsbVoltM[MAX_CHANNEL_COUNT];
extern LONG gl_nTriggerPos;
extern ULONG gl_StartAddr;
extern int gl_nChannelCount;
extern int gl_nSaveFileCount;
extern BOOL	gl_bSmlThenSize;

extern ULONGLONG gl_ReadSizeWords;
#define MsgBox AfxMessageBox
CString strMsg;
extern CString g_strFileFullName;
extern ULONG gl_ChannelArrary[MAX_CHANNEL_COUNT];
extern ACTS1000_STATUS_AD gl_ADStatus;
extern ULONG m_nSampRangeCount;
extern ULONG m_nSampRangeVal[8];
extern TCHAR g_strChannelEn[2][64];
extern TCHAR g_strGain[4][64];
extern TCHAR g_strRefGround[3][64];
extern TCHAR g_strAIRange[3][64];
extern TCHAR g_strInputImped[2][64];
extern ULONG gl_CHIdx[MAX_CHANNEL_COUNT];
