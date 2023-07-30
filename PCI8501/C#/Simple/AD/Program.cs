using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class Program
    {
        [DllImport("msvcrt.dll")]
        public static extern Int32 _getch();
        [DllImport("msvcrt.dll")]
        public static extern Int32 _kbhit();       
        public static PCI8501.PCI8501_PARA_AD ADPara;
        public static PCI8501.PCI8501_STATUS_AD ADStatus;
        static void Main(string[] args)
        {

            UInt16[] ADBuffer = new UInt16[1048576];            
            IntPtr hDevice;  // �豸������
            Int32 DeviceLgcID;
            Boolean bFirstWait = true;      // Ϊÿ�εȴ�ֻ��ʾһ����ʾ
            UInt32 ulDDR2Length=0;
            Int32 InputRange;
            Int32 nReadSizeWords;   // ÿ�ζ�ȡAD���ݵĳ���(��)
            Int32 nRetSizeWords = 0;
            Int32 nReadSize;            
            Int32 nADChannel = 0;
            Int32[] bChannelArray = new Int32[4];
            Int32[] bSegmentSts = new Int32[64];            
            UInt16 ADData;
            float fVolt = 0.00f;     // ��ADԭʼ����ת��Ϊ��ѹֵ
            String strTemp;
            DeviceLgcID = 0;
            hDevice = (IntPtr)(-1);
            hDevice = PCI8501.PCI8501_CreateDevice(DeviceLgcID); // �����豸����
            if (hDevice == (IntPtr)(-1))
            {
                Console.WriteLine("Create Device Error...");
                _getch();
                return; // ��������豸����ʧ�ܣ��򷵻�
            }

            PCI8501.PCI8501_GetDDR2Length(hDevice, ref  ulDDR2Length);
            InputRange = SelectInputRange();  // Ҫ���û��Ӽ�����ѡ����������

            ADPara.Frequency = 100000; // ����Ƶ��(Hz)
            ADPara.InputRange0 = InputRange;  // ģ��ͨ��0��������
            ADPara.InputRange1 = InputRange;  // ģ��ͨ��1��������
            ADPara.InputRange2 = InputRange;  // ģ��ͨ��2��������
            ADPara.InputRange3 = InputRange;  // ģ��ͨ��3��������
            ADPara.InputRange4 = InputRange;  // ģ��ͨ��4��������
            ADPara.InputRange5 = InputRange;  // ģ��ͨ��5��������
            ADPara.InputRange6 = InputRange;  // ģ��ͨ��6��������
            ADPara.InputRange7 = InputRange;  // ģ��ͨ��7��������
            ADPara.Gains0 = 0; // ����ͨ��0����
            ADPara.Gains1 = 0; // ����ͨ��1����
            ADPara.Gains2 = 0; // ����ͨ��2����
            ADPara.Gains3 = 0; // ����ͨ��3����	
            ADPara.Gains4 = 0; // ����ͨ��4����
            ADPara.Gains5 = 0; // ����ͨ��5����
            ADPara.Gains6 = 0; // ����ͨ��6����
            ADPara.Gains7 = 0; // ����ͨ��7����
            ADPara.M_Length = 0;						// M����
            ADPara.N_Length = 1024;				// N����
            ADPara.TriggerMode = PCI8501.PCI8501_TRIGMODE_MIDL;	// Ӳ���м䴥��(����Ԥ�������󴥷�����)
            ADPara.TriggerSource = PCI8501.PCI8501_TRIGMODE_SOFT;	// �������
            ADPara.TriggerDir = PCI8501.PCI8501_TRIGDIR_NEGATIVE; // �½��ش���
            ADPara.TrigLevelVolt = 0;
            ADPara.ClockSource = PCI8501.PCI8501_CLOCKSRC_IN;		// ʹ���ڲ�ʱ��
            ADPara.bClockSourceDir = 0;
            ADPara.OutClockSource = PCI8501.PCI8501_OUTCLOCKSRC_TRIGGER0;


            if (!(PCI8501.PCI8501_InitDeviceAD(hDevice, ref ADPara) > 0))// ��ʼ��Ӳ��
            {
                Console.WriteLine("PCI8501_InitDeviceAD Error...\n");
                _getch();
                goto ExitRead;
            }

            nReadSizeWords = (ADPara.M_Length + ADPara.N_Length) * 8; // M��N����Ϊ��ȡ���ݵĳ���

            UInt32 ulStartAddr = 0;
            

            while (!(Convert.ToBoolean(_kbhit())))
            {
                if (!(PCI8501.PCI8501_StartDeviceAD(hDevice) > 0)) // �����豸
                {
                    Console.WriteLine("StartDeviceAD Error...\n");
                    _getch();
                }
                if (!(PCI8501.PCI8501_SetDeviceTrigAD(hDevice) > 0)) // ����AD
                {
                    Console.WriteLine("SetDeviceTrigAD Error...\n");
                    _getch();
                }
                ulStartAddr = 0;
                nReadSize = nReadSizeWords;
                bFirstWait = true;
                while (true) // ��ѯ��ǰ�������������Ƿ���׼������
                {
                    if (!(PCI8501.PCI8501_GetDevStatusAD(hDevice, ref ADStatus) > 0))
                    {
                        Console.WriteLine("GetDevStatusAD error...\n");
                        _getch();
                        goto ExitRead;
                    }

                    if (ADStatus.bComplete == 1)
                    {
                        break;  // ������FIFO�洢���������ﵽ�������ϣ����˳�״̬��ѯ,��ʼ��ȡ��������
                    }
                    else
                    {
                        if (bFirstWait)
                        {
                            Console.WriteLine("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...\n");
                            bFirstWait = false;
                        }
                        if (_kbhit() == 1) goto ExitRead; // ����û����������˳�
                    }
                }

                if (!(PCI8501.PCI8501_StopDeviceAD(hDevice) > 0)) // ֹͣ�豸
                {
                    Console.WriteLine("StopDeviceAD Error...\n");
                    _getch();
                }

                Int32 offsetAddr = ADStatus.lEndAddr - nReadSizeWords * 2 + 4;
                if (offsetAddr < 0)
                {
                    ulStartAddr = (UInt32)(ulDDR2Length * 1024 * 1024 + (ADStatus.lEndAddr - nReadSizeWords * 2 + 4));
                }
                else
                    ulStartAddr = (UInt32)offsetAddr;

                while (nReadSize > 0)
                {
                    if (!(PCI8501.PCI8501_ReadDeviceAD(hDevice, ADBuffer, nReadSizeWords, (Int32)ulStartAddr, ref nRetSizeWords) > 0))
                    {
                        Console.WriteLine("ReadDeviceDmaAD error...\n");
                        _getch();
                        goto ExitRead;
                    }
                    nReadSize = nReadSize - 1048576;
                    ulStartAddr = ulStartAddr + 1048576 * 2;

                    for (int Index = 0; Index < 64; )
                    {
                        for (nADChannel = 0; nADChannel < 8; nADChannel++)
                        {
                            if (_kbhit()!=0) goto ExitRead;

                            ADData = (UInt16)((ADBuffer[Index]) & 0xFFFF);
                            // ��ԭ��ת��Ϊ��ѹֵ
                            switch (InputRange)
                            {
                                case PCI8501.PCI8501_INPUT_N10000_P10000mV: // -10000mV - +10000mV
                                    fVolt = (float)((20000.0 / 65536) * ADData - 10000.0);
                                    break;
                                case PCI8501.PCI8501_INPUT_N5000_P5000mV:	// -5000mV - +5000mV
                                    fVolt = (float)((10000.0 / 65536) * ADData - 5000.0);
                                    break;
                                case PCI8501.PCI8501_INPUT_N2500_P2500mV:	// -2500mV - +2500mV
                                    fVolt = (float)((5000.0 / 65536) * ADData - 2500.0);
                                    break;
                                case PCI8501.PCI8501_INPUT_0_P10000mV:	// 0mV - +10000mV
                                    fVolt = (float)((10000.0 / 65536) * ADData);
                                    break;
                                case PCI8501.PCI8501_INPUT_0_P5000mV:	// 0mV - +5000mV
                                    fVolt = (float)((5000.0 / 65536) * ADData);
                                    break;
                                default:
                                    break;
                            }
                            strTemp = fVolt.ToString("0.##");
                            Console.Write("[AI{0}]={1}    ", nADChannel, strTemp); // ��ʾ��ѹֵ					
                            Index++;
                        }
                        Console.WriteLine("\n");
                    } // for(Index=0; Index<64; Index++)
                }

            }

        ExitRead:
            PCI8501.PCI8501_StopDeviceAD(hDevice); // ֹͣAD
            PCI8501.PCI8501_ReleaseDeviceAD(hDevice); // �ͷ�AD
            PCI8501.PCI8501_ReleaseDevice(hDevice); // �ͷ��豸����
            return;
        }


        //////////////////////////////////////////////////////
        // ��ȡ�û�ѡ�����������
        static int SelectInputRange()
        {
            Int32 InputRange;
        Repeat:
            Console.WriteLine("\n");
            Console.WriteLine("0. -10V �� +10V\n");
            Console.WriteLine("1. -5V �� +5V\n");
            Console.WriteLine("2. -2500mV �� +2500mV\n");
            Console.WriteLine("3. 0mV �� +10000mV\n");
            Console.WriteLine("4. 0mV �� +5000mV\n");
            Console.WriteLine("Please Select Input Range[0-4]:");
            InputRange = Convert.ToInt32(Console.ReadLine());
            if (InputRange < 0 || InputRange > 4) goto Repeat; // �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��
            return InputRange;
        }
    }
}
                