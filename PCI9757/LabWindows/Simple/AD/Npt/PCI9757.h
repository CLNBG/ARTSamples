#ifndef _PCI9757_DEVICE_
#define _PCI9757_DEVICE_
//#################### ADӲ������PCI9757_PARA_AD���� #####################
// ����AD������ʵ��Ӳ������
typedef struct _PCI9757_PARA_AD     
{
	LONG bChannelArray[4];	// ����ͨ��ѡ�����У��ֱ����4��ͨ����=TRUE��ʾ��ͨ�����������򲻲���
	LONG InputRange[4];		// ģ������������ѡ�����У��ֱ����4��ͨ��
	LONG Frequency;         // �ɼ�Ƶ��, ��λΪHz, [3, 800000]
	LONG TriggerMode;		// ����ģʽѡ��
	LONG TriggerSource;		// ����Դѡ��
	LONG TriggerType;		// ��������ѡ��(���ش���/���崥��)
	LONG TriggerDir;		// ��������ѡ��(����/���򴥷�)
	LONG TrigLevelVolt;		// ������ƽ(0��10000mV)
	LONG TrigWindow;		// ����������[1, 65535], ��λ25����
	LONG ClockSource;		// ʱ��Դѡ��(��/��ʱ��Դ)
} PCI9757_PARA_AD, *PPCI9757_PARA_AD;

//***********************************************************
// ADӲ������PCI9757_PARA_AD�е�InputRange[x]ģ�������뷶Χ��ʹ�õ�ѡ��
const long PCI9757_INPUT_N10000_P10000mV= 0x00; // ��10000mV
const long PCI9757_INPUT_N5000_P5000mV	= 0x01; // ��5000mV
const long PCI9757_INPUT_N2500_P2500mV	= 0x02; // ��2500mV
const long PCI9757_INPUT_0_P10000mV		= 0x03; // 0��10000mV
const long PCI9757_INPUT_0_P5000mV		= 0x04; // 0��5000mV

//***********************************************************
// ADӲ������PCI9757_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
const long PCI9757_TRIGMODE_SOFT		= 0x00; // ����ڴ���
const long PCI9757_TRIGMODE_POST		= 0x01; // Ӳ���󴥷�

//***********************************************************
// ADӲ������PCI9757_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
const long PCI9757_TRIGSRC_ATR			= 0x00; // ѡ���ⲿATR����Դ
const long PCI9757_TRIGSRC_DTR			= 0x01; // ѡ���ⲿDTR����Դ
const long PCI9757_TRIGSRC_PCI_TRIG0	= 0x02; // CONVST_IN�źŴ���Դ

// ADӲ������PCI9757_PARA_AD�е�TriggerType����������ʹ�õ�ѡ��
const long PCI9757_TRIGTYPE_EDGE		= 0x00; // ���ش���
const long PCI9757_TRIGTYPE_PULSE		= 0x01; // ���崥��(��ƽ)

//***********************************************************
// ADӲ������PCI9757_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
const long PCI9757_TRIGDIR_NEGATIVE		= 0x00; // ���򴥷�(������/�½��ش���)
const long PCI9757_TRIGDIR_POSITIVE		= 0x01; // ���򴥷�(������/�����ش���)
const long PCI9757_TRIGDIR_POSIT_NEGAT	= 0x02; // �����򴥷�(��/�����������/�½��ش���)

//***********************************************************
// ADӲ������PCI9757_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
const long PCI9757_CLOCKSRC_IN			= 0x00; // �ڲ�ʱ��
const long PCI9757_CLOCKSRC_OUT			= 0x01; // �ⲿʱ��

//*************************************************************************************
// ����AD������ʵ��Ӳ������
typedef struct _PCI9757_STATUS_AD     
{
	LONG bNotEmpty;			// ����FIFO�洢���ķǿձ�־��=TRUE�ǿգ� = FALSE ��
	LONG bHalf;				// ����FIFO�洢���İ�����־��=TRUE�������ϣ� = FALSE ��������
	LONG bDynamic_Overflow; // ����FIFO�洢���Ķ�̬�����־��= TRUE�ѷ�������� = FALSE��δ�������
	LONG bStatic_Overflow;  // ����FIFO�洢���ľ�̬�����־��= TRUE�ѷ�������� = FALSE��δ�������
	LONG bConverting;		// AD�Ƿ�����ת���� =TRUE:��ʾ����ת���� =FALS��ʾת�����
	LONG bTriggerFlag;		// ������־, =TRUE��ʾ�����¼���������=FALSE��ʾ�����¼�δ����
	LONG nTriggerPos;       // ����λ��
} PCI9757_STATUS_AD, *PPCI9757_STATUS_AD;

const int MAX_SEGMENT_COUNT = 128;
typedef struct _PCI9757_STATUS_DMA     
{
	LONG iCurSegmentID;								// ��ǰ�λ���ID,��ʾDMA���ڴ���Ļ�������
	LONG bSegmentSts[128];			// �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
	LONG bBufferOverflow;							// �������״̬,=1��ʾ���������δ���
} PCI9757_STATUS_DMA, *PPCI9757_STATUS_DMA;

//***********************************************************
// �û������ӿ�
#ifndef _PCI9757_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## �豸��������� ##############################
	HANDLE DEVAPI FAR PASCAL PCI9757_CreateDevice(int DeviceLgcID );     // ���߼��Ŵ����豸����
	int DEVAPI FAR PASCAL PCI9757_GetDeviceCount(HANDLE hDevice); // ȡ���豸��̨��
	BOOL DEVAPI FAR PASCAL PCI9757_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID);
	BOOL DEVAPI FAR PASCAL PCI9757_ListDeviceDlg(HANDLE hDevice); // �ԶԻ����巽ʽ�б�ϵͳ���е����еĸ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI9757_ReleaseDevice(HANDLE hDevice); // ���ͷ��豸����

	//####################### AD���ݶ�ȡ���� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
	// AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�
	BOOL DEVAPI FAR PASCAL PCI9757_Calibration(HANDLE hDevice); // ADУ׼ 
	// AD�����ѯ��ʽ����
	BOOL DEVAPI FAR PASCAL PCI9757_InitDeviceProAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									HANDLE hDevice,			// �豸����,����CreateDevice��������
									PPCI9757_PARA_AD pADPara); // Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL PCI9757_StartDeviceProAD(		// �ڳ�ʼ��֮�������豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI9757_ReadDeviceProAD_Npt(		// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(����ǿշ�ʽ)
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									LONG ADBuffer[],		// ����ԭʼAD���ݵ��û�������
									LONG nReadSizeWords,	// �����ƫ�Ƶ���������ݳ���(��)
									PLONG nRetSizeWords);   // ����ʵ�ʶ�ȡ�ĳ���(��)

	BOOL DEVAPI FAR PASCAL PCI9757_GetDevStatusProAD(		// ��AD����������ȡ���豸�ĸ���״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									PPCI9757_STATUS_AD pADStatus); // AD�ĸ�����Ϣ�ṹ��

	BOOL DEVAPI FAR PASCAL PCI9757_ReadDeviceProAD_Half(	// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(���������ʽ)
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									LONG ADBuffer[],		// ����ԭʼAD���ݵ��û�������
									LONG nReadSizeWords,	// �����ƫλ����������ݳ���(��)
									PLONG nRetSizeWords);	// ����ʵ�ʶ�ȡ�ĳ���(��)
		
    BOOL DEVAPI FAR PASCAL PCI9757_StopDeviceProAD(			// �������豸֮����ͣ�豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI9757_ReleaseDeviceProAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
									HANDLE hDevice);		// �豸���,����CreateDevice��������

	//##################### AD���ݶ�ȡ����(DMA��ʽ)(�ϲ��û�����) ###########################
	// ADֱ���ڴ�(DMA)��ʽ����
	BOOL DEVAPI FAR PASCAL PCI9757_InitDeviceDmaAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									HANDLE hDevice,			// �豸����,����CreateDevice��������
									HANDLE hDmaEvent,		// DMA�¼����,����CreateSystemEvent����
									LONG ADBuffer[],		// �û�������,���Ϊ��ά����
									LONG nReadSizeWords,	// ÿ��DMAʱ,�û���ָ������Ӧ��ȡ��ʵ�ʳ���(ҪС�ڻ����nSegmentSizeWords)
									LONG nSegmentCount,		// ����ֶε�����,ȡֵ��ΧΪ2-128
									LONG nSegmentSizeWords,	// �������ֶεĶγ�(�������FIFO��������)
									PPCI9757_PARA_AD pADPara);	// Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL PCI9757_StartDeviceDmaAD(		// �ڳ�ʼ��֮�������豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI9757_GetDevStatusDmaAD(		// ��AD����������ȡ��DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									PPCI9757_STATUS_DMA pDMAStatus); // ��õ�DMA����״̬

	BOOL DEVAPI FAR PASCAL PCI9757_SetDevStatusDmaAD(		// ��AD��������������DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									LONG iClrBufferID);		// Ҫ����Ļ�����ID,������Ϊ0
		
    BOOL DEVAPI FAR PASCAL PCI9757_StopDeviceDmaAD(			// �������豸֮����ͣ�豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI9757_ReleaseDeviceDmaAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
									HANDLE hDevice);		// �豸���,����CreateDevice��������
	
	//##################### AD��Ӳ�������������� ###########################
	BOOL DEVAPI FAR PASCAL PCI9757_SaveParaAD(HANDLE hDevice, PPCI9757_PARA_AD pADPara); // ����ǰ��AD��������������ϵͳ��
    BOOL DEVAPI FAR PASCAL PCI9757_LoadParaAD(HANDLE hDevice, PPCI9757_PARA_AD pADPara); // ��AD����������ϵͳ�ж���
    BOOL DEVAPI FAR PASCAL PCI9757_ResetParaAD(HANDLE hDevice, PPCI9757_PARA_AD pADPara); // ��AD���������ָ�������Ĭ��ֵ

	//################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	BOOL DEVAPI FAR PASCAL PCI9757_GetDeviceBar(					// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ
											HANDLE hDevice,			// �豸������,����CreateDevice��������
											PUCHAR pbPCIBar[6]);	// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����

	BOOL DEVAPI FAR PASCAL PCI9757_WriteRegisterByte(				// ���豸��ӳ��Ĵ����ռ�ָ���˿�д�뵥��������
											HANDLE hDevice,			// �豸����
											PUCHAR pbLinearAddr,	// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes,		// ����ڻ���ַ��ƫ��λ��
											BYTE Value);			// ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����
	BOOL DEVAPI FAR PASCAL PCI9757_WriteRegisterWord(				// д˫�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes,  
											WORD Value);
	BOOL DEVAPI FAR PASCAL PCI9757_WriteRegisterULong(				// д�Ľ������ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes,  
											ULONG Value);
	BYTE DEVAPI FAR PASCAL PCI9757_ReadRegisterByte(				// ���뵥�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes);
	WORD DEVAPI FAR PASCAL PCI9757_ReadRegisterWord(				// ����˫�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes);
	ULONG DEVAPI FAR PASCAL PCI9757_ReadRegisterULong(				// �������ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes);

	//################# I/O�˿�ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	// ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI9757_WritePortByte(HANDLE hDevice, PUCHAR pbPort, BYTE Value);
    BOOL DEVAPI FAR PASCAL PCI9757_WritePortWord(HANDLE hDevice, PUCHAR pbPort, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI9757_WritePortULong(HANDLE hDevice, PUCHAR pbPort, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI9757_ReadPortByte(HANDLE hDevice, PUCHAR pbPort);
    WORD DEVAPI FAR PASCAL PCI9757_ReadPortWord(HANDLE hDevice, PUCHAR pbPort);
    ULONG DEVAPI FAR PASCAL PCI9757_ReadPortULong(HANDLE hDevice, PUCHAR pbPort);

	//########################### ���Ӳ������� ######################################
	HANDLE DEVAPI FAR PASCAL PCI9757_CreateSystemEvent(void);			// �����ں��¼����󣬹�InitDeviceInt��VB���̵߳Ⱥ���ʹ��
	BOOL DEVAPI FAR PASCAL PCI9757_ReleaseSystemEvent(HANDLE hEvent);	// �ͷ��ں��¼�����

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
const long PCI9757_REG_MEM_PLXCHIP	= 0x00; // 0�żĴ�����ӦPLXоƬ��ʹ�õ��ڴ�ģʽ����ַ(ʹ��LinearAddr)
const long PCI9757_REG_IO_PLXCHIP	= 0x01; // 1�żĴ�����ӦPLXоƬ��ʹ�õ�IOģʽ����ַ(ʹ��PhysAddr)
const long PCI9757_REG_IO_CPLD		= 0x02; // 2�żĴ�����Ӧ���Ͽ��Ƶ�Ԫ��ʹ�õ�IOģʽ����ַ(ʹ��PhysAddr)
 

#endif // _PCI9757_DEVICE_
