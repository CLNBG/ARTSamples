VERSION 5.00
Object = "{65E121D4-0C60-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSChrt20.ocx"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5655
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   14595
   LinkTopic       =   "Form1"
   ScaleHeight     =   5655
   ScaleWidth      =   14595
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command2 
      Caption         =   "停止读取"
      Height          =   495
      Left            =   7920
      TabIndex        =   2
      Top             =   4440
      Width           =   1575
   End
   Begin MSChart20Lib.MSChart MSChart1 
      Height          =   3735
      Left            =   360
      OleObjectBlob   =   "AI.frx":0000
      TabIndex        =   1
      Top             =   360
      Width           =   13815
   End
   Begin VB.CommandButton Command1 
      Caption         =   "开始读取"
      Height          =   495
      Left            =   4680
      TabIndex        =   0
      Top             =   4440
      Width           =   1575
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'*********************************************
'连续采集例程 软件触发
'*********************************************

Dim taskHandle As Long
Dim data(0 To 1000) As Double
Dim read As Long
Dim str() As Byte
Dim strr() As Byte
Dim length As Long
Dim index As Long
Dim ret As Long
Dim Values(1 To 1000, 1 To 2)
Dim mDevRun As Boolean

Private Sub Command1_Click()
    str = StrConv("", vbFromUnicode)
    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ai0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAIVoltageChan(taskHandle, str(0), 0, ArtDAQ_Val_Cfg_Default, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "通道配置失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgSampClkTiming(taskHandle, 0, 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "配置采集参数失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始采集任务失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    mDevRun = True
    
    Do While (mDevRun)
        If ArtDAQ_ReadAnalogF64(taskHandle, 1000, 10#, ArtDAQ_Val_GroupByChannel, data(0), 1000, read, 0) <> 0 Then
            MsgBox "数据读取失败!"
            ArtDAQ_StopTask (taskHandle)
            ArtDAQ_ClearTask (taskHandle)
            Exit Sub
        Else
            For index = 1 To 1000
                Values(index, 1) = "x" & Format(index - 1, "0")
                Values(index, 2) = data(index - 1)
            Next
            MSChart1.chartType = VtChChartType2dLine
            MSChart1.ChartData = Values
        End If
        DoEvents
    Loop
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停止采集任务失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Command2_Click()
    mDevRun = False
End Sub

Private Sub Form_Load()
    For index = 1 To 1000
        Values(index, 1) = "x" & Format(index - 1, "0")
        Values(index, 2) = 0
    Next
    MSChart1.chartType = VtChChartType2dLine
    MSChart1.ChartData = Values
    
    mDevRun = False
End Sub
