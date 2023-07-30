#ifndef _PCI8510_DEVICE_
#define _PCI8510_DEVICE_
//#################### ADӲ������PCI8510_PARA_AD���� #####################
// ����AD������ʵ��Ӳ������
typedef struct _PCI8510_PARA_AD     
{	
	LONG Gains[8];			// ģ��������ѡ�����У��ֱ����8��ͨ��
	LONG Frequency;         // �ɼ�Ƶ��, ��λΪHz, [1, 500000]
	LONG InputRange;		// ����ѡ��
	LONG TriggerMode;		// ����ģʽѡ��
	LONG TriggerSource;		// ����Դѡ��
	LONG TriggerType;		// ��������ѡ��(���ش���/���崥��)
	LONG TriggerDir;		// ��������ѡ��(����/���򴥷�)
	LONG TrigLevelVolt;		// ������ƽ(��10000mV)
	LONG ClockSource;		// ����ʱ��ѡ��
	LONG bClockOutput;      // �Ƿ�ʱ���ź����,=TRUE:�������, =FALSE:��ֹ���
	LONG OutTrigSource;		// Trig�ź�����/���Դ
	LONG OutTrigSourceDir;	// Trig�ź�����/�������ѡ��=TRUE:�������, =FALSE:��������
}PCI8510_PARA_AD,*PPCI8510_PARA_AD;

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�Gains[x]ģ�������뷶Χ��ʹ�õ�ѡ��
const long PCI8510_GAINS_1MULT			= 0x00; // 1������
const long PCI8510_GAINS_2MULT			= 0x01; // 2������
const long PCI8510_GAINS_5MULT			= 0x02; // 4������
const long PCI8510_GAINS_10MULT			= 0x03; // 8������

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�InputRangeģ�������뷶Χ��ʹ�õ�ѡ��
const long PCI8510_INPUT_0_P5000mV		= 0x00; // 0��5000mV
const long PCI8510_INPUT_0_P10000mV		= 0x01; // 0��10000mV
const long PCI8510_INPUT_N5000_P5000mV	= 0x02; // ��5000mV
const long PCI8510_INPUT_N10000_P10000mV= 0x03; // ��10000mV

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
const long PCI8510_TRIGMODE_SOFT		= 0x00; // ����ڴ���
const long PCI8510_TRIGMODE_POST		= 0x01; // Ӳ���󴥷�

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
const long PCI8510_TRIGSRC_ATR			= 0x00; // ѡ���ⲿATR����Դ
const long PCI8510_TRIGSRC_DTR			= 0x01; // ѡ���ⲿDTR����Դ

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�OutTrigSourceʱ��Դ��ʹ�õ�ѡ��
const long PCI8510_OUTTRIGSRC_TRIGGER0	= 0x00; // ѡ��TRIG0���
const long PCI8510_OUTTRIGSRC_TRIGGER1	= 0x01; // ѡ��TRIG1���
const long PCI8510_OUTTRIGSRC_TRIGGER2	= 0x02; // ѡ��TRIG2���
const long PCI8510_OUTTRIGSRC_TRIGGER3	= 0x03; // ѡ��TRIG3���
const long PCI8510_OUTTRIGSRC_TRIGGER4	= 0x04; // ѡ��TRIG4���
const long PCI8510_OUTTRIGSRC_TRIGGER5	= 0x05; // ѡ��TRIG5���
const long PCI8510_OUTTRIGSRC_TRIGGER6	= 0x06; // ѡ��TRIG6���
const long PCI8510_OUTTRIGSRC_TRIGGER7	= 0x07; // ѡ��TRIG7���
const long PCI8510_OUTCLOCKSRC_CLKIN	= 0x08; // ѡ���ⲿ����ʱ��Դ

// ADӲ������PCI8510_PARA_AD�е�TriggerType����������ʹ�õ�ѡ��
const long PCI8510_TRIGTYPE_EDGE		= 0x00; // ���ش���
const long PCI8510_TRIGTYPE_LEVELVOLT	= 0x01; // ��ƽ����

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
const long PCI8510_TRIGDIR_NEGATIVE		= 0x00; // ���򴥷�(�͵�ƽ/�½��ش���)
const long PCI8510_TRIGDIR_POSITIVE		= 0x01; // ���򴥷�(�ߵ�ƽ/�����ش���)
const long PCI8510_TRIGDIR_POSIT_NEGAT	= 0x02; // �����򴥷�(��/�͵�ƽ������/�½��ش���)

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
const long PCI8510_CLOCKSRC_IN			= 0x00; // �ڲ�ʱ��
const long PCI8510_CLOCKSRC_OUT			= 0x01; // �ⲿʱ��

//*************************************************************************************
// ����AD������ʵ��Ӳ������
typedef struct _PCI8510_STATUS_AD     
{
	LONG bNotEmpty;		// ����FIFO�洢���ķǿձ�־��=TRUE�ǿգ� = FALSE ��
	LONG bHalf;			// ����FIFO�洢���İ�����־��=TRUE�������ϣ� = FALSE ��������
	LONG bOverflow;		// ����FIFO�洢���Ķ�̬�����־��= TRUE�ѷ�������� = FALSE��δ�������
} PCI8510_STATUS_AD, *PPCI8510_STATUS_AD;

const int MAX_SEGMENT_COUNT = 64;
typedef struct _PCI8510_STATUS_DMA     
{
	LONG iCurSegmentID;								// ��ǰ�λ���ID,��ʾDMA���ڴ���Ļ�������
	LONG bSegmentSts[MAX_SEGMENT_COUNT];			// �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
	LONG bBufferOverflow;							// �������״̬
} PCI8510_STATUS_DMA, *PPCI8510_STATUS_DMA;

//***********************************************************
// CreateFileObject���õ��ļ�������ʽ������(��ͨ����ָ��ʵ�ֶ��ַ�ʽ������)
const long PCI8510_modeRead					= 0x0000;	// ֻ���ļ���ʽ
const long PCI8510_modeWrite				= 0x0001;	// ֻд�ļ���ʽ
const long PCI8510_modeReadWrite			= 0x0002;	// �ȶ���д�ļ���ʽ
const long PCI8510_modeCreate				= 0x1000;	// ����ļ�������Դ������ļ���������ڣ����ؽ����ļ�������0
const long PCI8510_typeText					= 0x4000;	// ���ı���ʽ�����ļ�

//***********************************************************
// �û������ӿ�
#ifndef _PCI8510_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## �豸��������� ##############################
	HANDLE DEVAPI FAR PASCAL PCI8510_CreateDevice(int DeviceLgcID = 0);     // ���߼��Ŵ����豸����
	int DEVAPI FAR PASCAL PCI8510_GetDeviceCount(HANDLE hDevice); // ȡ���豸��̨��
	BOOL DEVAPI FAR PASCAL PCI8510_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID);
    BOOL DEVAPI FAR PASCAL PCI8510_ReleaseDevice(HANDLE hDevice); // ���ͷ��豸����

	//####################### AD���ݶ�ȡ���� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
	// AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�

	// AD�����ѯ��ʽ����
	BOOL DEVAPI FAR PASCAL PCI8510_InitDeviceProAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									HANDLE hDevice,			// �豸����,����CreateDevice��������
									PPCI8510_PARA_AD pADPara); // Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL PCI8510_StartDeviceProAD(		// �ڳ�ʼ��֮�������豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8510_ReadDeviceProAD_Npt(		// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(����ǿշ�ʽ)
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									ULONG ADBuffer[],		// ����ԭʼAD���ݵ��û�������
									ULONG nReadSizeWords,	// �����ƫ�Ƶ���������ݳ���(��)
									PLONG nRetSizeWords);   // ����ʵ�ʶ�ȡ�ĳ���(��)

	BOOL DEVAPI FAR PASCAL PCI8510_GetDevStatusProAD(		// ��AD����������ȡ���豸�ĸ���״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									PPCI8510_STATUS_AD pADStatus); // AD�ĸ�����Ϣ�ṹ��

	BOOL DEVAPI FAR PASCAL PCI8510_ReadDeviceProAD_Half(	// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(���������ʽ)
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									ULONG ADBuffer[],		// ����ԭʼAD���ݵ��û�������
									ULONG nReadSizeWords,	// �����ƫλ����������ݳ���(��)
									PLONG nRetSizeWords);	// ����ʵ�ʶ�ȡ�ĳ���(��)
		
    BOOL DEVAPI FAR PASCAL PCI8510_StopDeviceProAD(			// �������豸֮����ͣ�豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8510_ReleaseDeviceProAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
									HANDLE hDevice);		// �豸���,����CreateDevice��������

	// ADֱ���ڴ�(DMA)��ʽ����
	BOOL DEVAPI FAR PASCAL PCI8510_InitDeviceDmaAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									HANDLE hDevice,			// �豸����,����CreateDevice��������
									HANDLE hDmaEvent,		// DMA�¼����,����CreateSystemEvent����
									ULONG ADBuffer[],		// �û�������,���Ϊ��ά����
									ULONG nReadSizeWords,	// ÿ��DMAʱ,�û���ָ������Ӧ��ȡ��ʵ�ʳ���(ҪС�ڻ����nSegmentSizeWords)
									LONG nSegmentCount,		// ����ֶε�����,ȡֵ��ΧΪ2-128
									ULONG nSegmentSizeWords,// �������ֶεĶγ�(�������FIFO��������)
									PPCI8510_PARA_AD pADPara);	// Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL PCI8510_StartDeviceDmaAD(		// �ڳ�ʼ��֮�������豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8510_GetDevStatusDmaAD(		// ��AD����������ȡ��DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									PPCI8510_STATUS_DMA pDMAStatus); // ��õ�DMA����״̬

	BOOL DEVAPI FAR PASCAL PCI8510_SetDevStatusDmaAD(		// ��AD��������������DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									LONG iClrBufferID);		// Ҫ����Ļ�����ID,������Ϊ0
		
    BOOL DEVAPI FAR PASCAL PCI8510_StopDeviceDmaAD(			// �������豸֮����ͣ�豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8510_ReleaseDeviceDmaAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
									HANDLE hDevice);		// �豸���,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8510_ADCalibration(			// �豸У׼����
									HANDLE hDevice);		// �豸������
		
	//##################### AD��Ӳ�������������� ###########################
	BOOL DEVAPI FAR PASCAL PCI8510_SaveParaAD(HANDLE hDevice, PPCI8510_PARA_AD pADPara); // ����ǰ��AD��������������ϵͳ��
    BOOL DEVAPI FAR PASCAL PCI8510_LoadParaAD(HANDLE hDevice, PPCI8510_PARA_AD pADPara); // ��AD����������ϵͳ�ж���
    BOOL DEVAPI FAR PASCAL PCI8510_ResetParaAD(HANDLE hDevice, PPCI8510_PARA_AD pADPara); // ��AD���������ָ�������Ĭ��ֵ

	//####################### ����I/O����������� #################################
	// �û�����ʹ��WriteRegisterULong��ReadRegisterULong�Ⱥ���ֱ�ӿ��ƼĴ�������I/O
	// �����������ʹ����������������ʡ�£�������Ҫ�����ļĴ��������λ�����ȣ���ֻ
	// ����VB�����Ե����Բ�����ô�򵥵�ʵ�ָ�������ͨ���Ŀ��ơ�
    BOOL DEVAPI FAR PASCAL PCI8510_SetDeviceDO(				// ���������״̬
										HANDLE hDevice,		// �豸���								        
										BYTE bDOSts[8]);	// ����״̬

	BOOL DEVAPI FAR PASCAL PCI8510_GetDeviceDI(				// ȡ�ÿ�����״̬     
										HANDLE hDevice,		// �豸���								        
										BYTE bDISts[8]);	// ����״̬	
	
	BOOL DEVAPI FAR PASCAL PCI8510_RetDeviceDO(				// �ض�������״̬
										HANDLE hDevice,		// �豸���	 
										BYTE bDOSts[8]);	// ����״̬

	//################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	BOOL DEVAPI FAR PASCAL PCI8510_GetDeviceBar(			// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									__int64  pbPCIBar[6]);	// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����

	BOOL DEVAPI FAR PASCAL PCI8510_GetDevVersion(			// ��ȡ�豸�̼�������汾
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									PULONG pulFmwVersion,	// �̼��汾
									PULONG pulDriverVersion);// �����汾

    BOOL DEVAPI FAR PASCAL PCI8510_WriteRegisterByte(		// ��ָ���Ĵ����ռ�λ��д�뵥��������
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									__int64 pbLinearAddr,	// ָ���Ĵ��������Ի���ַ,������GetDeviceAddr�е�LinearAddr��������ֵ
									ULONG OffsetBytes,		// ��������Ի���ַ����ַ��ƫ��λ��(�ֽ�)
									BYTE Value);			// ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����

	BOOL DEVAPI FAR PASCAL PCI8510_WriteRegisterWord(		// д˫�ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes,  
									WORD Value);

	BOOL DEVAPI FAR PASCAL PCI8510_WriteRegisterULong(		// д�Ľ������ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes,  
									ULONG Value);

	BYTE DEVAPI FAR PASCAL PCI8510_ReadRegisterByte(		// ���뵥�ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes);

	WORD DEVAPI FAR PASCAL PCI8510_ReadRegisterWord(		// ����˫�ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes);

	ULONG DEVAPI FAR PASCAL PCI8510_ReadRegisterULong(		// �������ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes);

	//################# I/O�˿�ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	// ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI8510_WritePortByte(			// 8λд�˿�
									HANDLE hDevice,			// �豸���(��CreateDevice����)
									__int64 pbPort,			// ָ���Ĵ������������ַ
									ULONG OffsetBytes,		// ������������ַ��ƫ��λ��(�ֽ�)
									BYTE Value);			// д����8λ��������
    BOOL DEVAPI FAR PASCAL PCI8510_WritePortWord(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI8510_WritePortULong(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI8510_ReadPortByte(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes);
    WORD DEVAPI FAR PASCAL PCI8510_ReadPortWord(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes);
    ULONG DEVAPI FAR PASCAL PCI8510_ReadPortULong(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes);
	// �����Ҫ���û�ģʽ�£�ֱ�ӷ���Ӳ���˿ڣ��밲װ��ʹ��ISA\CommUser�µ�����ReadPortByteEx�Ⱥ���

	//########################### �̲߳������� ######################################
	HANDLE DEVAPI FAR PASCAL PCI8510_CreateSystemEvent(void); // �����ں��¼����󣬹�InitDeviceDmaAD��VB���̵߳Ⱥ���ʹ��
	BOOL DEVAPI FAR PASCAL PCI8510_ReleaseSystemEvent(HANDLE hEvent); // �ͷ��ں��¼�����
#ifdef __cplusplus
}
#endif

//***********************************************************
// ����FIFO�洢���ĳ���(��)
const long FIFO_IDT7202_LENGTH			= 1024;
const long FIFO_IDT7203_LENGTH			= 2048;
const long FIFO_IDT7204_LENGTH			= 4096;
const long FIFO_IDT7205_LENGTH			= 8192;
const long FIFO_IDT7206_LENGTH			= 16384;
const long FIFO_IDT7207_LENGTH			= 32768;

//***********************************************************
// �豸��ַ��ȡ����GetDeviceAddr�Ĳ���RegisterID��ʹ�õ�ѡ��(��Ч����)
const long PCI8510_REG_MEM_PLXCHIP		= 0x00; // 0�żĴ�����ӦPLXоƬ��ʹ�õ��ڴ�ģʽ����ַ(ʹ��LinearAddr)
const long PCI8510_REG_IO_PLXCHIP		= 0x01; // 1�żĴ�����ӦPLXоƬ��ʹ�õ�IOģʽ����ַ(ʹ��PhysAddr)

// �Զ������������������
#ifndef _PCI8510_DRIVER_
#ifndef _WIN64
#pragma comment(lib, "PCI8510_32.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with PCI8510_32.dll...")
#pragma message("======== Successfully linked with PCI8510_32.dll")
#else
#pragma comment(lib, "PCI8510_64.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with PCI8510_64.dll...")
#pragma message("======== Successfully linked with PCI8510_64.dll")
#endif	
#endif

#endif // _PCI8510_DEVICE_