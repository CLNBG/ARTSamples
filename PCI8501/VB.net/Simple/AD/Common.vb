Module Common
    Public Const INVALID_HANDLE_VALUE = -1
    Public Const INFINITE = &HFFFFFFFF            ' Infinite timeout

    ' 微软公司WIN32 API接口
    Declare Function GetLastError Lib "kernel32" () As Int32
    Declare Function ResumeThread Lib "kernel32" (ByVal hThread As Int32) As Int32
    Declare Function SuspendThread Lib "kernel32" (ByVal hThread As Int32) As Int32
    Declare Function WaitForSingleObject Lib "kernel32" (ByVal hEvent As Int32, ByVal dwMilliseconds As Int32) As Int32
    Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Int32)
    Declare Function SetThreadPriority Lib "kernel32" (ByVal hThread As Int32, ByVal nPriority As Int32) As Int32
    Declare Function SetEvent Lib "kernel32" (ByVal hEvent As Int32) As Int32
    Declare Function Beep Lib "kernel32" (ByVal dwFreq As Int32, ByVal dwDuration As Int32) As Int32
    Declare Function GetDiskFreeSpace Lib "kernel32" Alias "GetDiskFreeSpaceA" (ByVal lpRootPathName As String, ByVal lpSectorsperCluster As Int32, ByVal lpBytespersector As Int32, ByVal lpNumberofFreeClusters As Int32, ByVal lpTotalNumberofClusters As Int32) As Int32

    Declare Function _getch Lib "msvcrt" () As Int32
    Declare Function _kbhit Lib "msvcrt" () As Int32




End Module
