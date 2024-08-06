//****************************************************************
// ��������ʾAO������� �ڲ�ʱ�� �����������
//****************************************************************
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
            Double[] data = new Double[4000];
            Int32 nSamplePerChanWrite;
            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strChannelName = string.Empty;
            string strSource = string.Empty;

            for (int index = 0; index < 4000; index++)
            {
                data[index] = 5.0 * index / 4000.0;
            }

            /*********************************************/
            // ArtDAQ Configure Code
            /*********************************************/
            Console.WriteLine("������������:(��:task1,���ֻ��һ�������������Ϊ��)");
            strTaskName = Console.ReadLine();
            //step1: CreateTask
            error = ArtDAQ.ArtDAQ_CreateTask(strTaskName, out taskHandle);

            Console.WriteLine("������忨��DMC��ʶ���������:(��:Dev1)");
            strCardName = Console.ReadLine();
            Console.WriteLine("������ͨ����:(��:ao0)");
            strChannelName = Console.ReadLine();
            strChannelName = strCardName + "/" + strChannelName;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateAOVoltageChan(taskHandle, strChannelName, "", -10.0, 10.0, ArtDAQ.ArtDAQ_Val_Volts, null);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step3: Configure Clock
            error = ArtDAQ.ArtDAQ_CfgSampClkTiming(taskHandle, "", 1000.0, ArtDAQ.ArtDAQ_Val_Rising, ArtDAQ.ArtDAQ_Val_FiniteSamps, 4000);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step4:Write Code
            error = ArtDAQ.ArtDAQ_WriteAnalogF64(taskHandle, 4000, 0, 10.0, ArtDAQ.ArtDAQ_Val_GroupByChannel, data, out nSamplePerChanWrite, (IntPtr)0);

            //step5: Start Task
            error = ArtDAQ.ArtDAQ_StartTask(taskHandle);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step6: Wait until task done
            error = ArtDAQ.ArtDAQ_WaitUntilTaskDone(taskHandle, 10.0);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            Console.WriteLine("��������˳�");
            _getch();

            //step7: Exit ReleaseTask
            Exit:
             ArtDAQ.ArtDAQ_StopTask(taskHandle);
             ArtDAQ.ArtDAQ_ClearTask(taskHandle);
             _getch();
             return;

        }
    }
}

