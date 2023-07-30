Module common
    Public Const INVALID_HANDLE_VALUE = -1
    Public Const INFINITE = &HFFFFFFFF            ' Infinite timeout

    ' 微软公司WIN32 API接口
    Declare Function GetLastError Lib "kernel32" () As Long
    Declare Function ResumeThread Lib "kernel32" (ByVal hThread As Long) As Long
    Declare Function SuspendThread Lib "kernel32" (ByVal hThread As Long) As Long
    Declare Function WaitForSingleObject Lib "kernel32" (ByVal hEvent As Long, ByVal dwMilliseconds As Long) As Long
    Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
    Declare Function SetThreadPriority Lib "kernel32" (ByVal hThread As Long, ByVal nPriority As Long) As Long
    Declare Function SetEvent Lib "kernel32" (ByVal hEvent As Long) As Long
    Declare Function Beep Lib "kernel32" (ByVal dwFreq As Long, ByVal dwDuration As Long) As Long
    Declare Function GetDiskFreeSpace Lib "kernel32" Alias "GetDiskFreeSpaceA" (ByVal lpRootPathName As String, ByVal lpSectorsperCluster As Long, ByVal lpBytespersector As Long, ByVal lpNumberofFreeClusters As Long, ByVal lpTotalNumberofClusters As Long) As Long

    Declare Function _kbhit Lib "MSVCRT" () As Integer
    Declare Function _getch Lib "MSVCRT" () As Integer
End Module
