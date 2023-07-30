using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class PCI8510
    {
        //#################### ADӲ������PCI8510_PARA_AD���� #####################
        // ����AD������ʵ��Ӳ������
        public struct PCI8510_PARA_AD
        {
            public Int32 Gains0;			// ģ��������ѡ�����У��ֱ����8��ͨ��
            public Int32 Gains1;
            public Int32 Gains2;
            public Int32 Gains3;
            public Int32 Gains4;
            public Int32 Gains5;
            public Int32 Gains6;
            public Int32 Gains7;
	 public Int32 Frequency;         // �ɼ�Ƶ��, ��λΪHz, [1, 500000]
	 public Int32 InputRange;		// ����ѡ��
	 public Int32 TriggerMode;		// ����ģʽѡ��
	 public Int32 TriggerSource;		// ����Դѡ��
	 public Int32 TriggerType;		// ��������ѡ��(���ش���/���崥��)
	 public Int32 TriggerDir;		// ��������ѡ��(����/���򴥷�)
	 public Int32 TrigLevelVolt;		// ������ƽ(��10000mV)
	 public Int32 ClockSource;		// ����ʱ��ѡ��
	 public Int32 bClockOutput;      // �Ƿ�ʱ���ź����,=TRUE:�������, =FALSE:��ֹ���
	 public Int32 OutTrigSource;		// Trig�ź�����/���Դ
	 public Int32 OutTrigSourceDir;	// Trig�ź�����/�������ѡ��=TRUE:�������, =FALSE:��������

        }
        
//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�Gains[x]ģ�������뷶Χ��ʹ�õ�ѡ��
public const Int32 PCI8510_GAINS_1MULT			= 0x00; // 1������
public const Int32 PCI8510_GAINS_2MULT			= 0x01; // 2������
public const Int32 PCI8510_GAINS_5MULT			= 0x02; // 4������
public const Int32 PCI8510_GAINS_10MULT			= 0x03; // 8������

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�InputRangeģ�������뷶Χ��ʹ�õ�ѡ��
public const Int32 PCI8510_INPUT_0_P5000mV		= 0x00; // 0��5000mV
public const Int32 PCI8510_INPUT_0_P10000mV		= 0x01; // 0��10000mV
public const Int32 PCI8510_INPUT_N5000_P5000mV	= 0x02; // ��5000mV
public const Int32 PCI8510_INPUT_N10000_P10000mV= 0x03; // ��10000mV

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
public const Int32 PCI8510_TRIGMODE_SOFT		= 0x00; // ����ڴ���
public const Int32 PCI8510_TRIGMODE_POST		= 0x01; // Ӳ���󴥷�

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
public const Int32 PCI8510_TRIGSRC_ATR			= 0x00; // ѡ���ⲿATR����Դ
public const Int32 PCI8510_TRIGSRC_DTR			= 0x01; // ѡ���ⲿDTR����Դ

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�OutTrigSourceʱ��Դ��ʹ�õ�ѡ��
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER0	= 0x00; // ѡ��TRIG0���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER1	= 0x01; // ѡ��TRIG1���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER2	= 0x02; // ѡ��TRIG2���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER3	= 0x03; // ѡ��TRIG3���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER4	= 0x04; // ѡ��TRIG4���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER5	= 0x05; // ѡ��TRIG5���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER6	= 0x06; // ѡ��TRIG6���
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER7	= 0x07; // ѡ��TRIG7���
public const Int32 PCI8510_OUTCLOCKSRC_CLKIN	= 0x08; // ѡ���ⲿ����ʱ��Դ

// ADӲ������PCI8510_PARA_AD�е�TriggerType����������ʹ�õ�ѡ��
public const Int32 PCI8510_TRIGTYPE_EDGE		= 0x00; // ���ش���
public const Int32 PCI8510_TRIGTYPE_LEVELVOLT	= 0x01; // ��ƽ����

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
public const Int32 PCI8510_TRIGDIR_NEGATIVE		= 0x00; // ���򴥷�(�͵�ƽ/�½��ش���)
public const Int32 PCI8510_TRIGDIR_POSITIVE		= 0x01; // ���򴥷�(�ߵ�ƽ/�����ش���)
public const Int32 PCI8510_TRIGDIR_POSIT_NEGAT	= 0x02; // �����򴥷�(��/�͵�ƽ������/�½��ش���)

//***********************************************************
// ADӲ������PCI8510_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
public const Int32 PCI8510_CLOCKSRC_IN			= 0x00; // �ڲ�ʱ��
public const Int32 PCI8510_CLOCKSRC_OUT			= 0x01; // �ⲿʱ��

//*************************************************************************************
// ����AD������ʵ��Ӳ������
public struct PCI8510_STATUS_AD     
{
	public Int32 bNotEmpty;		// ����FIFO�洢���ķǿձ�־��=TRUE�ǿգ� = FALSE ��
	public Int32 bHalf;			// ����FIFO�洢���İ�����־��=TRUE�������ϣ� = FALSE ��������
	public Int32 bOverflow;		// ����FIFO�洢���Ķ�̬�����־��= TRUE�ѷ�������� = FALSE��δ�������
} 

public const Int32 MAX_SEGMENT_COUNT = 64;
public struct PCI8510_STATUS_DMA     
{
	public Int32 iCurSegmentID;								// ��ǰ�λ���ID,��ʾDMA���ڴ���Ļ�������
    public Int32 bSegmentSts0;			// �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
    public Int32 bSegmentSts1;
    public Int32 bSegmentSts2;
    public Int32 bSegmentSts3;
    public Int32 bSegmentSts4;
    public Int32 bSegmentSts5;
    public Int32 bSegmentSts6;
    public Int32 bSegmentSts7;
    public Int32 bSegmentSts8;
    public Int32 bSegmentSts9;
    public Int32 bSegmentSts10;
    public Int32 bSegmentSts11;
    public Int32 bSegmentSts12;
    public Int32 bSegmentSts13;
    public Int32 bSegmentSts14;
    public Int32 bSegmentSts15;
    public Int32 bSegmentSts16;
    public Int32 bSegmentSts17;
    public Int32 bSegmentSts18;
    public Int32 bSegmentSts19;
    public Int32 bSegmentSts20;
    public Int32 bSegmentSts21;
    public Int32 bSegmentSts22;
    public Int32 bSegmentSts23;
    public Int32 bSegmentSts24;
    public Int32 bSegmentSts25;
    public Int32 bSegmentSts26;
    public Int32 bSegmentSts27;
    public Int32 bSegmentSts28;
    public Int32 bSegmentSts29;
    public Int32 bSegmentSts30;
    public Int32 bSegmentSts31;
    public Int32 bSegmentSts32;
    public Int32 bSegmentSts33;
    public Int32 bSegmentSts34;
    public Int32 bSegmentSts35;
    public Int32 bSegmentSts36;
    public Int32 bSegmentSts37;
    public Int32 bSegmentSts38;
    public Int32 bSegmentSts39;
    public Int32 bSegmentSts40;
    public Int32 bSegmentSts41;
    public Int32 bSegmentSts42;
    public Int32 bSegmentSts43;
    public Int32 bSegmentSts44;
    public Int32 bSegmentSts45;
    public Int32 bSegmentSts46;
    public Int32 bSegmentSts47;
    public Int32 bSegmentSts48;
    public Int32 bSegmentSts49;
    public Int32 bSegmentSts50;
    public Int32 bSegmentSts51;
    public Int32 bSegmentSts52;
    public Int32 bSegmentSts53;
    public Int32 bSegmentSts54;
    public Int32 bSegmentSts55;
    public Int32 bSegmentSts56;
    public Int32 bSegmentSts57;
    public Int32 bSegmentSts58;
    public Int32 bSegmentSts59;
    public Int32 bSegmentSts60;
    public Int32 bSegmentSts61;
    public Int32 bSegmentSts62;
    public Int32 bSegmentSts63;				// �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
	public Int32 bBufferOverflow;							// �������״̬
} 

//***********************************************************
// CreateFileObject���õ��ļ�������ʽ������(��ͨ����ָ��ʵ�ֶ��ַ�ʽ������)
public const Int32 PCI8510_modeRead					= 0x0000;	// ֻ���ļ���ʽ
public const Int32 PCI8510_modeWrite				= 0x0001;	// ֻд�ļ���ʽ
public const Int32 PCI8510_modeReadWrite			= 0x0002;	// �ȶ���д�ļ���ʽ
public const Int32 PCI8510_modeCreate				= 0x1000;	// ����ļ�������Դ������ļ���������ڣ����ؽ����ļ�������0
public const Int32 PCI8510_typeText					= 0x4000;	// ���ı���ʽ�����ļ�




        

        //######################## �豸��������� ##############################

        [DllImport("PCI8510_32.DLL")]
        public static extern IntPtr PCI8510_CreateDevice(Int32 DeviceLgcID);     // ���߼��Ŵ����豸����
        [DllImport("PCI8510_32.DLL")]
        public static extern Int32 PCI8510_GetDeviceCount(IntPtr hDevice); // ȡ���豸��̨��
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDeviceCurrentID(IntPtr hDevice, ref Int32 DeviceLgcID, ref Int32 DevicePhysID);
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseDevice(IntPtr hDevice); // ���ͷ��豸����

        //####################### AD���ݶ�ȡ���� #################################
        // ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
        // �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
        // AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�


        // AD�����ѯ��ʽ����
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_InitDeviceProAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
                                        IntPtr hDevice,			// �豸����,����CreateDevice��������
                                         ref PCI8510_PARA_AD pADPara); // Ӳ������, �����ڴ˺����о���Ӳ��״̬
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StartDeviceProAD(		// �ڳ�ʼ��֮�������豸
                                        IntPtr hDevice);		// �豸������,����CreateDevice��������
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReadDeviceProAD_Npt(		// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(����ǿշ�ʽ)
                                        IntPtr hDevice,			// �豸���,����CreateDevice��������
                                        UInt32[] ADBuffer,		// ����ԭʼAD���ݵ��û�������
                                        UInt32 nReadSizeWords,	// �����ƫλ����������ݳ���(��)
                                        ref Int32 nRetSizeWords);   // ����ʵ�ʶ�ȡ�ĳ���(��)
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDevStatusProAD(		// ��AD����������ȡ���豸�ĸ���״̬,����ֵ��ʾ�����Ƿ�ɹ�
                                        IntPtr hDevice,			// �豸���,����CreateDevice��������
                                        ref PCI8510_STATUS_AD pADStatus); // AD�ĸ�����Ϣ�ṹ��
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReadDeviceProAD_Half(	// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(���������ʽ)
                                        IntPtr hDevice,			// �豸���,����CreateDevice��������
                                        UInt32[] ADBuffer,		// ����ԭʼAD���ݵ��û�������
                                        UInt32 nReadSizeWords,	// �����ƫλ����������ݳ���(��)
                                        ref Int32 nRetSizeWords);	// ����ʵ�ʶ�ȡ�ĳ���(��)
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StopDeviceProAD(			// �������豸֮����ͣ�豸
                                        IntPtr hDevice);		// �豸������,����CreateDevice��������
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseDeviceProAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
                                        IntPtr hDevice);		// �豸���,����CreateDevice��������

        //##################### AD���ݶ�ȡ����(�жϷ�ʽ)(�ϲ��û�����) ###########################
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_InitDeviceDmaAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									IntPtr hDevice,			// �豸����,����CreateDevice��������
									IntPtr hDmaEvent,		// DMA�¼����,����CreateSystemEvent����
									UInt32[,] ADBuffer,		// �û�������,���Ϊ��ά����
									UInt32 nReadSizeWords,	// ÿ��DMAʱ,�û���ָ������Ӧ��ȡ��ʵ�ʳ���(ҪС�ڻ����nSegmentSizeWords)
									Int32 nSegmentCount,		// ����ֶε�����,ȡֵ��ΧΪ2-128
									UInt32 nSegmentSizeWords,// �������ֶεĶγ�(�������FIFO��������)
									ref PCI8510_PARA_AD  pADPara );	
        
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StartDeviceDmaAD(		// �ڳ�ʼ��֮�������豸
                                        IntPtr hDevice);		// �豸������,����CreateDevice��������
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDevStatusDmaAD(		// ��AD����������ȡ��DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									IntPtr hDevice,			// �豸���,����CreateDevice��������
									ref PCI8510_STATUS_DMA pDMAStatus); // ��õ�DMA����״̬
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_SetDevStatusDmaAD(		// ��AD��������������DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									IntPtr hDevice,			// �豸���,����CreateDevice��������
									Int32 iClrBufferID);  
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StopDeviceDmaAD(			// �������豸֮����ͣ�豸
                                        IntPtr hDevice);		// �豸������,����CreateDevice��������
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseDeviceDmaAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
                                        IntPtr hDevice);		// �豸������,����CreateDevice��������
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ADCalibration(			// �豸У׼����
									IntPtr hDevice);

        //##################### AD��Ӳ�������������� ###########################
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_SaveParaAD(IntPtr hDevice, ref PCI8510_PARA_AD pADPara); // ����ǰ��AD��������������ϵͳ��
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_LoadParaAD(IntPtr hDevice, ref PCI8510_PARA_AD pADPara); // ��AD����������ϵͳ�ж���
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ResetParaAD(IntPtr hDevice, ref PCI8510_PARA_AD pADPara); // ��AD���������ָ�������Ĭ��ֵ



        //####################### ����I/O����������� #################################
        // �û�����ʹ��WriteRegisterULong��ReadRegisterULong�Ⱥ���ֱ�ӿ��ƼĴ�������I/O
        // �����������ʹ����������������ʡ�£�������Ҫ�����ļĴ��������λ�����ȣ���ֻ
        // ����VB�����Ե����Բ�����ô�򵥵�ʵ�ָ�������ͨ���Ŀ��ơ�
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_SetDeviceDO(				// ���������״̬
                                            IntPtr hDevice,		// �豸���								        
                                           Byte[] bDOSts);	// ����״̬
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDeviceDI(				// ȡ�ÿ�����״̬     
                                            IntPtr hDevice,		// �豸���								        
                                           Byte[] bDISts);	// ����״̬	
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_RetDeviceDO(				// �ض�������״̬
                                            IntPtr hDevice,		// �豸���	 
                                           Byte[] bDOSts);	// ����״̬

        //################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
        // �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
        // ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
       [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDeviceBar(			// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ
                                        IntPtr hDevice,			// �豸������,����CreateDevice��������
                                        Int64[] pulPCIBar);	// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDevVersion(			// ��ȡ�豸�̼�������汾
                                        IntPtr hDevice,			// �豸������,����CreateDevice��������
                                        ref UInt32 pulFmwVersion,	// �̼��汾
                                        ref UInt32 pulDriverVersion);// �����汾
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WriteRegisterByte(		// ��ָ���Ĵ����ռ�λ��д�뵥��������
                                        IntPtr hDevice,			// �豸������,����CreateDevice��������
                                        Int64 LinearAddr,		// ָ���Ĵ��������Ի���ַ,������GetDeviceAddr�е�LinearAddr��������ֵ
                                        UInt32 OffsetBytes,		// ��������Ի���ַ����ַ��ƫ��λ��(�ֽ�)
                                        Byte Value);			// ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WriteRegisterWord(		// д˫�ֽ����ݣ�����ͬ�ϣ�
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes,
                                        UInt16 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WriteRegisterULong(		// д�Ľ������ݣ�����ͬ�ϣ�
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes,
                                        UInt32 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Byte PCI8510_ReadRegisterByte(		// ���뵥�ֽ����ݣ�����ͬ�ϣ�
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt16 PCI8510_ReadRegisterWord(		// ����˫�ֽ����ݣ�����ͬ�ϣ�
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt32 PCI8510_ReadRegisterULong(		// �������ֽ����ݣ�����ͬ�ϣ�
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes);

        //################# I/O�˿�ֱ�Ӳ�������д���� ########################
        // �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
        // ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
        // ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WritePortByte(			// 8λд�˿�
                                        IntPtr hDevice,			// �豸���(��CreateDevice����)
                                        Int64 nPort,				// �˿ڵ�ַ
                                        Byte Value);			// д����8λ��������
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WritePortWord(IntPtr hDevice, Int64 nPort, UInt16 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WritePortULong(IntPtr hDevice, Int64 nPort, UInt32 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Byte PCI8510_ReadPortByte(IntPtr hDevice, Int64 nPort);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt16 PCI8510_ReadPortWord(IntPtr hDevice, Int64 nPort);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt32 PCI8510_ReadPortULong(IntPtr hDevice, Int64 nPort);
        // �����Ҫ���û�ģʽ�£�ֱ�ӷ���Ӳ���˿ڣ��밲װ��ʹ��ISA\CommUser�µ�����ReadPortByteEx�Ⱥ���

  
        //########################### �̲߳������� ######################################
        [DllImport("PCI8510_32.DLL")]
        public static extern IntPtr PCI8510_CreateSystemEvent(); // �����ں��¼����󣬹�InitDeviceDmaAD��VB���̵߳Ⱥ���ʹ��
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseSystemEvent(IntPtr hEvent); // �ͷ��ں��¼�����
       

   

        //***********************************************************
        // ����FIFO�洢���ĳ���(��)
        public const Int32 FIFO_IDT7202_LENGTH = 1024;
        public const Int32 FIFO_IDT7203_LENGTH = 2048;
        public const Int32 FIFO_IDT7204_LENGTH = 4096;
        public const Int32 FIFO_IDT7205_LENGTH = 8192;
        public const Int32 FIFO_IDT7206_LENGTH = 16384;
        public const Int32 FIFO_IDT7207_LENGTH = 32768;

       

        //***********************************************************
        // �豸��ַ��ȡ����GetDeviceAddr�Ĳ���RegisterID��ʹ�õ�ѡ��(��Ч����)
        public const Int32 PCI8510_REG_MEM_PLXCHIP = 0x00; // 0�żĴ�����ӦPLXоƬ��ʹ�õ��ڴ�ģʽ����ַ(ʹ��LinearAddr)
        public const Int32 PCI8510_REG_IO_PLXCHIP = 0x01; // 1�żĴ�����ӦPLXоƬ��ʹ�õ�IOģʽ����ַ(ʹ��PhysAddr)

    }
}