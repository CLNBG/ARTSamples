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
        public static PCI8510.PCI8510_PARA_AD ADPara; // 硬件参数
        public static PCI8510.PCI8510_STATUS_DMA DMAStatus;
        static void Main(string[] args)
        {

            UInt32[,] ADBuffer = new UInt32[32, 4096];
            IntPtr hDevice;
            Int32 DeviceID;
            UInt32 nReadSizeWords;   // 每次读取AD数据的长度(字)
            Int32 nRetSizeWords;
            Int32 Index;
            IntPtr hDmaEvent;
            Int32 InputRange;
          //  Int32 nChannel ;
            float fVolt;
            Boolean bFirstWait;
            Int32 g_ulSegmentID;
            Int32[] DMASta = new Int32[32];
            fVolt = 0;
            
            g_ulSegmentID = 0;
            DeviceID = 0;
            hDevice = PCI8510.PCI8510_CreateDevice(DeviceID); // 创建设备对象
            if (hDevice == (IntPtr)(-1))
            {
                Console.WriteLine("Create Error...\n");
                _getch();
                return;
            }

        Repeat:
            Console.WriteLine("\n");
            Console.WriteLine("\n");
            Console.WriteLine("0. 0V ～ +5V\n");
            Console.WriteLine("1. 0V ～ +10V\n");
            Console.WriteLine("2. -5V ～ +5V\n");
            Console.WriteLine("3. -10V ～ +10V\n");

            Console.WriteLine("Please Select Input Range[0-3]:");
            InputRange = Convert.ToInt32(Console.ReadLine());
            if (InputRange < 0 || InputRange > 3) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择



            //memset(&ADPara, 0x00, sizeof(ADPara)); // 初始化成已知量(强烈建议)

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
            hDmaEvent = PCI8510.PCI8510_CreateSystemEvent();
            if (!PCI8510.PCI8510_InitDeviceDmaAD(hDevice, hDmaEvent,  ADBuffer, 4096, 32, 4096, ref ADPara))
            {
                Console.WriteLine("PCI8510_InitDeviceProAD...\n");
                _getch();
                return;
            }

            //nReadSizeWords = 4096;//数据长度字转换为双字	

            if (!PCI8510.PCI8510_StartDeviceProAD(hDevice)) // 启动设备
            {
                Console.WriteLine("StartDeviceAD Error...\n");
                _getch();
            }
            while (_kbhit() == 0)
            {
                bFirstWait = true;
                while (true) // 查询当前物理缓冲区数据是否已准备就绪
                {
                    if (WaitForSingleObject(hDmaEvent, 100) == WAIT_OBJECT_0) break; // 等待DMA事件
                    else
                    {
                        if (bFirstWait)
                        {
                            Console.WriteLine("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n");
                            bFirstWait = false;
                        }
                        if (_kbhit() != 0)
                        {
                            goto ExitRead0;
                        }
                    }
                }

                if (!PCI8510.PCI8510_GetDevStatusDmaAD(hDevice, ref DMAStatus))
                {
                    Console.WriteLine("Get Device Status Error...\n");
                    goto ExitRead0;
                }
                DMASta[0] = DMAStatus.bSegmentSts0;
            DMASta[1] = DMAStatus.bSegmentSts1;
            DMASta[2] = DMAStatus.bSegmentSts2;
            DMASta[3] = DMAStatus.bSegmentSts3;
            DMASta[4] = DMAStatus.bSegmentSts4;
            DMASta[5] = DMAStatus.bSegmentSts5;
            DMASta[6] = DMAStatus.bSegmentSts6;
            DMASta[7] = DMAStatus.bSegmentSts7;
            DMASta[8] = DMAStatus.bSegmentSts8;
            DMASta[9] = DMAStatus.bSegmentSts9;

            DMASta[10] = DMAStatus.bSegmentSts10;
            DMASta[11] = DMAStatus.bSegmentSts11;
            DMASta[12] = DMAStatus.bSegmentSts12;
            DMASta[13] = DMAStatus.bSegmentSts13;
            DMASta[14] = DMAStatus.bSegmentSts14;
            DMASta[15] = DMAStatus.bSegmentSts15;
            DMASta[16] = DMAStatus.bSegmentSts16;
            DMASta[17] = DMAStatus.bSegmentSts17;
            DMASta[18] = DMAStatus.bSegmentSts18;
            DMASta[19] = DMAStatus.bSegmentSts19;

            DMASta[20] = DMAStatus.bSegmentSts20;
            DMASta[21] = DMAStatus.bSegmentSts21;
            DMASta[22] = DMAStatus.bSegmentSts22;
            DMASta[23] = DMAStatus.bSegmentSts23;
            DMASta[24] = DMAStatus.bSegmentSts24;
            DMASta[25] = DMAStatus.bSegmentSts25;
            DMASta[26] = DMAStatus.bSegmentSts26;
            DMASta[27] = DMAStatus.bSegmentSts27;
            DMASta[28] = DMAStatus.bSegmentSts28;
            DMASta[29] = DMAStatus.bSegmentSts29;

            DMASta[30] = DMAStatus.bSegmentSts30;
            DMASta[31] = DMAStatus.bSegmentSts31;

                if (DMAStatus.bBufferOverflow == 1)
                {
                    Console.WriteLine("DMA Overflow...\n");
                    // Sleep(3000); // 当溢出时,为了让用户看得清楚,延时3秒
                }

                for (int i = 0; i < 32 ; i++)
                {
                    if (DMASta[g_ulSegmentID] > 0)
                    {
                        for ( Index = 0; Index < 8; )
                        {
                            
                                if (_kbhit() != 0)
                                {
                                    goto ExitRead0;
                                }
                                // 将原码转换为电压值
                                switch (InputRange)
                                {
                                    case PCI8510.PCI8510_INPUT_0_P5000mV: // -5V - +5V
                                        fVolt = (float)((5000.0 / 65536) * (ADBuffer[g_ulSegmentID,Index] & 0xFFFF));
                                        break;
                                    case PCI8510.PCI8510_INPUT_0_P10000mV: // -10V - +10V
                                        fVolt = (float)((10000.0 / 65536) * (ADBuffer[g_ulSegmentID,Index] & 0xFFFF));
                                        break;
                                    case PCI8510.PCI8510_INPUT_N5000_P5000mV: // -5V - +5V
                                        fVolt = (float)((10000.0 / 65536) * (ADBuffer[g_ulSegmentID,Index] & 0xFFFF) - 5000.0);
                                        break;
                                    case PCI8510.PCI8510_INPUT_N10000_P10000mV: // -10V - +10V
                                        fVolt = (float)((20000.0 / 65536) * (ADBuffer[g_ulSegmentID,Index] & 0xFFFF) - 10000.0);
                                        break;
                                    default:
                                        break;
                                }
                                Console.Write("CH{0}=  {1}    ", Index, fVolt.ToString("0.##")); // 显示电压值
                                if ((Index + 1) % 4 == 0)
                                {
                                    Console.WriteLine("\n"); // 显示电压值
                                }
                                Index++;
                            
                           
                        } // for(Index=0; Index<64; Index++)

                        if (!(PCI8510.PCI8510_SetDevStatusDmaAD(hDevice, g_ulSegmentID) ))
                        {

                            Console.WriteLine("PCI8510_SetDevStatusDmaAD Error");
                            _getch();
                            goto ExitRead1;
                        }

                        g_ulSegmentID = (g_ulSegmentID + 1) % 32;
                    } // for(Index=0; Index<64; Index++)
                } // end of for(SegmentID=0; SegmentID<SEGMENT_COUNT; SegmentID++)
            } // end of while(!kbhit())


        ExitRead0:
            _getch();
            PCI8510.PCI8510_ReleaseDeviceDmaAD(hDevice); // 释放AD
            PCI8510.PCI8510_ReleaseSystemEvent(hDmaEvent);
        ExitRead1:
            PCI8510.PCI8510_ReleaseDevice(hDevice); // 释放设备对象
            _getch();
            return;

        }
    }
}