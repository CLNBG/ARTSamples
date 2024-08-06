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
      Caption         =   "停止同步采集"
      Height          =   495
      Left            =   3600
      TabIndex        =   3
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "开始同步采集"
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
Dim AItaskHandle As Long
Dim AOtaskHandle As Long
Dim AOdata(0 To 999) As Double
Dim AIdata(0 To 999) As Double
Dim readAI As Long
Dim str() As Byte
Dim strr As String
Dim bRun As Boolean
Dim index As Long
Dim phase As Double

Private Sub Command_button_Click()

    'AI
    If ArtDAQ_CreateTask(0, AItaskHandle) <> 0 Then
        MsgBox "创建AI任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ai0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAIVoltageChan(AItaskHandle, str(0), 0, ArtDAQ_Val_Cfg_Default, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "AI通道创建失败!"
        ArtDAQ_ClearTask (AItaskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgSampClkTiming(AItaskHandle, 0, 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "配置时钟参数失败!"
        ArtDAQ_ClearTask (masterTaskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI1" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgDigEdgeStartTrig(AItaskHandle, str(0), ArtDAQ_Val_Rising) <> 0 Then
        MsgBox "配置边沿开始触发参数失败!"
        ArtDAQ_ClearTask (AItaskHandle)
        Exit Sub
    End If
    
    'AO
    If ArtDAQ_CreateTask(0, AOtaskHandle) <> 0 Then
        MsgBox "创建AO任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ao0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAOVoltageChan(AOtaskHandle, str(0), 0, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "AO通道创建失败!"
        ArtDAQ_ClearTask (AOtaskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgSampClkTiming(AOtaskHandle, 0, 5000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "配置AO采集时钟参数失败!"
        ArtDAQ_ClearTask (AOtaskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI1" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgDigEdgeStartTrig(AOtaskHandle, str(0), ArtDAQ_Val_Rising) <> 0 Then
        MsgBox "配置AO数字边沿开始触发参数失败!"
        ArtDAQ_ClearTask (AOtaskHandle)
        Exit Sub
    End If
    
    'AO输出产生正弦波形
    phase = 0#
    For index = 0 To 999
        AOdata(index) = 1# * Sin(pi / 180# * (phase + 360# * 1# / 1000 * index))
        phase = phase + 1# / 1000 * 360# * index Mod 360#
    Next
    
    If ArtDAQ_WriteAnalogF64(AOtaskHandle, 1000, 0, 10#, ArtDAQ_Val_GroupByChannel, AOdata(0), 0, 0) <> 0 Then
        MsgBox "写入AO失败!"
        ArtDAQ_ClearTask (AOtaskHandle)
        Exit Sub
    End If
     
    '开始
    If ArtDAQ_StartTask(AOtaskHandle) <> 0 Then
        MsgBox "开始AO失败!"
        ArtDAQ_ClearTask (AOtaskHandle)
        Exit Sub
    End If
    
    bRun = True
    Do While bRun = True
        If ArtDAQ_ReadAnalogF64(AItaskHandle, 1000, 10#, ArtDAQ_Val_GroupByChannel, AIdata(0), 1000, readAI, 0) <> 0 Then
            MsgBox "AI数据读取失败!"
            ArtDAQ_ClearTask (AItaskHandle)
            Exit Sub
        End If
        str = "AI本次读取数据" & CStr(readAI) & "个"
        Label2.Caption = str
        DoEvents
    Loop
    
    If ArtDAQ_StopTask(AItaskHandle) <> 0 Then
        MsgBox "AI停止任务失败!"
    End If
    If ArtDAQ_ClearTask(AItaskHandle) <> 0 Then
        MsgBox "释放AI任务失败!"
    End If
    
    If ArtDAQ_StopTask(AOtaskHandle) <> 0 Then
        MsgBox "AO停止任务失败!"
    End If

    If ArtDAQ_ClearTask(AOtaskHandle) <> 0 Then
        MsgBox "释放AO任务失败!"
    End If
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是同一张卡不同任务间进行同步的简易流程"
    bRun = False
End Sub
