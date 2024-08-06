VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3930
   ClientLeft      =   8550
   ClientTop       =   5655
   ClientWidth     =   6555
   LinkTopic       =   "Form1"
   ScaleHeight     =   3930
   ScaleWidth      =   6555
   Begin VB.CommandButton Command1 
      Caption         =   "停止采集"
      Height          =   495
      Left            =   3720
      TabIndex        =   3
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "开始采集"
      Height          =   495
      Left            =   1320
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
Dim TaskHandle As Long
Dim data(0 To 999) As Double
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strr As String
Dim ret As Long
Dim bRun As Boolean

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, TaskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ao0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAIRTDChan(TtaskHandle, str(0), 0, 0#, 100#, ArtDAQ_Val_DegC, ArtDAQ_Val_Pt3750, ArtDAQ_Val_4Wire, ArtDAQ_Val_Internal, 0.004, 100#)
    If ret <> 0 Then
        MsgBox "创建AI RTD通道失败!"
        ArtDAQ_ClearTask (TaskHandle)
        Exit Sub
    End If
    
    
    
    If ArtDAQ_CfgSampClkTiming(TaskHandle, 0, 50#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "配置时钟参数失败!"
        ArtDAQ_ClearTask (TaskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(TaskHandle) <> 0 Then
        MsgBox "开始任务失败!"
        ArtDAQ_ClearTask (TaskHandle)
        Exit Sub
    End If
    
    bRun = True
    Do While bRun = True
        If ArtDAQ_ReadAnalogF64(TaskHandle, 10, 10#, ArtDAQ_Val_GroupByScanNumber, data(0), 1000, sampsRead, 0) <> 0 Then
            MsgBox "读取采集值失败!"
            ArtDAQ_ClearTask (TaskHandle)
            Exit Sub
        End If
        
        strr = "Acquired:" & CStr(sampsRead)
        Label2.Caption = strr
        
        DoEvents
        
    Loop
    
    If ArtDAQ_StopTask(TaskHandle) <> 0 Then
        MsgBox "停止任务失败!"
    End If
    If ArtDAQ_ClearTask(TaskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是采集热敏电阻简易流程"
    bRun = False
End Sub

