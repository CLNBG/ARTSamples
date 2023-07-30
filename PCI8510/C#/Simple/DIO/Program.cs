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

        static void Main(string[] args)
        {
            Int32 key;
            IntPtr hDevice;
            Int32 DeviceID = 0;
            byte[] bDOSts = new byte[8];
            byte[] bDISts = new byte[8];
            bDOSts[0] = 0;      // 设置开关量输出的状态
            bDOSts[1] = 1;
            bDOSts[2] = 0;
            bDOSts[3] = 1;
            bDOSts[4] = 0;
            bDOSts[5] = 1;
            bDOSts[6] = 0;
            bDOSts[7] = 1;


            hDevice = PCI8510.PCI8510_CreateDevice(DeviceID);
            if (hDevice == (IntPtr)(-1))
            {
                Console.WriteLine("Create Error...\n");
                _getch();
                return;
            }
        read:
            PCI8510.PCI8510_SetDeviceDO(hDevice, bDOSts); // 开关量输出
            Console.WriteLine("Press any key to continue get DO...");
            _getch();
            Console.WriteLine("\n\n");
            // 反应时间比较长，所以需要一定的延时
            if (bDOSts[0] == 1) Console.WriteLine("DO0 = On\n"); else Console.WriteLine("DO0 = Off\n");
            if (bDOSts[1] == 1) Console.WriteLine("DO1 = On\n"); else Console.WriteLine("DO1 = Off\n");
            if (bDOSts[2] == 1) Console.WriteLine("DO2 = On\n"); else Console.WriteLine("DO2 = Off\n");
            if (bDOSts[3] == 1) Console.WriteLine("DO3 = On\n"); else Console.WriteLine("DO3 = Off\n");
            if (bDOSts[4] == 1) Console.WriteLine("DO4 = On\n"); else Console.WriteLine("DO4 = Off\n");
            if (bDOSts[5] == 1) Console.WriteLine("DO5 = On\n"); else Console.WriteLine("DO5 = Off\n");
            if (bDOSts[6] == 1) Console.WriteLine("DO6 = On\n"); else Console.WriteLine("DO6 = Off\n");
            if (bDOSts[7] == 1) Console.WriteLine("DO7 = On\n"); else Console.WriteLine("DO7 = Off\n");

            Console.WriteLine("Press any key to continue get DI...");
            _getch();
            Console.WriteLine("\n\n");

            PCI8510.PCI8510_GetDeviceDI(hDevice, bDISts);  // 开关量输入

            if (bDISts[0] == 1) Console.WriteLine("DI0 = On\n"); else Console.WriteLine("DI0 = Off\n");
            if (bDISts[1] == 1) Console.WriteLine("DI1 = On\n"); else Console.WriteLine("DI1 = Off\n");
            if (bDISts[2] == 1) Console.WriteLine("DI2 = On\n"); else Console.WriteLine("DI2 = Off\n");
            if (bDISts[3] == 1) Console.WriteLine("DI3 = On\n"); else Console.WriteLine("DI3 = Off\n");
            if (bDISts[4] == 1) Console.WriteLine("DI4 = On\n"); else Console.WriteLine("DI4 = Off\n");
            if (bDISts[5] == 1) Console.WriteLine("DI5 = On\n"); else Console.WriteLine("DI5 = Off\n");
            if (bDISts[6] == 1) Console.WriteLine("DI6 = On\n"); else Console.WriteLine("DI6 = Off\n");
            if (bDISts[7] == 1) Console.WriteLine("DI7 = On\n"); else Console.WriteLine("DI7 = Offn");
            
            key = _getch();
            if (key != 27) goto read;

            PCI8510.PCI8510_ReleaseDevice(hDevice);

            return;
        }












    }
}