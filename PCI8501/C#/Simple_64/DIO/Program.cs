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

        static void Main(string[] args)
        {	
            IntPtr hDevice;  // 设备对象句柄
            Int32 DeviceLgcID;   // 物理设备ID号(由板上JP1决定)

            Byte[] bDOSts = new Byte[8];   // 对应开关量输出的各通道
            Byte[] bDISts = new Byte[8];   // 对应开关量输入的各通道

            DeviceLgcID = 0;
            hDevice = (IntPtr)(-1);
            hDevice = PCI8501.PCI8501_CreateDevice(DeviceLgcID); // 根据物理ID号创建设备对象
            if(hDevice == (IntPtr)(-1))
            {
	            Console.WriteLine("Create Device Error...");
	            _getch();
	            return ;
            }

            // 预置开关量输出的状态参数
            bDOSts[0] = 0;
            bDOSts[1] = 1;
            bDOSts[2] = 0;
            bDOSts[3] = 1;
            bDOSts[4] = 0;
            bDOSts[5] = 1;
            bDOSts[6] = 0;
            bDOSts[7] = 1;
            
        Repeat:
            Console.WriteLine("Press any key to SetDeviceDO...");
            _getch();

            
            if (!(PCI8501.PCI8501_SetDeviceDO(hDevice, bDOSts) > 0))  // 开关量输出
            {
                Console.WriteLine("GetDeviceDO Error");
                _getch();
                goto Exit;
            }

            if (bDOSts[0] == 1) Console.WriteLine("DO0 = On"); else Console.WriteLine("DO0 = Off");
            if (bDOSts[1] == 1) Console.WriteLine("DO1 = On"); else Console.WriteLine("DO1 = Off");
            if (bDOSts[2] == 1) Console.WriteLine("DO2 = On"); else Console.WriteLine("DO2 = Off");
            if (bDOSts[3] == 1) Console.WriteLine("DO3 = On"); else Console.WriteLine("DO3 = Off");
            if (bDOSts[4] == 1) Console.WriteLine("DO4 = On"); else Console.WriteLine("DO4 = Off");
            if (bDOSts[5] == 1) Console.WriteLine("DO5 = On"); else Console.WriteLine("DO5 = Off");
            if (bDOSts[6] == 1) Console.WriteLine("DO6 = On"); else Console.WriteLine("DO6 = Off");
            if (bDOSts[7] == 1) Console.WriteLine("DO7 = On"); else Console.WriteLine("DO7 = Off");
            

            Console.WriteLine( "Press any key to get DI" );
            _getch();

            if (!(PCI8501.PCI8501_GetDeviceDI(hDevice, bDISts) > 0))  // 开关量输入
            {
	            Console.WriteLine("GetDeviceDI Error");
	            _getch();
	            goto Exit;
            }

            if (bDISts[0] == 1) Console.WriteLine("DI0 = On"); else Console.WriteLine("DI0 = Off");
            if (bDISts[1] == 1) Console.WriteLine("DI1 = On"); else Console.WriteLine("DI1 = Off");
            if (bDISts[2] == 1) Console.WriteLine("DI2 = On"); else Console.WriteLine("DI2 = Off");
            if (bDISts[3] == 1) Console.WriteLine("DI3 = On"); else Console.WriteLine("DI3 = Off");
            if (bDISts[4] == 1) Console.WriteLine("DI4 = On"); else Console.WriteLine("DI4 = Off");
            if (bDISts[5] == 1) Console.WriteLine("DI5 = On"); else Console.WriteLine("DI5 = Off");
            if (bDISts[6] == 1) Console.WriteLine("DI6 = On"); else Console.WriteLine("DI6 = Off");
            if (bDISts[7] == 1) Console.WriteLine("DI7 = On"); else Console.WriteLine("DI7 = Off");
            
            Console.WriteLine();
            char Key;
            Console.WriteLine("Press any to continue, ESC to quit");
            Key = Convert.ToChar(_getch());
            if(Key != 27) goto Repeat;

            Exit:
            PCI8501.PCI8501_ReleaseDevice(hDevice);


        }
    }
}
