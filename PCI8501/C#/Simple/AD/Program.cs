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
            IntPtr hDevice;  // 设备对象句柄
            Int32 DeviceLgcID;
            Boolean bFirstWait = true;      // 为每次等待只显示一次提示
            UInt32 ulDDR2Length=0;
            Int32 InputRange;
            Int32 nReadSizeWords;   // 每次读取AD数据的长度(字)
            Int32 nRetSizeWords = 0;
            Int32 nReadSize;            
            Int32 nADChannel = 0;
            Int32[] bChannelArray = new Int32[4];
            Int32[] bSegmentSts = new Int32[64];            
            UInt16 ADData;
            float fVolt = 0.00f;     // 将AD原始数据转换为电压值
            String strTemp;
            DeviceLgcID = 0;
            hDevice = (IntPtr)(-1);
            hDevice = PCI8501.PCI8501_CreateDevice(DeviceLgcID); // 创建设备对象
            if (hDevice == (IntPtr)(-1))
            {
                Console.WriteLine("Create Device Error...");
                _getch();
                return; // 如果创建设备对象失败，则返回
            }

            PCI8501.PCI8501_GetDDR2Length(hDevice, ref  ulDDR2Length);
            InputRange = SelectInputRange();  // 要求用户从键盘上选择输入量程

            ADPara.Frequency = 100000; // 采样频率(Hz)
            ADPara.InputRange0 = InputRange;  // 模拟通道0输入量程
            ADPara.InputRange1 = InputRange;  // 模拟通道1输入量程
            ADPara.InputRange2 = InputRange;  // 模拟通道2输入量程
            ADPara.InputRange3 = InputRange;  // 模拟通道3输入量程
            ADPara.InputRange4 = InputRange;  // 模拟通道4输入量程
            ADPara.InputRange5 = InputRange;  // 模拟通道5输入量程
            ADPara.InputRange6 = InputRange;  // 模拟通道6输入量程
            ADPara.InputRange7 = InputRange;  // 模拟通道7输入量程
            ADPara.Gains0 = 0; // 允许通道0采样
            ADPara.Gains1 = 0; // 允许通道1采样
            ADPara.Gains2 = 0; // 允许通道2采样
            ADPara.Gains3 = 0; // 允许通道3采样	
            ADPara.Gains4 = 0; // 允许通道4采样
            ADPara.Gains5 = 0; // 允许通道5采样
            ADPara.Gains6 = 0; // 允许通道6采样
            ADPara.Gains7 = 0; // 允许通道7采样
            ADPara.M_Length = 0;						// M长度
            ADPara.N_Length = 1024;				// N长度
            ADPara.TriggerMode = PCI8501.PCI8501_TRIGMODE_MIDL;	// 硬件中间触发(包括预触发、后触发功能)
            ADPara.TriggerSource = PCI8501.PCI8501_TRIGMODE_SOFT;	// 软件触发
            ADPara.TriggerDir = PCI8501.PCI8501_TRIGDIR_NEGATIVE; // 下降沿触发
            ADPara.TrigLevelVolt = 0;
            ADPara.ClockSource = PCI8501.PCI8501_CLOCKSRC_IN;		// 使用内部时钟
            ADPara.bClockSourceDir = 0;
            ADPara.OutClockSource = PCI8501.PCI8501_OUTCLOCKSRC_TRIGGER0;


            if (!(PCI8501.PCI8501_InitDeviceAD(hDevice, ref ADPara) > 0))// 初始化硬件
            {
                Console.WriteLine("PCI8501_InitDeviceAD Error...\n");
                _getch();
                goto ExitRead;
            }

            nReadSizeWords = (ADPara.M_Length + ADPara.N_Length) * 8; // M加N长度为读取数据的长度

            UInt32 ulStartAddr = 0;
            

            while (!(Convert.ToBoolean(_kbhit())))
            {
                if (!(PCI8501.PCI8501_StartDeviceAD(hDevice) > 0)) // 启动设备
                {
                    Console.WriteLine("StartDeviceAD Error...\n");
                    _getch();
                }
                if (!(PCI8501.PCI8501_SetDeviceTrigAD(hDevice) > 0)) // 触发AD
                {
                    Console.WriteLine("SetDeviceTrigAD Error...\n");
                    _getch();
                }
                ulStartAddr = 0;
                nReadSize = nReadSizeWords;
                bFirstWait = true;
                while (true) // 查询当前物理缓冲区数据是否已准备就绪
                {
                    if (!(PCI8501.PCI8501_GetDevStatusAD(hDevice, ref ADStatus) > 0))
                    {
                        Console.WriteLine("GetDevStatusAD error...\n");
                        _getch();
                        goto ExitRead;
                    }

                    if (ADStatus.bComplete == 1)
                    {
                        break;  // 若板载FIFO存储器数据量达到半满以上，则退出状态查询,开始读取半满数据
                    }
                    else
                    {
                        if (bFirstWait)
                        {
                            Console.WriteLine("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n");
                            bFirstWait = false;
                        }
                        if (_kbhit() == 1) goto ExitRead; // 如果用户按键，则退出
                    }
                }

                if (!(PCI8501.PCI8501_StopDeviceAD(hDevice) > 0)) // 停止设备
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
                            // 将原码转换为电压值
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
                            Console.Write("[AI{0}]={1}    ", nADChannel, strTemp); // 显示电压值					
                            Index++;
                        }
                        Console.WriteLine("\n");
                    } // for(Index=0; Index<64; Index++)
                }

            }

        ExitRead:
            PCI8501.PCI8501_StopDeviceAD(hDevice); // 停止AD
            PCI8501.PCI8501_ReleaseDeviceAD(hDevice); // 释放AD
            PCI8501.PCI8501_ReleaseDevice(hDevice); // 释放设备对象
            return;
        }


        //////////////////////////////////////////////////////
        // 获取用户选择的输入量程
        static int SelectInputRange()
        {
            Int32 InputRange;
        Repeat:
            Console.WriteLine("\n");
            Console.WriteLine("0. -10V ～ +10V\n");
            Console.WriteLine("1. -5V ～ +5V\n");
            Console.WriteLine("2. -2500mV ～ +2500mV\n");
            Console.WriteLine("3. 0mV ～ +10000mV\n");
            Console.WriteLine("4. 0mV ～ +5000mV\n");
            Console.WriteLine("Please Select Input Range[0-4]:");
            InputRange = Convert.ToInt32(Console.ReadLine());
            if (InputRange < 0 || InputRange > 4) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
            return InputRange;
        }
    }
}
                