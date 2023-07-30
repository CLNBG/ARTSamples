Module Common_Module
    Public Const INVALID_HANDLE_VALUE = -1
    Public Const INFINITE = &HFFFFFFFF            ' Infinite timeout

    ' 微软公司WIN32 API接口
    Declare Function GetLastError Lib "kernel32" () As Integer
    Declare Function ResumeThread Lib "kernel32" (ByVal hThread As Integer) As Integer
    Declare Function SuspendThread Lib "kernel32" (ByVal hThread As Integer) As Integer
    Declare Function WaitForSingleObject Lib "kernel32" (ByVal hEvent As Integer, ByVal dwMilliseconds As Integer) As Integer
    Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Integer)
    Declare Function SetThreadPriority Lib "kernel32" (ByVal hThread As Integer, ByVal nPriority As Integer) As Integer
    Declare Function SetEvent Lib "kernel32" (ByVal hEvent As Integer) As Integer
    Declare Function Beep Lib "kernel32" (ByVal dwFreq As Integer, ByVal dwDuration As Integer) As Integer
    Declare Function GetDiskFreeSpace Lib "kernel32" Alias "GetDiskFreeSpaceA" (ByVal lpRootPathName As String, ByVal lpSectorsperCluster As Integer, ByVal lpBytespersector As Integer, ByVal lpNumberofFreeClusters As Integer, ByVal lpTotalNumberofClusters As Integer) As Integer

    Declare Function _kbhit Lib "MSVCRT" () As Integer
    Declare Function _getch Lib "MSVCRT" () As Integer


End Module
