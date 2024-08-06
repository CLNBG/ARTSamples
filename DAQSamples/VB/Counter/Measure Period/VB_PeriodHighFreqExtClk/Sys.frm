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
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command1 
      Caption         =   "ֹͣ���ڲ���"
      Height          =   495
      Left            =   3600
      TabIndex        =   3
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "��ʼ���ڲ���"
      Height          =   495
      Left            =   1080
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
Dim data(0 To 999) As Double
Dim dutyStep As Double
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strr As String
Dim index As Long
Dim bRun As Boolean

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "��������ʧ��!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ctr0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateCIPeriodChan(taskHandle, str(0), 0, 0.001, 0.1, ArtDAQ_Val_Seconds, ArtDAQ_Val_Rising, ArtDAQ_Val_HighFreq2Ctr, 0.0001, 4, 0)
    If ret <> 0 Then
        MsgBox "������������ͨ��ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI3" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgSampClkTiming(taskHandle, str(0), 100, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "����ʱ�Ӳ���ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "��ʼ����ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    bRun = True
    
    Do While bRun = True
        If ArtDAQ_ReadCounterF64(taskHandle, 1000, 10#, data(0), 1000, sampsRead, 0) <> 0 Then
            MsgBox "��ȡ����ֵʧ��!"
            ArtDAQ_ClearTask (taskHandle)
            Exit Sub
        End If
        Label2.Caption = "Acquired " & CStr(sampsRead) & "samples"
        DoEvents
    Loop
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "ֹͣ����ʧ��!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
    
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "��������ʾ���ǲ����ⲿʱ�Ӳ������ڵļ�������"
End Sub
