// calibrate

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

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
            IntPtr taskHandle = IntPtr.Zero;
            bool bError = false;
            string resourceName;

            Console.Write("Please enter the device name:");
            resourceName = Console.ReadLine();

            if (ART_SCOPE.ArtScope_init(resourceName, ref taskHandle) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_Calibration(taskHandle) < 0)
            {
                bError = true;
                goto Error_close;
            }

        Error_close:
            // Get wrong information
            if (bError == true)
            {
                byte[] strError = new byte[2048];
                ART_SCOPE.ArtScope_GetExtendedErrorInfo(strError, 2048);
                string str = System.Text.Encoding.Default.GetString(strError);
                Console.WriteLine(str.Replace("\n\n", "\n").Replace("\0", ""));
                if (taskHandle != IntPtr.Zero)
                {
                    ART_SCOPE.ArtScope_Close(taskHandle);
                }
                Console.WriteLine("Press any key to quit...");
                _getch();
                return;
            }
            ART_SCOPE.ArtScope_Close(taskHandle);


            Console.WriteLine("Press any key to quit...");
            _getch();
        }
    }
}
