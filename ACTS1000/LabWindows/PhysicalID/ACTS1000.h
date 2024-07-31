#ifndef _ACTS1000_DEVICE_
#define _ACTS1000_DEVICE_

#define ACTS1000_AD_MAX_CHANNELS	8 // ���������֧��8·ģ��������ͨ���İ忨
//######################## ֧�ְ忨 #################################
// PXIE/PCIE 8502 4CH 12Bit 40Msps
// PXIE/PCIE 8504 4CH 14Bit 40Msps
// PXIE/PCIE 8512 4CH 12Bit 80Msps
// PXIE/PCIE 8514 4CH 14Bit 80Msps
// PXIE/PCIE 8912 2CH 12Bit 250Msps,֧���迹ѡ��
// PXIE/PCIE 8914 2CH 14Bit 250Msps,֧���迹ѡ��
// PXIE/PCIE 8916 2CH 16Bit 250Msps,֧���迹ѡ��
// PXIE/PCIE 8582 8CH 12Bit 100Msps,PFIѡ��(�������/��������),��֧����ʱ��,��֧��ʱ�����
// PXIE/PCIE 8584 8CH 14Bit 100Msps,PFIѡ��(�������/��������),��֧����ʱ��,��֧��ʱ�����
// PXIE/PCIE 8586 8CH 16Bit 100Msps,PFIѡ��(�������/��������),��֧����ʱ��,��֧��ʱ�����

//***********************************************************
// ����AD������ʵ��Ӳ������
typedef struct _ACTS1000_PARA_AD     
{
	LONG bChannelArray[8];	// ����ͨ��ѡ������,�ֱ����8��ͨ��,=TRUE��ʾ��ͨ������,���򲻲���(ֻ֧��3��ͨ������:0 01 0123)
	LONG InputRange[8];		// ģ������������ѡ��
	LONG CouplingType[8];	// �������(ֱ�����, �������)
	LONG InputImped[8];		// �����迹�������У��ֱ����8��ͨ��(���忨8912/8914/8916֧��)
	LONG FreqDivision;      // ��Ƶ��[1, 2147483647],��ʱ��:����Ƶ��=�ⲿʱ��Ƶ��/��Ƶ��;����ʱ��:����Ƶ��=��׼Ƶ��/��Ƶ��;
	LONG SampleMode;		// ����ģʽ(���޲���,��������)
	ULONG M_Length;         // M�γ���(��),��ʱ������Ч
	ULONG N_Length;         // N�γ���(��),���޵������Ч
							// �м䴥��M+NΪ��Ч��ȡ����,��Χ[512/ʹ��ͨ���� , �ڴ��С(�ֽ�)/2/ʹ��ͨ����],��Ϊ��Сֵ������;
							// �󴥷�M����Ϊ0,N*ʹ��ͨ����*������������Ϊ512��������
							// Ԥ����M����Ϊ0,NΪ��Ч��ȡ����,��Χ[512/ʹ��ͨ���� , �ڴ��С(�ֽ�)/2/ʹ��ͨ����],��Ϊ��Сֵ������;
							// Ӳ����ʱ,M��ʾ��ʱ������Χ[0, 4294967295],N*ʹ��ͨ����*������������Ϊ512��������
							
	LONG PFISel;			// PFI����ѡ��, ������泣������(���忨8582 8584֧��)
	LONG TriggerMode;		// ����ģʽѡ��
	LONG TriggerSource;		// ����Դѡ��
	LONG TriggerDir;		// ��������ѡ��(�½���/������/���±��ؾ�����)
	LONG TrigLevelVolt;		// ������ƽ(���̰�ģ����������)
	LONG TrigWindow;		// ������������λnS,����Ϊ������߲����ʵĲ�������;�������80M,������Ϊ12.5nS
	ULONG TrigCount;		// ������������,Ĭ��Ϊ1,Ϊ0ʱ��ʾ���޴���,�˹��ܽ������޲���(�󴥷���Ӳ����ʱ����)ģʽ����Ч
	LONG ReferenceClock;	// �ο�ʱ��ѡ��
	LONG TimeBaseClock;		// ����ʱ��ѡ��
	LONG bMasterEn;			// ���豸ʹ��  
							// =0�����豸,�������豸���͵�ͬ�������ź�
							// =1�����豸,Ϊ���豸��������Ĵ����ź�
							// ע���ڶ�ģ��ͬ��ϵͳ��,ֻ���趨����һ���豸Ϊ���豸,�������趨Ϊ���豸,���ϵͳ��ֻ��һ���豸�����ж���豸���ǲ�Ҫ��ͬ��,�轫�����豸�趨Ϊ���豸
	LONG SyncTrigSignal;	// ͬ������Դ
	LONG bClkOutEn;			// ʱ���Ƿ����,TRUE���
	LONG ClkOutSel;			// ʱ�����ѡ��
	LONG bTrigOutEn;		// �����Ƿ����,TRUE���
	LONG TrigOutPolarity;	// �����������
	LONG TrigOutWidth;		// �������������ȵ�λnS,[50, 50000]����50nS
	BOOL bSaveFile;			// �Ƿ��Զ����� =TRUE:����
	TCHAR chFileName[256];	// ����·��������,��:L"F:\\1.pci"
} ACTS1000_PARA_AD, *PACTS1000_PARA_AD;

//***********************************************************
// AD����ACTS1000_PARA_AD�е�InputRange[X]ʹ�õ�Ӳ������ѡ��
const long ACTS1000_INPUT_N5000_P5000mV		= 0x00; // ��5000mV
const long ACTS1000_INPUT_N1000_P1000mV		= 0x01; // ��1000mV

// AD����ACTS1000_PARA_AD�е�CouplingType���������ʹ�õ�ѡ��
const long ACTS1000_COUPLING_DC				= 0x00;	// ֱ�����
const long ACTS1000_COUPLING_AC				= 0x01;	// �������

// AD����ACTS1000_PARA_AD�е�InputImpedʹ�õ������迹ѡ��
const long ACTS1000_IMPED_1M				= 0x00; // 1M��
const long ACTS1000_IMPED_50				= 0x01; // 75��

// AD����ACTS1000_PARA_AD�е�SampleMode����ģʽ��ʹ�õ�ѡ��
const long ACTS1000_SAMPMODE_FINITE			= 0x00;	// ���޵����
const long ACTS1000_SAMPMODE_CONTINUOUS		= 0x01;	// ��������(�󴥷���Ӳ����ʱ����֧����������,����������������,������һ�κ�һֱ����)


// AD����ACTS1000_PARA_AD�е�PFISel PFI����ѡ����ʹ�õ�ѡ��(��8582 8584�忨֧��)
const long ACTS1000_PFISEL_TRIG_OUT			= 0x00;	// �������
const long ACTS1000_PFISEL_TRIG_IN			= 0x01; // ��������

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
const long ACTS1000_TRIGMODE_MIDL			= 0x00; // �м䴥��:MΪ����ǰ�ɼ�����,NΪ������ɼ�����
const long ACTS1000_TRIGMODE_POST			= 0x01; // �󴥷�:M��Ч,NΪ������ɼ�����
const long ACTS1000_TRIGMODE_PRE			= 0x02; // Ԥ����:M��Ч,NΪ����ǰ�ɼ�����
const long ACTS1000_TRIGMODE_DELAY			= 0x03; // Ӳ����ʱ����:MΪ��������ʱ����,NΪ��ʱ��ɼ�����

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
const long ACTS1000_TRIGMODE_SOFT			= 0x00; // �������(ȱʡֵ)
const long ACTS1000_TRIGSRC_DTR				= 0x01; // �ⲿ���ִ���(DTR/PFI)
const long ACTS1000_TRIGSRC_TRIGGER			= 0x02; // ͬ���źŴ���(���ڶ࿨ͬ��,�����ź�,ֱ�Ӳ�׽����)
const long ACTS1000_TRIGSRC_CH0				= 0x03; // ͨ��0����
const long ACTS1000_TRIGSRC_CH1				= 0x04; // ͨ��1����
const long ACTS1000_TRIGSRC_CH2				= 0x05; // ͨ��2����
const long ACTS1000_TRIGSRC_CH3				= 0x06; // ͨ��3����
const long ACTS1000_TRIGSRC_CH4				= 0x07; // ͨ��4����
const long ACTS1000_TRIGSRC_CH5				= 0x08; // ͨ��5����
const long ACTS1000_TRIGSRC_CH6				= 0x09; // ͨ��6����
const long ACTS1000_TRIGSRC_CH7				= 0x0A; // ͨ��7����

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
const long ACTS1000_TRIGDIR_NEGATIVE		= 0x00; // �½��ش���
const long ACTS1000_TRIGDIR_POSITIVE		= 0x01; // �����ش���
const long ACTS1000_TRIGDIR_NEGAT_POSIT		= 0x02; // ���±��ؾ�����

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�ReferenceClock�ο�ʱ��ѡ����ʹ�õ�ѡ��
const long ACTS1000_RECLK_ONBOARD			= 0x00; // ����ʱ��(Ĭ��)
const long ACTS1000_RECLK_EXT_10M			= 0x01;	// �ⲿ10M��CLK_IN��(�忨8582 8584��֧��)
const long ACTS1000_RECLK_PXI_10M			= 0x02;	// PXIe_CLK10M(PXIE)/����10M(PCIE)
const long ACTS1000_RECLK_PXIe_100M			= 0x03; // PXIe_CLK100M(PCIE���߰忨��֧��)

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�TimeBaseClock����ʱ��ѡ����ʹ�õ�ѡ��
const long ACTS1000_TBCLK_IN				= 0x00; // �ڲ�ʱ��(Ĭ��)
const long ACTS1000_RECLK_EXT				= 0x01;	// �ⲿʱ�ӣ�CLK_IN��(�忨8582 8584��֧��)

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�SyncTrigSignalͬ������Դ��ʹ�õ�ѡ��
const long ACTS1000_STS_TRIGGER0			= 0x00; // ͬ�������ź�TRIG0 
const long ACTS1000_STS_TRIGGER1			= 0x01; // ͬ�������ź�TRIG1 
const long ACTS1000_STS_TRIGGER2			= 0x02; // ͬ�������ź�TRIG2 
const long ACTS1000_STS_TRIGGER3			= 0x03; // ͬ�������ź�TRIG3 
const long ACTS1000_STS_TRIGGER4			= 0x04; // ͬ�������ź�TRIG4 
const long ACTS1000_STS_TRIGGER5			= 0x05; // ͬ�������ź�TRIG5 
const long ACTS1000_STS_TRIGGER6			= 0x06; // ͬ�������ź�TRIG6 
const long ACTS1000_STS_TRIGGER7			= 0x07; // ͬ�������ź�TRIG7 

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�ClkOutSelʱ�����ѡ����ʹ�õ�ѡ��
const long ACTS1000_CLKOUT_REFERENCE		= 0x00; // ����ο�ʱ��
const long ACTS1000_CLKOUT_TIMEBASE			= 0x01; // �����׼ʱ�� 

//***********************************************************
// ADӲ������ACTS1000_PARA_AD�е�TrigOutPolarityʱ�����ѡ����ʹ�õ�ѡ��
const long ACTS1000_TOP_NEGATIVE			= 0x00; // ���������
const long ACTS1000_TOP_POSITIVE			= 0x01; // ��������� 



//*************************************************************************************
// ����AD������ʵ��Ӳ������
typedef struct _ACTS1000_STATUS_AD     
{
	LONG bADEanble;		// AD�Ƿ��Ѿ�ʹ��,=TRUE:��ʾ��ʹ��,=FALSE:��ʾδʹ��
	LONG bTrigger;		// AD�Ƿ񱻴���,=TRUE:��ʾ�ѱ�����,=FALSE:��ʾδ������
	LONG bComplete;		// �Զ����̵������޵������Ƿ����,=TRUE:��ʾ�����,=FALSE:��ʾδ���(���޲�����Ч)
	LONG bAheadTrig;	// AD�Ƿ����,=TRUE:��ʾ�����,=FALSE:��ʾδ���
	ULONGLONG lCanReadPoint;	// ���Զ�ȡ�ĵ���(���޲�����Ч)
	ULONGLONG lSavePoints;// �Զ����̴�С(��)
} ACTS1000_STATUS_AD, *PACTS1000_STATUS_AD;


// #################### AD��Ҫ��Ϣ�ṹ��(ACTS1000_AD_MAIN_INFO) #######################
typedef struct _ACTS1000_AD_MAIN_INFO
{
	LONG nDeviceType;			// ��������\�豸����0x20128514 20118512...2012(PXIE)2011(PCIE)
	LONG nChannelCount;			// ADͨ������
	LONG nDepthOfMemory;		// AD���ش洢�����(MB)
	LONG nSampResolution;		// AD�����ֱ���(��=8��ʾ8Bit; =12��ʾ12Bit; =14��ʾ14Bit; =16��ʾ16Bit)
	LONG nSampCodeCount;		// AD������������(��256, 4096, 16384, 65536)
	LONG nTrigLvlResolution;	// ������ƽ�ֱ���(��=8��ʾ8Bit; =12��ʾ12Bit; =16��ʾ16Bit)
	LONG nTrigLvlCodeCount;		// ������ƽ��������(��256, 4096)
	LONG nBaseRate;				// ��׼Ƶ��Hz
	LONG nMaxRate;				// ���Ƶ��Hz
	LONG nMinFreqDivision;		// ��С��Ƶ��
	LONG nSupportImped;			// �Ƿ�֧�������迹(0:��֧�� 1:֧��)
	LONG nSupportPFI;			// �Ƿ�֧��PFI�����������ѡ��(0:��֧�� 1:֧��)
	LONG nSupportExtClk;		// �Ƿ�֧����ʱ��(0:��֧�� 1:֧��)
	LONG nSupportPXIE100M;		// �Ƿ�֧��PXIE100M(0:��֧�� 1:֧��)
	LONG nSupportClkOut;		// �Ƿ�֧��ʱ�����(0:��֧�� 1:֧��)

	LONG nReserved0;			// �����ֶ�(��δ����)
	LONG nReserved1;			// �����ֶ�(��δ����)
	LONG nReserved2;			// �����ֶ�(��δ����)
	LONG nReserved3;			// �����ֶ�(��δ����)
} ACTS1000_AD_MAIN_INFO, *PACTS1000_AD_MAIN_INFO;

//***********************************************************
// �û������ӿ�
#ifndef _ACTS1000_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## ����ͨ�ú��� #################################
	// �����ڱ��豸�����������
	HANDLE DEVAPI FAR PASCAL ACTS1000_CreateDevice(int DeviceID);		// �����豸����
	HANDLE DEVAPI FAR PASCAL ACTS1000_CreateDeviceEx(LONG DevicePhysID);		// �����豸����(�ú���ʹ������ID���255)
	LONG DEVAPI FAR PASCAL ACTS1000_GetDeviceCount(HANDLE hDevice);			// ȡ���豸��̨��
	BOOL DEVAPI FAR PASCAL ACTS1000_SetDevicePhysID(HANDLE hDevice, LONG DevicePhysID); // ���õ�ǰ�豸������ID��[0:255]
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID); // ȡ�õ�ǰ�豸���߼�ID�ź�����ID��
	BOOL DEVAPI FAR PASCAL ACTS1000_ListDeviceDlg(HANDLE hDevice);			// �б�ϵͳ���е����еĸ�PCI�豸
    BOOL DEVAPI FAR PASCAL ACTS1000_ReleaseDevice(HANDLE hDevice);			// �ر��豸,��ֹ����,���ͷ���Դ

	//####################### AD���ݶ�ȡ���� #################################
	// ���ڴ������ͨ�û�,��Щ�ӿ���򵥡����ݡ���ɿ�,���û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ��,��������ĳ�ʼ���豸�Ͷ�ȡ
	// AD���ݼ��������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�
	BOOL DEVAPI FAR PASCAL ACTS1000_GetMainInfo(								// ���ذ���DDR2��С,��λΪMb
												HANDLE hDevice,					// �豸����
												PACTS1000_AD_MAIN_INFO pMainInfo);		// ���ذ忨��Ϣ
	BOOL DEVAPI FAR PASCAL ACTS1000_ADCalibration(								// ADУ׼(ע��:��֧�������迹Ϊ1MŷУ׼)
												HANDLE hDevice);				// �豸����				
    BOOL DEVAPI FAR PASCAL ACTS1000_InitDeviceAD(								// ��ʼ���豸,������TRUE��,�豸���̿�ʼ����.
												HANDLE hDevice,					// �豸����
												PACTS1000_PARA_AD pADPara);		// Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL ACTS1000_StartDeviceAD(								// �ڳ�ʼ��֮��,�����豸
												HANDLE hDevice);				// �豸������

	BOOL DEVAPI FAR PASCAL ACTS1000_SetDeviceTrigAD(								// ���豸ʹ�������,������������¼�(ֻ�д���ԴΪ�������ʱ��Ч)
												HANDLE hDevice);				// �豸������
	
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDevStatusAD(
												HANDLE hDevice,					// �豸���
												PACTS1000_STATUS_AD pADStatus);	// AD״̬

	BOOL DEVAPI FAR PASCAL ACTS1000_ReadDeviceAD(								// DMA��ʽ��AD����
												HANDLE hDevice,					// �豸���
												PWORD pADBuffer,				// �����ڽ������ݵ��û�������(�������뿪�ٴ���M��N���ֵĿռ�)
												ULONG nReadSizeWords,			// ��ȡ����
												PULONG nRetSizeWords,			// ����ʵ�ʶ�ȡ�����ݳ���
												PULONG pAvailSampsPoints,		// ʣ�����
												double fTimeout);				// ��ʱʱ��
								
    BOOL DEVAPI FAR PASCAL ACTS1000_StopDeviceAD(								// �������豸֮��,��ͣ�豸
												HANDLE hDevice);				// �豸������

	BOOL DEVAPI FAR PASCAL ACTS1000_ReleaseDeviceAD(								// �ر�AD�豸,��ֹ����,���ͷ���Դ
												HANDLE hDevice);				// �豸���

	PVOID DEVAPI FAR PASCAL ACTS1000_AllocateBuffer(							// �����ڴ�
												ULONG ulRequsetSize);			// �����ڴ��С����λ�ֽ�

	BOOL DEVAPI FAR PASCAL ACTS1000_FreeBuffer(									// �ͷ�������ڴ�
												PVOID pData);

	BOOL DEVAPI FAR PASCAL ACTS1000_SetDMAMemorySize(							// ����DMA�ڴ��С
												HANDLE hDevice,					// �豸���
												ULONG DMAMemSize);				// DMA�ڴ��С,��λKB,[512,131072],Ĭ��4096KB
	//################# AD��Ӳ�������������� ########################
	BOOL DEVAPI FAR PASCAL ACTS1000_SaveParaAD(HANDLE hDevice, PACTS1000_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL ACTS1000_LoadParaAD(HANDLE hDevice, PACTS1000_PARA_AD pADPara);
	BOOL DEVAPI FAR PASCAL ACTS1000_ResetParaAD(HANDLE hDevice, PACTS1000_PARA_AD pADPara);

	// ################################ ���뺯�� ################################
	BOOL DEVAPI FAR PASCAL ACTS1000_SetPassword(ULONG nPassword);
	// ################################ �豸���кź��� ################################
	BOOL DEVAPI FAR PASCAL ACTS1000_SetSerialNum(						// �������к�, �ɹ�ʱ����TRUE,���򷵻�FALSE(ACTS1000_SetPassword�ɹ���ſ�����)
												HANDLE hDevice,			// �豸������,����DEV_Create()��������
												ULONG nSerialNum);		// ���к�

	BOOL DEVAPI FAR PASCAL ACTS1000_GetSerialNum(						// ������к�, �ɹ�ʱ����TRUE,���򷵻�FALSE
												HANDLE hDevice,			// �豸������,����DEV_Create()��������
												ULONG* pSerialNum);		// ���ص����к�

	BOOL DEVAPI FAR PASCAL ACTS1000_GetBusInfo(							// ����豸������Ϣ
												HANDLE hDevice,			// �豸������,����DEV_Create()��������
												ULONG* pBusNumber,		// �������ߺ�
												ULONG* pFunctionNumber,	// ���ع��ܺ�
												ULONG* pDeviceNumber);	// �����豸��

	//################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ,���û�����ʹ����Щ�ӿ�����ʵ�֡�
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDeviceBar(					// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ
											HANDLE hDevice,			// �豸������,����CreateDevice��������
											__int64 pbPCIBar[6]);	// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDevVersion(					// ��ȡ�豸�̼�������汾
											HANDLE hDevice,			// �豸������,����CreateDevice��������
											PULONG pulFmwVersion,	// �̼��汾
											PULONG pulDriverVersion);// �����汾
	BOOL DEVAPI FAR PASCAL ACTS1000_WriteRegisterByte(				// ���豸��ӳ��Ĵ����ռ�ָ���˿�д�뵥��������
											HANDLE hDevice,			// �豸����
											__int64 pbLinearAddr,		// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes,		// ����ڻ���ַ��ƫ��λ��
											BYTE Value);			// ��ָ����ַд�뵥�ֽ�����(���ַ�����Ի���ַ��ƫ��λ�þ���)
	BOOL DEVAPI FAR PASCAL ACTS1000_WriteRegisterWord(				// д˫�ֽ�����(����ͬ��)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes,  
											WORD Value);
	BOOL DEVAPI FAR PASCAL ACTS1000_WriteRegisterULong(				// д�Ľ�������(����ͬ��)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes,  
											ULONG Value);
	BYTE DEVAPI FAR PASCAL ACTS1000_ReadRegisterByte(				// ���뵥�ֽ�����(����ͬ��)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes);
	WORD DEVAPI FAR PASCAL ACTS1000_ReadRegisterWord(				// ����˫�ֽ�����(����ͬ��)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes);
	ULONG DEVAPI FAR PASCAL ACTS1000_ReadRegisterULong(				// �������ֽ�����(����ͬ��)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes);

	//################# I/O�˿�ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ,���û�����ʹ����Щ�ӿ�����ʵ�֡�
	// ����Щ������Ҫ�����ڴ�ͳ�豸,��ISA���ߡ����ڡ����ڵ��豸,�������ڱ�PCI�豸
    BOOL DEVAPI FAR PASCAL ACTS1000_WritePortByte(HANDLE hDevice, __int64 pbPort, BYTE Value);
    BOOL DEVAPI FAR PASCAL ACTS1000_WritePortWord(HANDLE hDevice, __int64 pbPort, WORD Value);
    BOOL DEVAPI FAR PASCAL ACTS1000_WritePortULong(HANDLE hDevice, __int64 pbPort, ULONG Value);

    BYTE DEVAPI FAR PASCAL ACTS1000_ReadPortByte(HANDLE hDevice, __int64 pbPort);
    WORD DEVAPI FAR PASCAL ACTS1000_ReadPortWord(HANDLE hDevice, __int64 pbPort);
    ULONG DEVAPI FAR PASCAL ACTS1000_ReadPortULong(HANDLE hDevice, __int64 pbPort);

	//########################### �̲߳������� ######################################
	HANDLE DEVAPI FAR PASCAL ACTS1000_CreateSystemEvent(void);			// �����ں��¼�����,��InitDeviceInt��VB���̵߳Ⱥ���ʹ��
	BOOL DEVAPI FAR PASCAL ACTS1000_ReleaseSystemEvent(HANDLE hEvent);	// �ͷ��ں��¼�����

#ifdef __cplusplus
}
#endif

// �Զ������������������
#ifndef _ACTS1000_DRIVER_

	
#endif

#endif // _ACTS1000_DEVICE_
