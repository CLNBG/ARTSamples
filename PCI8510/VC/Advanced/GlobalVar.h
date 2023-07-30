
extern CSysApp theApp;

////////////////////////////////////////////////////////////////////////
extern CADStatusView* gl_pADStatusView;
extern CADParaCfgView* gl_pParaCfgView;

extern ULONG	ADBuffer[MAX_SEGMENT_COUNT][MAX_SEGMENT_SIZE/2];// 缓冲队列
extern ULONG	gl_ReadSizeWords;								// 读入的数据长度
extern BOOL		gl_bDeviceADRun;
extern PCI8510_PARA_AD ADPara;
extern PCI8510_STATUS_AD ADStatus;
extern int		gl_nSampleMode;									// 采集模式(1, 查询， 2, 中断， 3， DMA)
extern BOOL		gl_bCreateFile;
extern int		gl_ScreenVolume;								// 设置屏幕显示的量程
extern int		gl_Screen;										// 设置屏幕显示的量程
extern int		gl_InputRange[MAX_CHANNEL_COUNT];				// 各通道设置的电压量程范围
extern int		gl_ADINRange;
extern int		gl_TopTriggerVolt;								// 上限电压
extern int		gl_BottomTriggerVolt;							// 下限电压
extern int		gl_MiddleLsb[MAX_CHANNEL_COUNT];				// 求平移电压时的中间值
extern int		gl_HistMiddleLsb[MAX_CHANNEL_COUNT];			// 求平移电压时的中间值
extern int		gl_nChannelCount;
extern float	gl_PerLsbVolt[MAX_CHANNEL_COUNT];				// 单位LSB的电压值
extern float	gl_AnalyzeAllCount;
extern UINT		gl_OverLimitCount;
extern BOOL		gl_bProcessMoveVolt;							// 为1时, 平移电压
extern BOOL		gl_bHistProcessMoveVolt;						// 为1时, 平移电压
extern BOOL		gl_bCreateDevice;
extern BOOL		gl_bProgress;									// 是否更新进度条

extern int		gl_nProcMode;				// 数据处理方式 1：数字显示  2：波形显示  3：数据存盘
extern int		gl_DigitShowMode;			// 数字窗口显示模式
extern HANDLE	gl_hEvent;					// 采集线程与绘制线程的同步信号
extern HANDLE	gl_hFileObject;
extern BOOL		gl_bCloseFile;
extern HANDLE	gl_hExitEvent;

extern ULONGLONG	gl_FileLenghtWords;
extern int			gl_Offset;				// 当前缓冲段内偏移
extern int			gl_nDrawIndex;			// 绘图段索引

extern BOOL			gl_bDataProcessing;
extern CString		g_strFileFullName;
extern BOOL			gl_bTileWave;
extern BOOL			gl_FirstScreenStop;
extern int			gl_nReadIndex;
extern double gl_VoltRange;