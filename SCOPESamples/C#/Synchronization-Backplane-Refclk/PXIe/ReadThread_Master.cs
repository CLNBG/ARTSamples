using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace Sys
{
    public class ReadThread_Master
    {
        private IntPtr hDevice;
        private double fTimeout;
        private uint m_SampleLength;
        private uint m_Wfms;
        private ushort[] databuffer;
        private double fVolt;
        private ManualResetEvent hEvent;

        public ReadThread_Master(IntPtr hTask,
                                 double timeout,
                                 uint samplelength,
                                 uint numwave,
                                 ushort[] wavebuffer,
                                 ManualResetEvent hevent)
        {
            hDevice = hTask;
            fTimeout = timeout;
            m_SampleLength = samplelength;
            m_Wfms = numwave;
            databuffer = wavebuffer;
            hEvent = hevent;
        }

        public void ReadData()
        {
            ART_SCOPE.ArtScope_wfmInfo wfminfo = new ART_SCOPE.ArtScope_wfmInfo();
            wfminfo.fLsbs = new double[8];
            wfminfo.rangevalue = new double[8];

            // 这里根据板卡的位数选择合适的读数接口 如板卡是8位 调用ArtScope_FetchBinary8
            if (ART_SCOPE.ArtScope_FetchBinary16(hDevice, fTimeout, m_SampleLength, databuffer, ref wfminfo) < 0)
            {
                Console.WriteLine("Fetch Binary16 error!");
                goto Error_label;
            }
            Console.WriteLine("master data:");
            for (int k = 0; k < m_Wfms; k++)
            {
                fVolt = wfminfo.fLsbs[k % m_Wfms] * (databuffer[k] & wfminfo.wMaxLSB) - wfminfo.rangevalue[k % m_Wfms];
                Console.Write("{0,8:f2}(mV)\t", fVolt);
            }
            Console.WriteLine("");

       Error_label:
            ART_SCOPE.ArtScope_StopAcquisition(hDevice);
            ART_SCOPE.ArtScope_ReleaseAcquisition(hDevice);
            ART_SCOPE.ArtScope_Close(hDevice);
            hEvent.Set();
        }
    }
}
