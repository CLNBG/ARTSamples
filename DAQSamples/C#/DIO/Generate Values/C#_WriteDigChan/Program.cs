//*****************************************************************
// ��������ʾDO line��������
//*****************************************************************
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
            byte[] data = new byte[8] { 1,0, 0, 1, 1, 1, 1, 1};
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
            Console.WriteLine("������line��:(��:line0��line0:7)");
            strLineName = Console.ReadLine();
            strLineName = strCardName + "/" + strPortName + "/" + strLineName;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateDOChan(taskHandle, strLineName, "", ArtDAQ.ArtDAQ_Val_ChanForAllLines);
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

            //step4: Write
            error = ArtDAQ.ArtDAQ_WriteDigitalLines(taskHandle, 1, 1, 10.0, ArtDAQ.ArtDAQ_Val_GroupByChannel, data, out read, (IntPtr)0);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            Console.WriteLine("��������˳�...");
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

