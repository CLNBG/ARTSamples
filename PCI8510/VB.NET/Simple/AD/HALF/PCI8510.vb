Module PCI8510



    '#################### ADӲ������PCI8510_PARA_AD���� #####################
    ' ����AD������ʵ��Ӳ������
    Structure PCI8510_PARA_AD

        Dim Gains0 As Int32            ' ģ��������ѡ�����У��ֱ����8��ͨ��
        Dim Gains1 As Int32
        Dim Gains2 As Int32
        Dim Gains3 As Int32
        Dim Gains4 As Int32
        Dim Gains5 As Int32
        Dim Gains6 As Int32
        Dim Gains7 As Int32
        Dim Frequency As Int32          ' �ɼ�Ƶ��, ��λΪHz, [1, 500000]
        Dim InputRange As Int32         ' ����ѡ��
        Dim TriggerMode As Int32        ' ����ģʽѡ��
        Dim TriggerSource As Int32     ' ����Դѡ��
        Dim TriggerType As Int32       ' ��������ѡ��(���ش���/���崥��)
        Dim TriggerDir As Int32   ' ��������ѡ��(����/���򴥷�)
        Dim TrigLevelVolt As Int32      ' ������ƽ(��10000mV)
        Dim ClockSource As Int32      ' ����ʱ��ѡ��
        Dim bClockOutput As Int32     ' �Ƿ�ʱ���ź����,=TRUE:�������, =FALSE:��ֹ���
        Dim OutTrigSource As Int32     ' Trig�ź�����/���Դ
        Dim OutTrigSourceDir As Int32   ' Trig�ź�����/�������ѡ��=TRUE:�������, =FALSE:��������
    End Structure
    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�Gains[x]ģ�������뷶Χ��ʹ�õ�ѡ��
    Public Const PCI8510_GAINS_1MULT = &H0  ' 1������
    Public Const PCI8510_GAINS_2MULT = &H1  ' 2������
    Public Const PCI8510_GAINS_5MULT = &H2  ' 4������
    Public Const PCI8510_GAINS_10MULT = &H3  ' 8������

    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�InputRangeģ�������뷶Χ��ʹ�õ�ѡ��
    Public Const PCI8510_INPUT_0_P5000mV = &H0  ' 0��5000mV
    Public Const PCI8510_INPUT_0_P10000mV = &H1  ' 0��10000mV
    Public Const PCI8510_INPUT_N5000_P5000mV = &H2  ' ��5000mV
    Public Const PCI8510_INPUT_N10000_P10000mV = &H3  ' ��10000mV

    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
    Public Const PCI8510_TRIGMODE_SOFT = &H0  ' ����ڴ���
    Public Const PCI8510_TRIGMODE_POST = &H1  ' Ӳ���󴥷�

    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
    Public Const PCI8510_TRIGSRC_ATR = &H0  ' ѡ���ⲿATR����Դ
    Public Const PCI8510_TRIGSRC_DTR = &H1  ' ѡ���ⲿDTR����Դ

    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�OutTrigSourceʱ��Դ��ʹ�õ�ѡ��
    Public Const PCI8510_OUTTRIGSRC_TRIGGER0 = &H0  ' ѡ��TRIG0���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER1 = &H1  ' ѡ��TRIG1���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER2 = &H2  ' ѡ��TRIG2���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER3 = &H3  ' ѡ��TRIG3���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER4 = &H4  ' ѡ��TRIG4���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER5 = &H5  ' ѡ��TRIG5���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER6 = &H6  ' ѡ��TRIG6���
    Public Const PCI8510_OUTTRIGSRC_TRIGGER7 = &H7  ' ѡ��TRIG7���
    Public Const PCI8510_OUTCLOCKSRC_CLKIN = &H8  ' ѡ���ⲿ����ʱ��Դ

    ' ADӲ������PCI8510_PARA_AD�е�TriggerType����������ʹ�õ�ѡ��
    Public Const PCI8510_TRIGTYPE_EDGE = &H0  ' ���ش���
    Public Const PCI8510_TRIGTYPE_LEVELVOLT = &H1  ' ��ƽ����

    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
    Public Const PCI8510_TRIGDIR_NEGATIVE = &H0  ' ���򴥷�(�͵�ƽ/�½��ش���)
    Public Const PCI8510_TRIGDIR_POSITIVE = &H1  ' ���򴥷�(�ߵ�ƽ/�����ش���)
    Public Const PCI8510_TRIGDIR_POSIT_NEGAT = &H2  ' �����򴥷�(��/�͵�ƽ������/�½��ش���)

    '***********************************************************
    ' ADӲ������PCI8510_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
    Public Const PCI8510_CLOCKSRC_IN = &H0  ' �ڲ�ʱ��
    Public Const PCI8510_CLOCKSRC_OUT = &H1  ' �ⲿʱ��

    '*************************************************************************************
    ' ����AD������ʵ��Ӳ������
    Structure PCI8510_STATUS_AD


        Dim bNotEmpty As Int32      ' ����FIFO�洢���ķǿձ�־��=TRUE�ǿգ� = FALSE ��
        Dim bHalf As Int32         '  ����FIFO�洢���İ�����־��=TRUE�������ϣ� = FALSE ��������
        Dim bOverflow As Int32      ' ����FIFO�洢���Ķ�̬�����־��= TRUE�ѷ�������� = FALSE��δ�������
    End Structure

    Public Const MAX_SEGMENT_COUNT = 64
    Public Const bSegmentSts = MAX_SEGMENT_COUNT
    Structure PCI8510_STATUS_DMA

        Dim iCurSegmentID As Int32                              ' ��ǰ�λ���ID,��ʾDMA���ڴ���Ļ�������
        Dim bSegmentSts0 As Int32                                ' �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
        Dim bSegmentSts1 As Int32
        Dim bSegmentSts2 As Int32
        Dim bSegmentSts3 As Int32
        Dim bSegmentSts4 As Int32
        Dim bSegmentSts5 As Int32
        Dim bSegmentSts6 As Int32
        Dim bSegmentSts7 As Int32
        Dim bSegmentSts8 As Int32
        Dim bSegmentSts9 As Int32

        Dim bSegmentSts10 As Int32
        Dim bSegmentSts11 As Int32
        Dim bSegmentSts12 As Int32
        Dim bSegmentSts13 As Int32
        Dim bSegmentSts14 As Int32
        Dim bSegmentSts15 As Int32
        Dim bSegmentSts16 As Int32
        Dim bSegmentSts17 As Int32
        Dim bSegmentSts18 As Int32
        Dim bSegmentSts19 As Int32

        Dim bSegmentSts20 As Int32
        Dim bSegmentSts21 As Int32
        Dim bSegmentSts22 As Int32
        Dim bSegmentSts23 As Int32
        Dim bSegmentSts24 As Int32
        Dim bSegmentSts25 As Int32
        Dim bSegmentSts26 As Int32
        Dim bSegmentSts27 As Int32
        Dim bSegmentSts28 As Int32
        Dim bSegmentSts29 As Int32

        Dim bSegmentSts30 As Int32
        Dim bSegmentSts31 As Int32
        Dim bSegmentSts32 As Int32
        Dim bSegmentSts33 As Int32
        Dim bSegmentSts34 As Int32
        Dim bSegmentSts35 As Int32
        Dim bSegmentSts36 As Int32
        Dim bSegmentSts37 As Int32
        Dim bSegmentSts38 As Int32
        Dim bSegmentSts39 As Int32

        Dim bSegmentSts40 As Int32
        Dim bSegmentSts41 As Int32
        Dim bSegmentSts42 As Int32
        Dim bSegmentSts43 As Int32
        Dim bSegmentSts44 As Int32
        Dim bSegmentSts45 As Int32
        Dim bSegmentSts46 As Int32
        Dim bSegmentSts47 As Int32
        Dim bSegmentSts48 As Int32
        Dim bSegmentSts49 As Int32

        Dim bSegmentSts50 As Int32
        Dim bSegmentSts51 As Int32
        Dim bSegmentSts52 As Int32
        Dim bSegmentSts53 As Int32
        Dim bSegmentSts54 As Int32
        Dim bSegmentSts55 As Int32
        Dim bSegmentSts56 As Int32
        Dim bSegmentSts57 As Int32
        Dim bSegmentSts58 As Int32
        Dim bSegmentSts59 As Int32

        Dim bSegmentSts60 As Int32
        Dim bSegmentSts61 As Int32
        Dim bSegmentSts62 As Int32
        Dim bSegmentSts63 As Int32          ' �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
        Dim bBufferOverflow As Int32                            ' �������״̬
    End Structure



    '######################## �豸��������� ##############################

    Declare Function PCI8510_CreateDevice Lib "PCI8510_32" (ByVal DeviceLgcID As Int32) As IntPtr      ' ���߼��Ŵ����豸����
    Declare Function PCI8510_GetDeviceCount Lib "PCI8510_32" (ByVal hDevice As IntPtr) As Int32  ' ȡ���豸��̨��
    Declare Function PCI8510_GetDeviceCurrentID Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef DeviceLgcID As Int32, ByRef DevicePhysID As Int32) As Boolean
    Declare Function PCI8510_ReleaseDevice Lib "PCI8510_32" (ByVal hDevice As IntPtr) As Boolean  ' ���ͷ��豸����

    '####################### AD���ݶ�ȡ���� #################################
    ' ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
    ' �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
    ' AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�


    ' AD�����ѯ��ʽ����
    Declare Function PCI8510_InitDeviceProAD Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' ��ʼ���豸��������TRUE��,�豸��׼������.' �豸����,����CreateDevice��������' Ӳ������, �����ڴ˺����о���Ӳ��״̬

    Declare Function PCI8510_StartDeviceProAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean         ' �ڳ�ʼ��֮�������豸' �豸������,����CreateDevice��������

    Declare Function PCI8510_ReadDeviceProAD_Npt Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal ADBufferas() As UInt32, _
            ByVal nReadSizeWords As UInt32, _
            ByRef nRetSizeWords As Int32) As Boolean ' ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(����ǿշ�ʽ) ' �豸���,����CreateDevice��������' ����ԭʼAD���ݵ��û������� ' �����ƫλ����������ݳ���(��) ' ����ʵ�ʶ�ȡ�ĳ���(��)

    Declare Function PCI8510_GetDevStatusProAD Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByRef pADStatus As PCI8510_STATUS_AD) As Boolean  ' ��AD����������ȡ���豸�ĸ���״̬,����ֵ��ʾ�����Ƿ�ɹ�' �豸���,����CreateDevice��������' AD�ĸ�����Ϣ�ṹ��

    Declare Function PCI8510_ReadDeviceProAD_Half Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal ADBuffer() As UInt32, _
            ByVal nReadSizeWords As UInt32, _
            ByRef nRetSizeWords As Int32) As Boolean        ' ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(���������ʽ)' �豸���,����CreateDevice��������' ����ԭʼAD���ݵ��û�������' �����ƫλ����������ݳ���(��)' ����ʵ�ʶ�ȡ�ĳ���(��)

    Declare Function PCI8510_StopDeviceProAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean         ' �������豸֮����ͣ�豸' �豸������,����CreateDevice��������

    Declare Function PCI8510_ReleaseDeviceProAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean         ' �ر�AD�豸,��ֹ����,���ͷ���Դ' �豸���,����CreateDevice��������

    '##################### ' ADֱ���ڴ�(DMA)��ʽ����) ###########################
    Declare Function PCI8510_InitDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal hDmaEvent As IntPtr, _
         ByVal ADBuffer(,) As UInt32, _
         ByVal nReadSizeWords As UInt32, _
         ByVal nSegmentCount As Int32, _
         ByVal nSegmentSizeWords As UInt32, _
         ByRef pADPara As PCI8510_PARA_AD) As Boolean

    Declare Function PCI8510_StartDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean
    ' �ڳ�ʼ��֮�������豸 ' �豸������,����CreateDevice��������
    Declare Function PCI8510_GetDevStatusDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByRef pDMAStatus As PCI8510_STATUS_DMA) As Boolean ' ��AD����������ȡ��DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ� ' �豸���,����CreateDevice�������� ' ��õ�DMA����״̬

    Declare Function PCI8510_SetDevStatusDmaAD Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal iClrBufferID As Int32) As Boolean         ' ��AD��������������DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�' �豸���,����CreateDevice��������' Ҫ����Ļ�����ID,������Ϊ0

    Declare Function PCI8510_StopDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean        ' �������豸֮����ͣ�豸' �豸������,����CreateDevice��������

    Declare Function PCI8510_ReleaseDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean      ' �ر�AD�豸,��ֹ����,���ͷ���Դ' �豸���,����CreateDevice��������

    Declare Function PCI8510_ADCalibration Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean  ' �豸У׼����' �豸������

    '##################### AD��Ӳ�������������� ###########################
    Declare Function PCI8510_SaveParaAD Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' ����ǰ��AD��������������ϵͳ��
    Declare Function PCI8510_LoadParaAD Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' ��AD����������ϵͳ�ж���
    Declare Function PCI8510_ResetParaAD Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' ��AD���������ָ�������Ĭ��ֵ


    '####################### ����I/O����������� #################################
    ' �û�����ʹ��WriteRegisterULong��ReadRegisterULong�Ⱥ���ֱ�ӿ��ƼĴ�������I/O
    ' �����������ʹ����������������ʡ�£�������Ҫ�����ļĴ��������λ�����ȣ���ֻ
    ' ����VB�����Ե����Բ�����ô�򵥵�ʵ�ָ�������ͨ���Ŀ��ơ�
    Declare Function PCI8510_SetDeviceDO Lib "PCI8510_32" ( _
          ByVal hDevice As IntPtr, _
          ByVal bDOSts() As Byte) As Boolean    ' ���������״̬' �豸���	' ����״̬

    Declare Function PCI8510_GetDeviceDI Lib "PCI8510_32" ( _
             ByVal hDevice As IntPtr, _
             ByVal bDISts() As Byte) As Boolean    ' ȡ�ÿ�����״̬    ' �豸���		' ����״̬	

    Declare Function PCI8510_RetDeviceDO Lib "PCI8510_32" ( _
             ByVal hDevice As IntPtr, _
             ByVal bDOSts() As Byte) As Boolean    ' �ض�������״̬' �豸���	 ' ����״̬

    '################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
    ' �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
    ' ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
    Declare Function PCI8510_GetDeviceBar Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal pulPCIBar() As Int64) As Boolean  ' ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ' �豸������,����CreateDevice��������' ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����

    Declare Function PCI8510_GetDevVersion Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByRef pulFmwVersion As UInt32, _
            ByRef pulDriverVersion As UInt32) As Boolean ' ��ȡ�豸�̼�������汾' �豸������,����CreateDevice�������� ' �̼��汾' �����汾

    Declare Function PCI8510_WriteRegisterByte Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32, _
         ByVal Value As Byte) As Boolean            ' ��ָ���Ĵ����ռ�λ��д�뵥��������' �豸������,����CreateDevice��������' ָ���Ĵ��������Ի���ַ,������GetDeviceAddr�е�LinearAddr��������ֵ' ��������Ի���ַ����ַ��ƫ��λ��(�ֽ�)' ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����

    Declare Function PCI8510_WriteRegisterWord Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal LinearAddr As Int64, _
            ByVal OffsetBytes As UInt32, _
            ByVal Value As UInt16) As Boolean ' д˫�ֽ����ݣ�����ͬ�ϣ�

    Declare Function PCI8510_WriteRegisterULong Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal LinearAddr As Int64, _
            ByVal OffsetBytes As UInt32, _
            ByVal Value As UInt32) As Boolean ' д�Ľ������ݣ�����ͬ�ϣ�

    Declare Function PCI8510_ReadRegisterByte Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32) As Byte ' ���뵥�ֽ����ݣ�����ͬ�ϣ�

    Declare Function PCI8510_ReadRegisterWord Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32) As UInt16 ' ����˫�ֽ����ݣ�����ͬ�ϣ�

    Declare Function PCI8510_ReadRegisterULong Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32) As UInt32 ' �������ֽ����ݣ�����ͬ�ϣ�

    '################# I/O�˿�ֱ�Ӳ�������д���� ########################
    ' �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
    ' ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
    ' ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
    Declare Function PCI8510_WritePortByte Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal nPort As Int64, _
         ByVal Value As Byte) As Boolean         ' 8λд�˿�' �豸���(��CreateDevice����)	' �˿ڵ�ַ' д����8λ��������
    Declare Function PCI8510_WritePortWord Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64, ByVal Value As UInt16) As Boolean
    Declare Function PCI8510_WritePortULong Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64, ByVal Value As UInt32) As Boolean

    Declare Function PCI8510_ReadPortByte Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64) As Byte
    Declare Function PCI8510_ReadPortWord Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64) As UInt16
    Declare Function PCI8510_ReadPortULong Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64) As UInt32
    ' �����Ҫ���û�ģʽ�£�ֱ�ӷ���Ӳ���˿ڣ��밲װ��ʹ��ISA\CommUser�µ�����ReadPortByteEx�Ⱥ���

  
    '########################### �̲߳������� ######################################
    Declare Function PCI8510_CreateSystemEvent Lib "PCI8510_32" () As IntPtr  ' �����ں��¼����󣬹�InitDeviceDmaAD��VB���̵߳Ⱥ���ʹ��
    Declare Function PCI8510_ReleaseSystemEvent Lib "PCI8510_32" (ByVal hEvent As IntPtr) As Boolean  ' �ͷ��ں��¼�����
 

    '***********************************************************
    ' ����FIFO�洢���ĳ���(��)
    Public Const FIFO_IDT7202_LENGTH = 1024
    Public Const FIFO_IDT7203_LENGTH = 2048
    Public Const FIFO_IDT7204_LENGTH = 4096
    Public Const FIFO_IDT7205_LENGTH = 8192
    Public Const FIFO_IDT7206_LENGTH = 16384
    Public Const FIFO_IDT7207_LENGTH = 32768


    '***********************************************************
    ' �豸��ַ��ȡ����GetDeviceAddr�Ĳ���RegisterID��ʹ�õ�ѡ��(��Ч����)
    Public Const PCI8510_REG_MEM_PLXCHIP = &H0  ' 0�żĴ�����ӦPLXоƬ��ʹ�õ��ڴ�ģʽ����ַ(ʹ��LinearAddr)
    Public Const PCI8510_REG_IO_PLXCHIP = &H1  ' 1�żĴ�����ӦPLXоƬ��ʹ�õ�IOģʽ����ַ(ʹ��PhysAddr)


End Module
