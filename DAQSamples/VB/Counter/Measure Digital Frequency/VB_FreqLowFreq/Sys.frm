VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "ArtDAQ_Val_ContSamps"
   ClientHeight    =   3930
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   6555
   LinkTopic       =   "Form1"
   ScaleHeight     =   3930
   ScaleWidth      =   6555
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command_button 
      Caption         =   "测量频率"
      Height          =   495
      Left            =   2520
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label2 
      Height          =   615
      Left            =   2040
      TabIndex        =   2
      Top             =   1440
      Width           =   2415
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   615
      Left            =   480
      TabIndex        =   1
      Top             =   2280
      Width           =   5655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim taskHandle As Long
Dim data(0 To 1) As Double
Dim dutyStep As Double
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strr As String
Dim index As Long
Dim bRun As Boolean

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ctr0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateCIFreqChan(taskHandle, str(0), 0, 0.023284, 50000000#, ArtDAQ_Val_Hz, ArtDAQ_Val_Rising, ArtDAQ_Val_LowFreq1Ctr, 0.001, 4, 0)
    If ret <> 0 Then
        MsgBox "创建频率通道失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始任务失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    bRun = True
    
    If ArtDAQ_ReadCounterScalarF64(taskHandle, 10#, data(0), 0) <> 0 Then
        MsgBox "读取频率值失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    Label2.Caption = "Frequency " & CStr(data(0)) & " Hz"
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停止任务失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
    
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是测量频率的简易流程"
End Sub
