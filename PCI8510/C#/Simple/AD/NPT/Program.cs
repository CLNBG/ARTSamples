using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    class Program
    {
        [DllImport("msvcrt.dll")]
        public static extern Int32 _getch();
        [DllImport("msvcrt.dll")]
        public static extern Int32 _kbhit();

        public const int WAIT_OBJECT_0 = 0;
        [DllImport("Kernel32.dll")]
        public static extern int WaitForSingleObject(IntPtr hHandle, int dwMillisenconds);

        [DllImport("Kernel32.dll")]
        public static extern IntPtr CreateEvent(String lpEventAttributes, bool bManualReset, bool bInitialState, String lpName);

        public static PCI8510.PCI8510_PARA_AD ADPara;							// 硬件参数

        static void Main(string[] args)
        {
            IntPtr hDevice;
            Int32 DeviceID;
            UInt32[] ADBuffer = new UInt32[4096];	// 分配缓冲区(存储原始数据)
            UInt32 nReadSizeWords;					// 每次读取AD数据的长度(字)
            Int32 nRetSizeWords = 0;
            Int32 nChannel = 0;

            float fVolt;

            Int32 InputRange;
            Int32 Index;
            DeviceID = 0;
            fVolt = 0;
            hDevice = PCI8510.PCI8510_CreateDevice(DeviceID); // 创建设备对象
            if (hDevice == (IntPtr)(-1))
            {
                Console.WriteLine("PCI8510_CreateDevice Error");
                _getch();
                return; // 如果创建设备对象失败，则返回
            }
        Repeat:
            Console.WriteLine("\n");
            Console.WriteLine("0. 0V ～ 5V\n");
            Console.WriteLine("1. 0V ～ +10V\n");
            Console.WriteLine("2. -5V ～ +5V\n");
            Console.WriteLine("3. -10V ～ +10V\n");

            Console.WriteLine("Please Select Input Range[0-3]:");
            InputRange = Convert.ToInt32(Console.ReadLine()); ;
            if (InputRange < 0 || InputRange > 3) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择

            //memset(&ADPara, 0x00, sizeof(ADPara)); // 将各项参数复位至确定值0(强烈建议)

            // 预置硬件参数
            ADPara.Gains0 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道0输入量程
            ADPara.Gains1 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道1输入量程
            ADPara.Gains2 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道2输入量程
            ADPara.Gains3 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道3输入量程	
            ADPara.Gains4 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道4输入量程
            ADPara.Gains5 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道5输入量程
            ADPara.Gains6 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道6输入量程
            ADPara.Gains7 = PCI8510.PCI8510_GAINS_1MULT;  // 模拟通道7输入量程	
            ADPara.Frequency = 10000; // 采样频率(Hz)
            ADPara.InputRange = InputRange;
            ADPara.TriggerMode = PCI8510.PCI8510_TRIGMODE_SOFT;
            ADPara.TriggerSource = PCI8510.PCI8510_TRIGSRC_ATR; // 触发源
            ADPara.TriggerType = PCI8510.PCI8510_TRIGTYPE_EDGE; // 触发类型为边沿触发	
            ADPara.TriggerDir = PCI8510.PCI8510_TRIGDIR_NEGATIVE; // 触发方向为负向
            ADPara.TrigLevelVolt = 0;    // 触发电平
            ADPara.ClockSource = PCI8510.PCI8510_CLOCKSRC_IN; // 时钟源选用板内时钟源
            ADPara.bClockOutput = 0; // 禁止时钟输出
            ADPara.OutTrigSource = PCI8510.PCI8510_OUTTRIGSRC_TRIGGER0;
            ADPara.OutTrigSourceDir = 0;


            if (!PCI8510.PCI8510_InitDeviceProAD(hDevice, ref ADPara)) // 初始化硬件
            {

                Console.WriteLine("PCI8510_InitDeviceProAD  Error");
                _getch();
                return;
            }
            nReadSizeWords = 4096;
            if (!PCI8510.PCI8510_StartDeviceProAD(hDevice)) // 启动设备
            {
                Console.WriteLine("StartDeviceAD Error...");
                _getch();
            }
            bool bFirstWait = true;
            while (_kbhit() == 0)
            {
                bFirstWait = true;
                PCI8510.PCI8510_ReadDeviceProAD_Npt(hDevice, ADBuffer, nReadSizeWords, ref nRetSizeWords);
                for (Index = 0; Index < 64; )
                {
                    for (nChannel = 0; nChannel < 8; nChannel++)
                    {
                        if (_kbhit() != 0) goto ExitRead;
                        // 将原码转换为电压值
                        switch (InputRange)
                        {
                            case 0: // 0V - +5V
                                fVolt = (float)((5000.0 / 65536) * (ADBuffer[Index] & 0xFFFF));
                                break;
                            case 1: // 0V - +10V
                                fVolt = (float)((10000.0 / 65536) * (ADBuffer[Index] & 0xFFFF));
                                break;
                            case 2: // -5V - +5V
                                fVolt = (float)((10000.0 / 65536) * (ADBuffer[Index] & 0xFFFF) - 5000.0);
                                break;
                            case 3: // -10V - +10V
                                fVolt = (float)((20000.0 / 65536) * (ADBuffer[Index] & 0xFFFF) - 10000.0);
                                break;
                            default:
                                break;
                        }
                        Console.Write("CH{0}={1}   ", nChannel, fVolt.ToString("0.##")); // 显示电压值

                        if ((nChannel + 1) % 4 == 0)
                        {
                            Console.WriteLine("  "); // 显示电压值
                        }
                        Index++;
                    }
                }
            }


        ExitRead:
            PCI8510.PCI8510_ReleaseDeviceProAD(hDevice);	// 释放AD
            PCI8510.PCI8510_ReleaseDevice(hDevice);			// 释放设备对象

            return;







        }
    }
}