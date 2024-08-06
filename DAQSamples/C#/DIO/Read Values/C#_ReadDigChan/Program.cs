//*************************************************************
// ��������ʾDI��������
//*************************************************************
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

        public static void GetErrorString(Int32 errorCode)
        {
            if (errorCode < 0)
            {
                byte[] errorInfo = new byte[2048];
                ArtDAQ.ArtDAQ_GetExtendedErrorInfo(errorInfo, 2048);
                string str = Encoding.Default.GetString(errorInfo);
                Console.WriteLine("������ϢΪ:{0}", str);
            }
            return;     
        }

        static void Main(string[] args)
        {
           
            Int32 error = 0;
            string errorInfo = new string('0', 2048);
            IntPtr taskHandle = (IntPtr)(0);
            Int32 read = 0;
            Int32 bytesPerSamp;
            byte[] data = new byte[100];
            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strPortName = string.Empty;
            string strLineName = string.Empty;

            /*********************************************/
            // ArtDAQ Configure Code
            /*********************************************/
            Console.WriteLine("������������:(��:task1,���ֻ��һ�������������Ϊ��)");
            strTaskName = Console.ReadLine();
            //step1: CreateTask
            error = ArtDAQ.ArtDAQ_CreateTask(strTaskName, out taskHandle);

            Console.WriteLine("������忨��DMC��ʶ���������:(��:Dev1)");
            strCardName = Console.ReadLine();
            Console.WriteLine("������˿���:(��:port0)");
            strPortName = Console.ReadLine();
            Console.WriteLine("������Line��:(��:line0:7)");
            strLineName = Console.ReadLine();
            strLineName = strCardName + "/" + strPortName + "/" + strLineName;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateDIChan(taskHandle, strLineName, "", ArtDAQ.ArtDAQ_Val_ChanForAllLines);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step3: Start Task
            error = ArtDAQ.ArtDAQ_StartTask(taskHandle);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step4: Read
            error = ArtDAQ.ArtDAQ_ReadDigitalLines(taskHandle, 1, 10.0, ArtDAQ.ArtDAQ_Val_GroupByChannel, data, 100, out read, out bytesPerSamp, (IntPtr)0);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }
            for (int k = 0; k < read; k++)
            {
                Console.WriteLine("DI data={0}", data[k]);
            }

            Console.WriteLine("��������˳�");
            _getch();

            //step5: Exit ReleaseTask
            Exit:
             ArtDAQ.ArtDAQ_StopTask(taskHandle);
             ArtDAQ.ArtDAQ_ClearTask(taskHandle);
             _getch();
             return;

        }
    }
}

