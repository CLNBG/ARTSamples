VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3930
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   6555
   LinkTopic       =   "Form1"
   ScaleHeight     =   3930
   ScaleWidth      =   6555
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command_button 
      Caption         =   "读取DI"
      Height          =   495
      Left            =   2400
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label2 
      Height          =   495
      Left            =   1440
      TabIndex        =   2
      Top             =   1560
      Width           =   3255
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   615
      Left            =   600
      TabIndex        =   1
      Top             =   2640
      Width           =   5655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim taskHandle As Long
Dim data(0 To 999) As ULong
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strr As String
Dim ret As Long

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/port0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateDIChan(taskHandle, str(0), 0, ArtDAQ_Val_ChanForAllLines)
    If ret <> 0 Then
        MsgBox "通道配置失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI0" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgSampClkTiming(taskHandle, str(0), 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000) <> 0 Then
        MsgBox "配置时钟参数失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始DI输入失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_ReadDigitalU32(taskHandle, -1, 10#, ArtDAQ_Val_GroupByChannel, data(0), 1000, sampsRead, 0) <> 0 Then
        MsgBox "读取DI失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    strr = "Acquired:" & CStr(sampsRead)
    
    Label2.Caption = strr
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停止任务失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是采用外部时钟从Port口进行DI读取的简易流程"
    bRun = False
End Sub
