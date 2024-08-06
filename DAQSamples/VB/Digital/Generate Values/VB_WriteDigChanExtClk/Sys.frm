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
      Caption         =   "开始DO输出"
      Height          =   495
      Left            =   2400
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   1575
      Left            =   1200
      TabIndex        =   1
      Top             =   1560
      Width           =   4095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim taskHandle As Long
Dim data(0 To 999) As Byte
Dim read As Long
Dim str() As Byte
Dim strr() As Byte
Dim length As Long
Dim index As Long
Dim ret As Long

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/port0/line0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateDOChan(taskHandle, str(0), 0, ArtDAQ_Val_ChanPerLine)
    If ret <> 0 Then
        MsgBox "通道配置失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CfgSampClkTiming(taskHandle, str(0), 1000#, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000)
    If ret <> 0 Then
        MsgBox "配置时钟参数失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    For index = 0 To 999
        data(index) = index Mod 2
    Next
    
    If ArtDAQ_WriteDigitalLines(taskHandle, 1000, 0, 10#, ArtDAQ_Val_GroupByChannel, data(0), 0, 0) <> 0 Then
        MsgBox "DO写入失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始DO输出失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_WaitUntilTaskDone(taskHandle, 10#) <> 0 Then
        MsgBox "等待DO输出完成失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停DO输出失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
     
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是如何采用外时钟以有限模式输出DO简易流程"
End Sub
