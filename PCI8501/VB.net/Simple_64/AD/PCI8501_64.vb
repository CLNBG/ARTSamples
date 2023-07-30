Module PCI8501

    '***********************************************************
    ' ����AD�ɼ��Ĳ����ṹ
    Structure PCI8501_PARA_AD
        Dim Frequency As Int32          ' �ɼ�Ƶ��,��λΪHz,��߲�����4000000Hz        
        Dim InputRange0 As Int32
        Dim InputRange1 As Int32
        Dim InputRange2 As Int32
        Dim InputRange3 As Int32
        Dim InputRange4 As Int32
        Dim InputRange5 As Int32
        Dim InputRange6 As Int32
        Dim InputRange7 As Int32
        Dim Gains0 As Int32
        Dim Gains1 As Int32
        Dim Gains2 As Int32
        Dim Gains3 As Int32
        Dim Gains4 As Int32
        Dim Gains5 As Int32
        Dim Gains6 As Int32
        Dim Gains7 As Int32
        Dim M_Length As Int32
        Dim N_Length As Int32
        Dim TriggerMode As Int32        ' ����ģʽѡ��(����������󴥷�)
        Dim TriggerSource As Int32        ' ����Դѡ��
        Dim TriggerDir As Int32         ' ��������ѡ��(����/���򴥷�)
        Dim TrigLevelVolt As Int32       '  ������ƽ(��10000mV)
        Dim ClockSource As Int32        ' ʱ��Դѡ��(��/��ʱ��Դ)
        Dim OutClockSource As Int32        'ʱ���������Դ
        Dim bClockSourceDir As Int32     '�Ƿ�ʱ���ź������PXI����,=TRUE:�������, =FALSE:��������

    End Structure

    ' ADӲ������PCI8501_PARA_AD�е�InputRange[x]ģ�������뷶Χ��ʹ�õ�ѡ��
    Public Const PCI8501_INPUT_N10000_P10000mV = &H0    ' ��10000mV
    Public Const PCI8501_INPUT_N5000_P5000mV = &H1    ' ��5000mV
    Public Const PCI8501_INPUT_N2500_P2500mV = &H2    ' ��2500mV
    Public Const PCI8501_INPUT_0_P10000mV = &H3   ' 0��10000mV
    Public Const PCI8501_INPUT_0_P5000mV = &H4     '0��5000mV

    '***********************************************************
    'AD����PCI8501_PARA_AD�е�Gains[x]ʹ�õ�Ӳ������ѡ��
    Public Const PCI8501_GAINS_1MULT = &H0    '1������
    Public Const PCI8501_GAINS_2MULT = &H1    '2������
    Public Const PCI8501_GAINS_5MULT = &H2    '4������
    Public Const PCI8501_GAINS_10MULT = &H3   '8������

    '***********************************************************
    ' ADӲ������PCI8501_PARA_AD�е�TriggerMode��Ա������ʹ��AD����ģʽѡ��
    Public Const PCI8501_TRIGMODE_MIDL = &H0 ' �м䴥��
    Public Const PCI8501_TRIGMODE_POST = &H1 ' �󴥷�
    Public Const PCI8501_TRIGMODE_PRE = &H2 ' Ԥ����
    Public Const PCI8501_TRIGMODE_DELAY = &H3 ' Ӳ���󴥷�(�����ⴥ��)
    '***********************************************************
    ' ADӲ������PCI8501_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
    Public Const PCI8501_TRIGMODE_SOFT = &H0            ' �������
    Public Const PCI8501_TRIGSRC_DTR = &H1            ' ѡ���ⲿDTR����Դ
    Public Const PCI8501_TRIGSRC_ATR = &H2            ' ѡ���ⲿATR����Դ
    Public Const PCI8501_TRIGSRC_TRIGGER = &H3      ' Trigger�źŴ��������ڶ࿨ͬ����

    '***********************************************************
    ' ADӲ������PCI8501_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
    Public Const PCI8501_TRIGDIR_NEGATIVE = &H0   '�½��ش���
    Public Const PCI8501_TRIGDIR_POSITIVE = &H1   ' �����ش���
    Public Const PCI8501_TRIGDIR_NEGAT_POSIT = &H2   '���±��ؾ�����

    '***********************************************************
    'ADӲ������PCI8501_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
    Public Const PCI8501_CLOCKSRC_IN = &H0 ' ʹ���ڲ�ʱ��
    Public Const PCI8501_CLOCKSRC_OUT = &H1 ' ʹ���ⲿʱ��

    '***********************************************************
    ' ADӲ������PCI8501_PARA_AD�е�OutClockSourceͬ���źŴ���Դ��ʹ�õ�ѡ��
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER0 = &H0       ' ѡ��ͬ��TRIG0�źŴ���
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER1 = &H1      ' ѡ��ͬ��TRIG1�źŴ���
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER2 = &H2      ' ѡ��ͬ��TRIG2�źŴ���		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER3 = &H3      ' ѡ��ͬ��TRIG3�źŴ���		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER4 = &H4      ' ѡ��ͬ��TRIG4�źŴ���		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER5 = &H5      ' ѡ��ͬ��TRIG5�źŴ���		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER6 = &H6      ' ѡ��ͬ��TRIG6�źŴ���		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER7 = &H7      ' ѡ��ͬ��TRIG7�źŴ���	
    ' *************************************************************************************
    ' ����AD������ʵ��Ӳ������
    Structure PCI8501_STATUS_AD

        Dim bADEanble As Int32              'AD�Ƿ��Ѿ�ʹ�ܣ�=TRUE:��ʾ��ʹ�ܣ�=FALSE:��ʾδʹ��
        Dim bTrigger As Int32                  'AD�Ƿ񱻴�����=TRUE:��ʾ�ѱ�������=FALSE:��ʾδ������
        Dim bComplete As Int32      'AD�Ƿ�����ת�������Ƿ������=TRUE:��ʾ�ѽ�����=FALSE:��ʾδ����
        Dim bAheadTrig As Int32       'AD�������Ƿ���ǰ��=TRUE:��ʾ��������ǰ��=FALSE:��ʾ������δ��ǰ
        Dim lEndAddr As Int32           '������ɵĽ�����ַ

    End Structure


    '######################## �豸��������� ##############################
    Declare Function PCI8501_CreateDevice Lib "PCI8501_64" (Optional ByVal DeviceLgcID As Int32 = 0) As Int32     ' �����豸����(ʹ���豸�߼���)
    Declare Function PCI8501_GetDeviceCount Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32             ' ȡ��PCI8501��ϵͳ�е��豸����
    Declare Function PCI8501_ListDeviceDlg Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32                               ' �öԻ����б�ϵͳ���е�����USB2815�豸
    Declare Function PCI8501_ReleaseDevice Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32              ' �豸���

    '####################### AD���ݶ�ȡ���� #################################
    Declare Function PCI8501_GetDDR2Length Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pulDDR2Length As UInt32) As Boolean            ' ��ȡDDR2�Ĵ洢����
    Declare Function PCI8501_ADCalibration Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal InputRange As Int32) As Int32
    Declare Function PCI8501_InitDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32  '  ��ʼ���豸,������TRUE��,�豸���̿�ʼ����.					
    Declare Function PCI8501_StartDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32
    Declare Function PCI8501_SetDeviceTrigAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32
    Declare Function PCI8501_GetDevStatusAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_STATUS_AD) As Int32

    Declare Function PCI8501_ReadDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal ADBuffer() As UInt16, ByVal nReadSizeWords As Int32, ByVal ulStartAddr As Int32, ByRef nRetSizeWords As Int32) As Int32 'DMA��ʽ��AD����
    Declare Function PCI8501_StopDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32
    Declare Function PCI8501_ReleaseDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32

    '################# AD��Ӳ�������������� ########################	

    Declare Function PCI8501_SaveParaAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32
    Declare Function PCI8501_LoadParaAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32
    Declare Function PCI8501_ResetParaAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32 ' ��AD���������ָ�������Ĭ��ֵ


    '####################### ����I/O����������� #################################

    Declare Function PCI8501_GetDeviceDI Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal bDISts() As Byte) As Int32          ' ��������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)

    Declare Function PCI8501_SetDeviceDO Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal bDOSts() As Byte) As Int32          ' �������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)


    '############################################################################
    '################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
    Declare Function PCI8501_GetDeviceBar Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal pbPCIBar() As UInt16) As Int32
    Declare Function PCI8501_GetDevVersion Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal pulFmwVersion As UInt32, ByVal pulDriverVersion As UInt32) As Int32
    Declare Function PCI8501_WriteRegisterByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32, ByVal Value As Byte) As Int32
    Declare Function PCI8501_WriteRegisterWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32, ByVal Value As UInt16) As Int32
    Declare Function PCI8501_WriteRegisterULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32, ByVal Value As Int32) As Int32
    Declare Function PCI8501_ReadRegisterByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadRegisterWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadRegisterULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32) As Int32
    '################# I/O�˿�ֱ�Ӳ�������д���� ########################
    Declare Function PCI8501_WritePortByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt16, ByRef OffsetBytes As UInt32, ByVal Value As Byte) As Int32
    Declare Function PCI8501_WritePortWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt16, ByRef OffsetBytes As UInt32, ByVal Value As UInt16) As Int32
    Declare Function PCI8501_WritePortULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt16, ByRef OffsetBytes As UInt32, ByVal Value As Int32) As Int32
    Declare Function PCI8501_ReadPortByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadPortWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadPortULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt32, ByRef OffsetBytes As UInt32) As Int32
    '########################### ���Ӳ������� ######################################
    Declare Function PCI8501_CreateSystemEvent Lib "PCI8501_64" () As Int32     '�����ں�ϵͳ�¼�����
    Declare Function PCI8501_ReleaseSystemEvent Lib "PCI8501_64" (ByVal hEvent As Int32) As Boolean           '�ͷ��ں��¼�����

    '***********************************************************

    ' ��������֧�ֵĸ���FIFO�洢���ĳ���(��)
    Public Const FIFO_IDT7202_LENGTH = 1024
    Public Const FIFO_IDT7203_LENGTH = 2048
    Public Const FIFO_IDT7204_LENGTH = 4096
    Public Const FIFO_IDT7205_LENGTH = 8192
    Public Const FIFO_IDT7206_LENGTH = 16384
    Public Const FIFO_IDT7207_LENGTH = 32768

End Module
