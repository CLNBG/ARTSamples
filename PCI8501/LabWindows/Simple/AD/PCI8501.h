#ifndef _PCI8501_DEVICE_
#define _PCI8501_DEVICE_
//***********************************************************
// ����AD������ʵ��Ӳ������
typedef struct _PCI8501_PARA_AD     
{
	LONG Frequency;         // �ɼ�Ƶ��,��λΪHz, [10, 800000]
	LONG InputRange[8];		// ģ������������ѡ��
	LONG Gains[8];			// ����ѡ��
	LONG M_Length;          // M�γ���(��), �ܵ�ȡֵ��Χ1-32M, ����M��N���Ȳ��ܴ���32M
	LONG N_Length;          // N�γ���(��), �ܵ�ȡֵ��Χ1-32M, ����M��N���Ȳ��ܴ���32M
	LONG TriggerMode;		// ����ģʽѡ��
	LONG TriggerSource;		// ����Դѡ��
	LONG TriggerDir;		// ��������ѡ��(����/���򴥷�)
	LONG TrigLevelVolt;		// ������ƽ(-10000mV--10000mV)
	LONG ClockSource;		// ʱ��Դѡ��(��/��ʱ��Դ)
	LONG OutClockSource;	// ʱ���������Դ
	LONG bClockSourceDir;   // �Ƿ�ʱ���ź������PXI����,=TRUE:�������, =FALSE:��������
} PCI8501_PARA_AD, *PPCI8501_PARA_AD;

//***********************************************************
// AD����PCI8501_PARA_AD�е�InputRange[x]ʹ�õ�Ӳ������ѡ��
const long PCI8501_INPUT_N10000_P10000mV= 0x00; // ��10000mV
const long PCI8501_INPUT_N5000_P5000mV	= 0x01; // ��5000mV
const long PCI8501_INPUT_N2500_P2500mV	= 0x02; // ��2500mV
const long PCI8501_INPUT_0_P10000mV		= 0x03; // 0��10000mV
const long PCI8501_INPUT_0_P5000mV		= 0x04; // 0��5000mV

//***********************************************************
// AD����PCI8501_PARA_AD�е�Gains[x]ʹ�õ�Ӳ������ѡ��
const long PCI8501_GAINS_1MULT			= 0x00; // 1������
const long PCI8501_GAINS_2MULT			= 0x01; // 2������
const long PCI8501_GAINS_5MULT			= 0x02; // 4������
const long PCI8501_GAINS_10MULT			= 0x03; // 8������

//***********************************************************
// ADӲ������PCI8501_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
const long PCI8501_TRIGMODE_MIDL		= 0x00; // �м䴥��
const long PCI8501_TRIGMODE_POST		= 0x01; // �󴥷�
const long PCI8501_TRIGMODE_PRE			= 0x02; // Ԥ����
const long PCI8501_TRIGMODE_DELAY		= 0x03; // Ӳ����ʱ����

//***********************************************************
// ADӲ������PCI8501_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
const long PCI8501_TRIGMODE_SOFT		= 0x00; // �������
const long PCI8501_TRIGSRC_DTR			= 0x01; // ѡ��DTR��Ϊ����Դ
const long PCI8501_TRIGSRC_ATR			= 0x02; // ѡ��ATR��Ϊ����Դ
const long PCI8501_TRIGSRC_TRIGGER	    = 0x03; // Trigger�źŴ��������ڶ࿨ͬ����

//***********************************************************
// ADӲ������PCI8501_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
const long PCI8501_TRIGDIR_NEGATIVE		= 0x00; // �½��ش���
const long PCI8501_TRIGDIR_POSITIVE		= 0x01; // �����ش���
const long PCI8501_TRIGDIR_NEGAT_POSIT	= 0x02; // ���±��ؾ�����

//***********************************************************
// ADӲ������PCI8501_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
const long PCI8501_CLOCKSRC_IN			= 0x00; // ʹ���ڲ�ʱ��
const long PCI8501_CLOCKSRC_OUT			= 0x01; // ʹ���ⲿʱ��

//***********************************************************
// ADӲ������PCI8501_PARA_AD�е�OutClockSourceʱ��Դ��ʹ�õ�ѡ��
const long PCI8501_OUTCLOCKSRC_TRIGGER0	= 0x00; // ѡ��TRIG0����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER1	= 0x01; // ѡ��TRIG1����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER2	= 0x02; // ѡ��TRIG2����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER3	= 0x03; // ѡ��TRIG3����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER4	= 0x04; // ѡ��TRIG4����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER5	= 0x05; // ѡ��TRIG5����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER6	= 0x06; // ѡ��TRIG6����/���ʱ��
const long PCI8501_OUTCLOCKSRC_TRIGGER7	= 0x07; // ѡ��TRIG7����/���ʱ��

//*************************************************************************************
// ����AD������ʵ��Ӳ������
typedef struct _PCI8501_STATUS_AD     
{
	LONG bADEanble;	// AD�Ƿ��Ѿ�ʹ�ܣ�=TRUE:��ʾ��ʹ�ܣ�=FALSE:��ʾδʹ��
	LONG bTrigger;  // AD�Ƿ񱻴�����=TRUE:��ʾ�ѱ�������=FALSE:��ʾδ������
	LONG bComplete; // AD�Ƿ�����ת�������Ƿ������=TRUE:��ʾ�ѽ�����=FALSE:��ʾδ����
	LONG bAheadTrig;// AD�������Ƿ���ǰ��=TRUE:��ʾ��������ǰ��=FALSE:��ʾ������δ��ǰ
	LONG lEndAddr;	// ������ɵĽ�����ַ
} PCI8501_STATUS_AD, *PPCI8501_STATUS_AD;

//***********************************************************
// �û������ӿ�
#ifndef _PCI8501_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## ����ͨ�ú��� #################################
	// �����ڱ��豸�����������
	HANDLE DEVAPI FAR PASCAL PCI8501_CreateDevice(int DeviceID );		// �����豸����
	int DEVAPI FAR PASCAL PCI8501_GetDeviceCount(HANDLE hDevice);			// ȡ���豸��̨��
	BOOL DEVAPI FAR PASCAL PCI8501_ListDeviceDlg(HANDLE hDevice);			// �б�ϵͳ���е����еĸ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI8501_ReleaseDevice(HANDLE hDevice);			// �ر��豸,��ֹ����,���ͷ���Դ

	//####################### AD���ݶ�ȡ���� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
	// AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�
	BOOL DEVAPI FAR PASCAL PCI8501_GetDDR2Length(								// ��ȡDDR2�Ĵ洢����
												HANDLE hDevice,					// �豸����
												PULONG pulDDR2Length);			// DDR2�ĳ���(��λ��MB)

	BOOL DEVAPI FAR PASCAL PCI8501_ADCalibration(								// ADУ׼
												HANDLE hDevice, 				// �豸����		
												LONG InputRange);				// У׼����ѡ��		
    BOOL DEVAPI FAR PASCAL PCI8501_InitDeviceAD(								// ��ʼ���豸��������TRUE��,�豸���̿�ʼ����.
												HANDLE hDevice,					// �豸����
												PPCI8501_PARA_AD pADPara);		// Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL PCI8501_StartDeviceAD(								// �ڳ�ʼ��֮�������豸
												HANDLE hDevice);				// �豸������

	BOOL DEVAPI FAR PASCAL PCI8501_SetDeviceTrigAD(								// ���豸ʹ������󣬲�����������¼���ֻ�д���ԴΪ�������ʱ��Ч��
												HANDLE hDevice);				// �豸������
	
	BOOL DEVAPI FAR PASCAL PCI8501_GetDevStatusAD(
												HANDLE hDevice,					// �豸���
												PPCI8501_STATUS_AD pADStatus);	// AD״̬

	BOOL DEVAPI FAR PASCAL PCI8501_ReadDeviceAD(								// DMA��ʽ��AD����
												HANDLE hDevice,					// �豸���
												PWORD pADBuffer,				// �����ڽ������ݵ��û�������(�������뿪�ٴ���M��N���ֵĿռ�)
												ULONG nReadSizeWords,			// �������M��N�ĳ���
												ULONG ulStartAddr,				// ������RAM�е���ʼ��ַ
												PLONG nRetSizeWords);			// ����ʵ�ʶ�ȡ�����ݳ���
								
    BOOL DEVAPI FAR PASCAL PCI8501_StopDeviceAD(								// �������豸֮����ͣ�豸
												HANDLE hDevice);				// �豸������

	BOOL DEVAPI FAR PASCAL PCI8501_ReleaseDeviceAD(								// �ر�AD�豸,��ֹ����,���ͷ���Դ
												HANDLE hDevice);				// �豸���

	//################# AD��Ӳ�������������� ########################
	BOOL DEVAPI FAR PASCAL PCI8501_SaveParaAD(HANDLE hDevice, PPCI8501_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL PCI8501_LoadParaAD(HANDLE hDevice, PPCI8501_PARA_AD pADPara);
	BOOL DEVAPI FAR PASCAL PCI8501_ResetParaAD(HANDLE hDevice, PPCI8501_PARA_AD pADPara);

	//####################### ����I/O����������� #################################
	// �û�����ʹ��WriteRegisterULong��ReadRegisterULong�Ⱥ���ֱ�ӿ��ƼĴ�������I/O
	// �����������ʹ����������������ʡ�£�������Ҫ�����ļĴ��������λ�����ȣ���ֻ
	// ����VB�����Ե����Բ�����ô�򵥵�ʵ�ָ�������ͨ���Ŀ��ơ�
	BOOL DEVAPI FAR PASCAL PCI8501_GetDeviceDI(				// ȡ�ÿ�����״̬     
												HANDLE hDevice,					// �豸������,����CreateDevice��������								        
												BYTE bDISts[8]);				// ��������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)

    BOOL DEVAPI FAR PASCAL PCI8501_SetDeviceDO(									// ���������״̬
												HANDLE hDevice,					// �豸������,����CreateDevice��������        
												BYTE bDOSts[8]);				// �������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)

	//################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	BOOL DEVAPI FAR PASCAL PCI8501_GetDeviceBar(					// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ
											HANDLE hDevice,			// �豸������,����CreateDevice��������
											PUCHAR pbPCIBar[6]);	// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����
	BOOL DEVAPI FAR PASCAL PCI8501_GetDevVersion(					// ��ȡ�豸�̼�������汾
											HANDLE hDevice,			// �豸������,����CreateDevice��������
											PULONG pulFmwVersion,	// �̼��汾
											PULONG pulDriverVersion);// �����汾
	BOOL DEVAPI FAR PASCAL PCI8501_WriteRegisterByte(				// ���豸��ӳ��Ĵ����ռ�ָ���˿�д�뵥��������
											HANDLE hDevice,			// �豸����
											PUCHAR pbLinearAddr,		// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes,		// ����ڻ���ַ��ƫ��λ��
											BYTE Value);			// ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����
	BOOL DEVAPI FAR PASCAL PCI8501_WriteRegisterWord(				// д˫�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes,  
											WORD Value);
	BOOL DEVAPI FAR PASCAL PCI8501_WriteRegisterULong(				// д�Ľ������ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes,  
											ULONG Value);
	BYTE DEVAPI FAR PASCAL PCI8501_ReadRegisterByte(				// ���뵥�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes);
	WORD DEVAPI FAR PASCAL PCI8501_ReadRegisterWord(				// ����˫�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes);
	ULONG DEVAPI FAR PASCAL PCI8501_ReadRegisterULong(				// �������ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr, 
											ULONG OffsetBytes);

	//################# I/O�˿�ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	// ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI8501_WritePortByte(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes, BYTE Value);
    BOOL DEVAPI FAR PASCAL PCI8501_WritePortWord(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI8501_WritePortULong(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI8501_ReadPortByte(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes);
    WORD DEVAPI FAR PASCAL PCI8501_ReadPortWord(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes);
    ULONG DEVAPI FAR PASCAL PCI8501_ReadPortULong(HANDLE hDevice, PUCHAR pbPort, ULONG offserBytes);

	//########################### �̲߳������� ######################################
	HANDLE DEVAPI FAR PASCAL PCI8501_CreateSystemEvent(void);			// �����ں��¼����󣬹�InitDeviceInt��VB���̵߳Ⱥ���ʹ��
	BOOL DEVAPI FAR PASCAL PCI8501_ReleaseSystemEvent(HANDLE hEvent);	// �ͷ��ں��¼�����

#ifdef __cplusplus
}
#endif

// �Զ������������������
#ifndef _PCI8501_DRIVER_

	
#endif

#endif // _PCI8501_DEVICE_
