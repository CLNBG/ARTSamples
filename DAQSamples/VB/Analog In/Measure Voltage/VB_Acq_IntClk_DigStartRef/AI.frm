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
   StartUpPosition =   3  '����ȱʡ
   Begin MSChart20Lib.MSChart MSChart1 
      Height          =   3735
      Left            =   360
      OleObjectBlob   =   "AI.frx":0000
      TabIndex        =   1
      Top             =   360
      Width           =   13815
   End
   Begin VB.CommandButton Command1 
      Caption         =   "��ȡ"
      Height          =   495
      Left            =   7080
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
Dim taskHandle As Long
Dim databuff(0 To 999) As Double
Dim readret As Long
Dim str() As Byte
Dim strr() As Byte
Dim length As Long
Dim index As Long
Dim ret As Long
Dim Values(1 To 1000, 1 To 2)
Dim strdata As String

Private Sub Command1_Click()
    str = StrConv("", vbFromUnicode)
    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "��������ʧ��!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ai0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAIVoltageChan(taskHandle, str(0), 0, ArtDAQ_Val_Cfg_Default, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "ͨ������ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgSampClkTiming(taskHandle, 0, 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000) <> 0 Then
        MsgBox "���òɼ�����ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI0" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgDigEdgeStartTrig(taskHandle, str(0), ArtDAQ_Val_Rising) <> 0 Then
        MsgBox "�������ֱ��ؿ�ʼ����ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    If ArtDAQ_CfgDigEdgeRefTrig(taskHandle, str(0), ArtDAQ_Val_Rising, 100) <> 0 Then
        MsgBox "�������ֱ�����ش���ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "��ʼ�ɼ�����ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_ReadAnalogF64(taskHandle, 1000, 1#, ArtDAQ_Val_GroupByChannel, databuff(0), 1000, readret, 0) <> 0 Then
        MsgBox "���ݶ�ȡʧ��!"
        ArtDAQ_StopTask (taskHandle)
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    Else
        MsgBox "���ݶ�ȡ�ɹ�!"
        For index = 1 To readret
            Values(index, 1) = "x" & Format(index - 1, "0")
            Values(index, 2) = databuff(index - 1)
        Next
        MSChart1.chartType = VtChChartType2dLine
        MSChart1.ChartData = Values
    End If
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "ֹͣ�ɼ�����ʧ��!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
End Sub

Private Sub Form_Load()
    For index = 1 To 1000
        Values(index, 1) = "x" & Format(index - 1, "0")
        Values(index, 2) = databuff(index - 1)
    Next
    MSChart1.chartType = VtChChartType2dLine
    MSChart1.ChartData = Values
End Sub