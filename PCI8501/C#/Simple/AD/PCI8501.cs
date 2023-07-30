using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class PCI8501
    {
        //***********************************************************
        // ����AD�ɼ���ʵ��Ӳ������
        public struct PCI8501_PARA_AD
        {

            public Int32 Frequency;         // �ɼ�Ƶ��, ��λΪHz, ��߲�����4000000Hz
            public Int32 InputRange0;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange1;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange2;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange3;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange4;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange5;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange6;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 InputRange7;		// ģ������������ѡ�����У��ֱ����4��ͨ��
            public Int32 Gains0;	// ����ѡ�� 
            public Int32 Gains1;	// ����ѡ��
            public Int32 Gains2;	// ����ѡ��
            public Int32 Gains3;	// ����ѡ��
            public Int32 Gains4;	// ����ѡ�� 
            public Int32 Gains5;	// ����ѡ��
            public Int32 Gains6;	// ����ѡ��
            public Int32 Gains7;	// ����ѡ��
            public Int32 M_Length;          // M�γ���(��), �ܵ�ȡֵ��Χ1-32M, ����M��N���Ȳ��ܴ���32M
            public Int32 N_Length;          // N�γ���(��), �ܵ�ȡֵ��Χ1-32M, ����M��N���Ȳ��ܴ���32M            
            public Int32 TriggerMode;		// ����ģʽѡ��
            public Int32 TriggerSource;		// ����Դѡ��            
            public Int32 TriggerDir;		// ��������ѡ��(����/���򴥷�)
          public Int32 TrigLevelVolt;		// ������ƽ(��10000mV)            
            public Int32 ClockSource;		// ʱ��Դѡ��(��/��ʱ��Դ)
            public Int32 OutClockSource;	// ʱ���������Դ
            public Int32 bClockSourceDir;   // �Ƿ�ʱ���ź������PXI����,=TRUE:�������, =FALSE:��������
        }
        // ADӲ������PCI8501_PARA_AD�е�InputRange[x]ģ�������뷶Χ��ʹ�õ�ѡ��
        public const Int32 PCI8501_INPUT_N10000_P10000mV = 0x00; // ��10000mV
        public const Int32 PCI8501_INPUT_N5000_P5000mV = 0x01; // ��5000mV
        public const Int32 PCI8501_INPUT_N2500_P2500mV = 0x02; // ��2500mV
        public const Int32 PCI8501_INPUT_0_P10000mV = 0x03; // 0��10000mV
        public const Int32 PCI8501_INPUT_0_P5000mV = 0x04; // 0��5000mV

        //***********************************************************
        // AD����PCI8501_PARA_AD�е�Gains[x]ʹ�õ�Ӳ������ѡ��
        public const Int32 PCI8501_GAINS_1MULT = 0x00; // 1������
        public const Int32 PCI8501_GAINS_2MULT = 0x01; // 2������
        public const Int32 PCI8501_GAINS_5MULT = 0x02; // 4������
        public const Int32 PCI8501_GAINS_10MULT = 0x03; // 8������

        // ADӲ������PCI8501_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
        public const Int32 PCI8501_TRIGMODE_MIDL = 0x00; // �м䴥��
        public const Int32 PCI8501_TRIGMODE_POST = 0x01; // �󴥷�
        public const Int32 PCI8501_TRIGMODE_PRE = 0x02; // Ԥ����
        public const Int32 PCI8501_TRIGMODE_DELAY = 0x03; // Ӳ����ʱ����

        //***********************************************************
        // ADӲ������PCI8501_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
        public const Int32 PCI8501_TRIGMODE_SOFT = 0x00; // �������
        public const Int32 PCI8501_TRIGSRC_DTR = 0x01; // ѡ��DTR��Ϊ����Դ
        public const Int32 PCI8501_TRIGSRC_ATR = 0x02; // ѡ��ATR��Ϊ����Դ
        public const Int32 PCI8501_TRIGSRC_TRIGGER = 0x03; // Trigger�źŴ��������ڶ࿨ͬ����

        //***********************************************************
        // ADӲ������PCI8501_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
        public const Int32 PCI8501_TRIGDIR_NEGATIVE = 0x00; // �½��ش���
        public const Int32 PCI8501_TRIGDIR_POSITIVE = 0x01; // �����ش���
        public const Int32 PCI8501_TRIGDIR_NEGAT_POSIT = 0x02; // ���±��ؾ�����

        //***********************************************************
        // ADӲ������PCI8501_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
        public const Int32 PCI8501_CLOCKSRC_IN = 0x00; // ʹ���ڲ�ʱ��
        public const Int32 PCI8501_CLOCKSRC_OUT = 0x01; // ʹ���ⲿʱ��

        //***********************************************************
        // ADӲ������PCI8501_PARA_AD�е�OutClockSourceʱ��Դ��ʹ�õ�ѡ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER0 = 0x00; // ѡ��TRIG0����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER1 = 0x01; // ѡ��TRIG1����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER2 = 0x02; // ѡ��TRIG2����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER3 = 0x03; // ѡ��TRIG3����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER4 = 0x04; // ѡ��TRIG4����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER5 = 0x05; // ѡ��TRIG5����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER6 = 0x06; // ѡ��TRIG6����/���ʱ��
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER7 = 0x07; // ѡ��TRIG7����/���ʱ��

        //*************************************************************************************
        // ����AD������ʵ��Ӳ������
        public struct PCI8501_STATUS_AD      
       {
           public Int32 bADEanble;	// AD�Ƿ��Ѿ�ʹ�ܣ�=TRUE:��ʾ��ʹ�ܣ�=FALSE:��ʾδʹ��
           public Int32 bTrigger;  // AD�Ƿ񱻴�����=TRUE:��ʾ�ѱ�������=FALSE:��ʾδ������
           public Int32 bComplete; // AD�Ƿ�����ת�������Ƿ������=TRUE:��ʾ�ѽ�����=FALSE:��ʾδ����
           public Int32 bAheadTrig;// AD�������Ƿ���ǰ��=TRUE:��ʾ��������ǰ��=FALSE:��ʾ������δ��ǰ
           public Int32 lEndAddr;	// ������ɵĽ�����ַ
        }       
    
        //######################## �豸��������� #################################

        [DllImport("PCI8501_32.DLL")]
        public static extern IntPtr PCI8501_CreateDevice(Int32 DeviceID ); // �����豸����(�ú���ʹ��ϵͳ���߼��豸ID��
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDeviceCount(IntPtr hDevice);      //  ȡ���豸��̨��
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ListDeviceDlg(IntPtr hDevice); // �öԻ����б�ϵͳ���е�����PCI�豸
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReleaseDevice(IntPtr hDevice);    // ���ͷ��豸����

        //####################### AD���ݶ�ȡ���� #################################
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDDR2Length(IntPtr hDevice, ref UInt32 ulDDR2Length);    // �豸У׼
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ADCalibration(IntPtr hDevice, Int32 InputRange);    // �豸У׼
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_InitDeviceAD(				// ��ʼ���豸,������TRUE��,�豸���̿�ʼ����.
					                IntPtr hDevice,				// �豸���,��Ӧ��CreateDevice��������
					                ref PCI8501_PARA_AD pADPara);  // Ӳ������, �����ڴ˺����о���Ӳ��״̬
		[DllImport("PCI8501_32.DLL")]
		public static extern Int32 PCI8501_StartDeviceAD(IntPtr hDevice);   //�ڳ�ʼ��֮�������豸
			
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_SetDeviceTrigAD(IntPtr hDevice);	  // ���豸ʹ������󣬲�����������¼���ֻ�д���ԴΪ�������ʱ��Ч��
					                
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDevStatusAD(           // ��AD����������ȡ���豸�ĸ���״̬,����ֵ��ʾ�����Ƿ�ɹ�
                                    IntPtr hDevice,                      // �豸���,����CreateDevice��������
                                    ref PCI8501_STATUS_AD pADStatus);       // AD�ĸ�����Ϣ�ṹ��
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadDeviceAD(       // ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(����ʽ
                                    IntPtr hDevice,                     // �豸���,����CreateDevice��������
                                    UInt16[] ADBuffer,                  // ����ԭʼAD���ݵ��û�������
                                    Int32 nReadSizeWords,               // �����ƫλ����������ݳ���(��)
                                    Int32 ulStartAddr,                // ������RAM�е���ʼ��ַ
                                    ref Int32 nRetSizeWords);           // ����ʵ�ʶ�ȡ�ĳ���(��)

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_StopDeviceAD( IntPtr hDevice); // �������豸֮����ͣ�豸

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReleaseDeviceAD( IntPtr hDevice); // �ر�AD�豸,��ֹ����,���ͷ���Դ

      
        //################# AD��Ӳ�������������� ########################	

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_SaveParaAD(IntPtr hDevice, ref PCI8501_PARA_AD pADPara); // ����ǰ��AD��������������ϵͳ��
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_LoadParaAD(IntPtr hDevice, ref PCI8501_PARA_AD pADPara); // ��AD����������ϵͳ�ж���
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ResetParaAD(IntPtr hDevice, ref PCI8501_PARA_AD pADPara); // ��AD���������ָ�������Ĭ��ֵ       

      
       //####################### ����I/O����������� #################################

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDeviceDI(					// ȡ�ÿ�����״̬     
                                   IntPtr hDevice,				// �豸���,��Ӧ��CreateDevice��������								        
                                   Byte[] bDISts);			// ��������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_SetDeviceDO(					// ���������״̬
                                   IntPtr hDevice,				// �豸���,��Ӧ��CreateDevice��������								        
                                   Byte[] bDOSts);			// �������״̬(ע��: ���붨��Ϊ8���ֽ�Ԫ�ص�����)

        //################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDeviceBar(					// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ    
                                   IntPtr hDevice,				// �豸���,��Ӧ��CreateDevice��������								        
                                   UInt16[] pbPCIBar);			// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDevVersion(					// ��ָ���Ĵ����ռ�λ��д�뵥��������    
                                   IntPtr hDevice,				// �豸���,��Ӧ��CreateDevice��������
                                   UInt32 pulFmwVersion,          //  �̼��汾
                                   UInt32 pulDriverVersion);      // �����汾                                   			
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WriteRegisterByte(					// д���ֽ����ݣ�����ͬ�ϣ�   
                                   IntPtr hDevice,
                                   UInt16 pbLinearAddr,
                                   UInt32 OffsetBytes,
                                   Byte Value);
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WriteRegisterWord(					// д˫�ֽ����ݣ�����ͬ�ϣ�   
                                   IntPtr hDevice,				
                                   UInt16 pbLinearAddr,
                                   Int32 OffsetBytes,
                                   Byte Value);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WriteRegisterULong(					// д���ֽ����ݣ�����ͬ�ϣ�   
                                   IntPtr hDevice,
                                   UInt16 pbLinearAddr,
                                   Int32 OffsetBytes,
                                   Byte Value);
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadRegisterByte(					// ���뵥�ֽ����ݣ�����ͬ�ϣ�   
                                   IntPtr hDevice,
                                   UInt16 pbLinearAddr,
                                   Int32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadRegisterWord(					// ����˫�ֽ����ݣ�����ͬ�ϣ�   
                                   IntPtr hDevice,
                                   Int32 pbLinearAddr,
                                   Int32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadRegisterULong(					// �������ֽ����ݣ�����ͬ�ϣ�   
                                   IntPtr hDevice,
                                   Int32 pbLinearAddr,
                                   Int32 OffsetBytes);
        //################# I/O�˿�ֱ�Ӳ�������д���� ########################

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WritePortByte(					// 8λд�˿�   
                                   IntPtr hDevice,                          // �豸���(��CreateDevice����)
                                   Int32 pPort,                             // �˿ڵ�ַ
                                   UInt32 OffsetBytes,
                                   Byte Value);                             // д����8λ��������
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WritePortWord(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes, UInt16 Value);
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WritePortULong(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes, UInt32 Value);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadPortByte(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadPortWord(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadPortULong(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes);
        // �����Ҫ���û�ģʽ�£�ֱ�ӷ���Ӳ���˿ڣ��밲װ��ʹ��ISA\CommUser�µ�����ReadPortByteEx�Ⱥ���
        //########################### ���Ӳ������� ######################################

        [DllImport("PCI8501_32.DLL")]
        public static extern IntPtr PCI8501_CreateSystemEvent(); // ȡ��ָ���ļ����ȣ��ֽڣ�// �����ں��¼����󣬹�InitDeviceDmaAD��VB���̵߳Ⱥ���ʹ��
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReleaseSystemEvent(IntPtr hEvent);// �ͷ��ں��¼�����
        //***********************************************************
        // ����FIFO�洢���ĳ���(��)
        public const Int32 FIFO_IDT7202_LENGTH = 1024;
        public const Int32 FIFO_IDT7203_LENGTH = 2048;
        public const Int32 FIFO_IDT7204_LENGTH = 4096;
        public const Int32 FIFO_IDT7205_LENGTH = 8192;
        public const Int32 FIFO_IDT7206_LENGTH = 16384;
        public const Int32 FIFO_IDT7207_LENGTH = 32768;

    }
}
