using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    class Program
    {
        [DllImport("msvcrt.dll")]
        public static extern int _getch();
        [DllImport("msvcrt.dll")]
        public static extern int _kbhit();
        

        //////////////////////////////////////////////////////
        // 获取用户选择的输入量程
        public static int SelectInputRange()
        {
	        Int32 InputRange;
        Repeat:
            Console.WriteLine();
            Console.Write("0. -10V ~ +10V\n");
            Console.Write("1. -5V ~ +5V\n");
            Console.Write("2. -2.5V ~ +2.5V\n");
            Console.Write("3. 0V ~ +10V\n");
            Console.WriteLine("4. 0V ~ +5V\n");

            Console.Write("Please Select Input Range[0-4]:");
            InputRange = Convert.ToInt32(Console.ReadLine());
            if (InputRange < 0 || InputRange >4) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
            return InputRange;
        }
                 
        static PCI9757.PCI9757_PARA_AD ADPara; // 硬件参数
        static void Main(string[] args)
        {          
            IntPtr				hDevice;
            Int32				DeviceLgcID;
            Int32[] ADBuffer = new Int32[8192];	// 分配缓冲区(存储原始数据)
            Int32				nReadSizeWords;   // 每次读取AD数据的长度(字)
            Int32				nRetSizeWords = 0;
            UInt32				ADData;
            float				fVolt=0.0f;
            Int32 InputRange;
            Int32 Index;
            UInt32 nChannel;
            UInt32 nCurrentChannel;
            Int32[] bChannelArray = new Int32[8];	// 分配缓冲区(存储原始数据)

	        DeviceLgcID = 0;
            hDevice = PCI9757.PCI9757_CreateDevice(DeviceLgcID); // 创建设备对象
	        if(hDevice == (IntPtr)(-1))
	        {
                Console.WriteLine("PCI9757_CreateDevice Error");
		        _getch();
		        return ; // 如果创建设备对象失败，则返回
	        }

	        InputRange = SelectInputRange(); // 要求用户从键盘上选择输入量程

            	// 预置硬件参数
            ADPara.bChannelArray0 = 1; // 允许通道0采样
            ADPara.bChannelArray1 = 1; // 允许通道1采样
            ADPara.bChannelArray2 = 1; // 允许通道2采样
            ADPara.bChannelArray3 = 1; // 允许通道3采样

            ADPara.InputRange0 = InputRange;  // 模拟通道输入量程	
            ADPara.InputRange1 = InputRange;
            ADPara.InputRange2 = InputRange;
            ADPara.InputRange3 = InputRange;

            ADPara.Frequency        = 10000; // 采样频率(Hz)
	        ADPara.TriggerSource	= PCI9757.PCI9757_TRIGMODE_SOFT; // 触发源
	        ADPara.TriggerType		= PCI9757.PCI9757_TRIGTYPE_EDGE; // 触发类型为边沿触发	
            ADPara.TriggerDir       = PCI9757.PCI9757_TRIGDIR_NEGATIVE; // 触发方向为负向
            ADPara.TrigLevelVolt = 2000;
            ADPara.TrigWindow = 40;    // 触发灵敏度	
            ADPara.ClockSource = PCI9757.PCI9757_CLOCKSRC_IN; // 时钟源选用板内时钟源

            bChannelArray[0] = ADPara.bChannelArray0;
            bChannelArray[1] = ADPara.bChannelArray1;
            bChannelArray[2] = ADPara.bChannelArray2;
            bChannelArray[3] = ADPara.bChannelArray3;
            if (PCI9757.PCI9757_InitDeviceProAD(hDevice, ref ADPara)==false) // 初始化硬件
            {
                Console.WriteLine("PCI9757_InitDeviceProAD...\n");
                _getch();
                return ;
            }

            int ChannelCount = 0;
            for (nChannel = 0; nChannel < 8; nChannel++)
            {
                if (bChannelArray[nChannel] == 1)
                {
                    ChannelCount++;
                }
            }

            nReadSizeWords = 4096 - 4096 % ChannelCount; // 将数据长度字转换为双字	

            if (!PCI9757.PCI9757_StartDeviceProAD(hDevice)) // 启动设备
            {
                Console.WriteLine("StartDeviceAD Error...\n");
                _getch();
            }
          
            while (_kbhit()==0)
            {
                   
                PCI9757.PCI9757_ReadDeviceProAD_Npt(hDevice, ADBuffer, nReadSizeWords, ref nRetSizeWords);
                UInt32 nTChannel = 0;
                for (Index = 0; Index < 64; Index++)
                {
                    for (nChannel = nTChannel; nChannel < 4; nChannel++)
                    {
                        if (bChannelArray[nChannel] == 1) // 如果此通道被采样
                        {
                            nCurrentChannel = nChannel; // 记录此通道号
                            nTChannel = nChannel + 1;
                            if (_kbhit()!=0) goto ExitRead0;
                            // 将原码高4位屏蔽掉
                            ADData = Convert.ToUInt32(ADBuffer[Index] & 0xFFFF);
                            switch (InputRange)
                            {
                                case PCI9757.PCI9757_INPUT_N10000_P10000mV: // -10V - +10V
                                    fVolt = (float)((20000.0 / 65536) * ADData - 10000.0);
                                    break;
                                case PCI9757.PCI9757_INPUT_N5000_P5000mV: // -5V - +5V
                                    fVolt = (float)((10000.0 / 65536) * ADData - 5000.0);
                                    break;
                                case PCI9757.PCI9757_INPUT_N2500_P2500mV: // -5V - +5V
                                    fVolt = (float)((5000.0 / 65536) * ADData - 2500.0);
                                    break;
                                case PCI9757.PCI9757_INPUT_0_P10000mV: // -1V - +1V
                                    fVolt = (float)((10000.0 / 65536) * ADData);
                                    break;
                                case PCI9757.PCI9757_INPUT_0_P5000mV: // -1V - +1V
                                    fVolt = (float)((5000.0 / 65536) * ADData);
                                    break;
                                default:
                                    break;
                            }
                            Console.Write("CH{0}={1}\t", nCurrentChannel, fVolt.ToString("###0.00")); // 显示电压值
                            if ((Index + 1) % ChannelCount == 0)
                            {
                                nTChannel = 0;
                                Console.WriteLine("");
                            }
                            break;
                        }
                    }
                } // for(Index=0; Index<64; Index++)
            }
        ExitRead0:
            PCI9757.PCI9757_ReleaseDeviceProAD(hDevice); // 释放AD
            PCI9757.PCI9757_ReleaseDevice(hDevice); // 释放设备对象
 
        }
    }
}
