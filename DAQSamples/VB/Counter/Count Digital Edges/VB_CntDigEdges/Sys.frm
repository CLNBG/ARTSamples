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
   Begin VB.CommandButton Command1 
      Caption         =   "停止计数"
      Height          =   495
      Left            =   3720
      TabIndex        =   3
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "开始计数"
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
Dim taskHandle As Long
Dim data(0 To 1) As ULong
Dim dataTmp As Double
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strr As String
Dim ret As Long
Dim bRun As Boolean

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ctr0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateCICountEdgesChan(taskHandle, str(0), 0, ArtDAQ_Val_Rising, 0, ArtDAQ_Val_CountUp)
    If ret <> 0 Then
        MsgBox "创建计数器失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始计数失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    bRun = True
    Do While bRun = True
        If ArtDAQ_ReadCounterScalarU32(taskHandle, 10#, data(0), 0) <> 0 Then
            MsgBox "读取计数值失败!"
            ArtDAQ_ClearTask (taskHandle)
            Exit Sub
        End If
        dataTmp = data(0).data(0) '* 2 ^ 24 + data(0).data(1) * 2 ^ 16 + data(0).data(2) * 2 ^ 8 + data(0).data(3)
        strr = "Count:" & CStr(dataTmp)
        Label2.Caption = strr
        DoEvents
    Loop
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停止任务失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是采用外部时钟、数字暂停触发连续读取ctr0计数值的简易流程"
    bRun = False
End Sub
