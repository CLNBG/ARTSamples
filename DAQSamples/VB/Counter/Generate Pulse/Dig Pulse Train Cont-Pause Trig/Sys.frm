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
      Caption         =   "停止脉冲生成"
      Height          =   495
      Left            =   3840
      TabIndex        =   2
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "开始脉冲生成"
      Height          =   495
      Left            =   1200
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   615
      Left            =   480
      TabIndex        =   1
      Top             =   1920
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
    ret = ArtDAQ_CreateCOPulseChanFreq(taskHandle, str(0), 0, ArtDAQ_Val_Hz, ArtDAQ_Val_Low, 0#, 1#, 0.5)
    If ret <> 0 Then
        MsgBox "创建计数器失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI0" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgDigLvlPauseTrig(taskHandle, str(0), ArtDAQ_Val_Low) <> 0 Then
        MsgBox "配置数字暂停触发参数失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgImplicitTiming(taskHandle, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "配置采样模式失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始生成脉冲失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
End Sub

Private Sub Command1_Click()
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停止任务失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是数字暂停触发模式下生成连续脉冲的简易流程"
    bRun = False
End Sub
