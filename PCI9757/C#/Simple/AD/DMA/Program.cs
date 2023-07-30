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
        [DllImport("Kernel32.dll")]
        public static extern int WaitForSingleObject(IntPtr hHandle, int dwMillisenconds);


        public const int SEGMENT_COUNT = 64;
        public const int HALF_SIZE_WORDS = 4096;

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
            if (InputRange < 0 || InputRange > 4) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
            return InputRange;
        }

        static PCI9757.PCI9757_PARA_AD ADPara; // 硬件参数
        static PCI9757.PCI9757_STATUS_DMA DMAStatus; // DMA状态参数

        static void Main(string[] args)
        {
            IntPtr hDevice;
            IntPtr hDmaEvent;
            Int32 DeviceLgcID;          
            Int32[,] ADBuffer = new Int32[64,4096];
        
            UInt32 ADData;
            float fVolt = 0.0f;
            Int32 InputRange;
            bool bFirstWait;
            Int32 Index;           
            Int32 nChannel = 0;
            Int32 g_ulSegmentID = 0;
            Int32[] bChannelArray = new Int32[8];
            Int32[] bSegmentSts = new Int32[64];

            DeviceLgcID = 0;
            hDevice = PCI9757.PCI9757_CreateDevice(DeviceLgcID); // 创建设备对象
            if (hDevice == (IntPtr)(-1))
            {
                Console.WriteLine("PCI9757_CreateDevice Error");
                _getch();
                return; // 如果创建设备对象失败，则返回
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

            ADPara.Frequency = 10000; // 采样频率(Hz)	        
            ADPara.TriggerSource = PCI9757.PCI9757_TRIGMODE_SOFT; // 触发源
            ADPara.TriggerType = PCI9757.PCI9757_TRIGTYPE_EDGE; // 触发类型为边沿触发	
            ADPara.TriggerDir = PCI9757.PCI9757_TRIGDIR_NEGATIVE; // 触发方向为负向
            ADPara.TrigLevelVolt = 2000;
            ADPara.TrigWindow = 40;    // 触发灵敏度	
            ADPara.ClockSource = PCI9757.PCI9757_CLOCKSRC_IN; // 时钟源选用板内时钟源

            hDmaEvent = PCI9757.PCI9757_CreateSystemEvent();
            if (PCI9757.PCI9757_InitDeviceDmaAD(hDevice, hDmaEvent, ADBuffer, 4096, SEGMENT_COUNT, HALF_SIZE_WORDS, ref ADPara) == false) // 初始化硬件
            {
                Console.WriteLine("PCI9757_InitDeviceDmaAD error...");
                _getch();
                goto ExitRead1;
            }

            PCI9757.PCI9757_StartDeviceDmaAD(hDevice); // 启动设备
            while (_kbhit() == 0)
            {
                bFirstWait = true;
                while (true) // 查询当前物理缓冲区数据是否已准备就绪
                {
                    if (WaitForSingleObject(hDmaEvent, 100) == 0) break; // 等待DMA事件
                    else
                    {
                        if (bFirstWait)
                        {
                            Console.WriteLine("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n");
                            bFirstWait = false;
                        }
                        if (_kbhit() != 0) goto ExitRead0;
                    }
                }

                if (PCI9757.PCI9757_GetDevStatusDmaAD(hDevice, ref DMAStatus) == false)
                {
                    Console.WriteLine("PCI9757_GetDevStatusDmaAD error...");
                    goto ExitRead0;
                }

                if (DMAStatus.bBufferOverflow == 1)
                {
                    Console.WriteLine("DMA Overflow...");
                }
                int i = 0;
               
                // 根据回读状态，检查所有的缓冲段是否可用，然后依次处理
              //  for (i = 0; i < SEGMENT_COUNT && DMAStatus.bSegmentSts[g_ulSegmentID]; i++)
                for (i = 0; i < 32; i++)
                {
                    if (DMAStatus.bSegmentSts[g_ulSegmentID] == 1)
                    {                                           
                        for (Index = 0; Index < 64; Index++)
                        {
                            if (_kbhit() == 1)
                            {
                                goto ExitRead0;
                            }
                            // 将原码高2位屏蔽掉
                            ADData = Convert.ToUInt32(ADBuffer[g_ulSegmentID, Index] & 0xFFFF);
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

                            Console.Write("CH{0}={1}\t", nChannel, fVolt.ToString("###0.00")); // 显示电压值

                            nChannel++;
                            if (nChannel >= 4)
                            {
                                nChannel = 0;
                                Console.WriteLine("");// 到末通道时换行
                            }
                        }//end for 64
                        // 将刚处理完成的缓冲区页标志复位，通知硬件及驱动该缓冲区页已被处理
                        if (!PCI9757.PCI9757_SetDevStatusDmaAD(hDevice, g_ulSegmentID))
                        {
                            Console.WriteLine("SetDevStatusDmaAD Error...\n");
                            _getch();
                            goto ExitRead0;
                        }
                        g_ulSegmentID = (g_ulSegmentID + 1) % SEGMENT_COUNT;
                    }//end  if (DMAStatus.bSegmentSts[g_ulSegmentID] == 1)

                } // end for 32     

            }//end while
        ExitRead0:
            _getch();
            PCI9757.PCI9757_ReleaseDeviceDmaAD(hDevice); // 释放AD
            PCI9757.PCI9757_ReleaseSystemEvent(hDmaEvent);
        ExitRead1:
            PCI9757.PCI9757_ReleaseDevice(hDevice); // 释放设备对象
            _getch();
        }
    }
}
