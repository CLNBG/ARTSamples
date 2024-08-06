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
   Begin VB.CommandButton Command1 
      Caption         =   "停止测量"
      Height          =   495
      Left            =   3720
      TabIndex        =   3
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "开始测量"
      Height          =   495
      Left            =   840
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
Dim TaskHandle As Long
Dim data(0 To 999) As ULong
Dim dutyStep As Double
Dim written As Long
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strr As String
Dim index As Long
Dim bRun As Boolean
Dim Position(0 To 1000) As Long
Dim pulseWidth(0 To 1000) As Long



Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, TaskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    For index = 0 To 100
        Position(index) = 1000 * (index + 1)
        pulseWidth(index) = 1000    ' 单位时间，单位10ns
    Next
        
        
    str = StrConv("Dev1/ctr0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateCICountEdgesChan(TaskHandle, str(0), 0, ArtDAQ_Val_Rising, 0, ArtDAQ_Val_CountUp)
    If ret <> 0 Then
        MsgBox "创建编码通道失败!"
        ArtDAQ_ClearTask (TaskHandle)
        Exit Sub
    End If
    
        
        
    str = StrConv("/Dev1/PFI11" & vbNullChar, vbFromUnicode)
    If ArtDAQ_ExportPositionComparsionEvent(TaskHandle, str(0), ArtDAQ_Val_Toggle, ArtDAQ_Val_ActiveHigh, ArtDAQ_Val_High, ArtDAQ_Val_PulseWidth_Implicit) <> 0 Then
        MsgBox "输出位置比较事件失败!"
        ArtDAQ_ClearTask (TaskHandle)
        Exit Sub
    End If
    
        
    If ArtDAQ_WritePositionComparsionData(TaskHandle, 100, Position(0), pulseWidth(0), written, 0) <> 0 Then
        MsgBox "写入位置比较数据失败!"
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
        If ArtDAQ_ReadCounterU32(TaskHandle, 1000, 10#, data(0), 1000, sampsRead, 0) <> 0 Then
            MsgBox "读取数据失败!"
            ArtDAQ_ClearTask (TaskHandle)
            Exit Sub
        End If
        Label2.Caption = "Acquired " & CStr(sampsRead) & "samples"
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
    Label1.Caption = "位置比较输出"
    bRun = False
End Sub

