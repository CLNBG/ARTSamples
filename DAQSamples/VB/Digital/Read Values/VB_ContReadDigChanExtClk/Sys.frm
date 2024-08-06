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
   StartUpPosition =   3  '����ȱʡ
   Begin VB.CommandButton Command1 
      Caption         =   "ֹͣ��ȡDI"
      Height          =   495
      Left            =   3240
      TabIndex        =   2
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "��ʼ��ȡDI"
      Height          =   495
      Left            =   1320
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label2 
      Height          =   495
      Left            =   1440
      TabIndex        =   3
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
Dim data(0 To 999) As ULong
Dim sampsRead As Long
Dim totalRead As Long
Dim str() As Byte
Dim strr() As Byte
Dim length As Long
Dim index As Long
Dim ret As Long
Dim bRun As Boolean

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "��������ʧ��!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/port0/line0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateDIChan(taskHandle, str(0), 0, ArtDAQ_Val_ChanPerLine)
    If ret <> 0 Then
        MsgBox "ͨ������ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI0" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgSampClkTiming(taskHandle, str(0), 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "����ʱ�Ӳ���ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "��ʼDO���ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    bRun = True
    totalRead = 0
    Do While (bRun)
        If ArtDAQ_ReadDigitalU32(taskHandle, 1000, 10#, ArtDAQ_Val_GroupByChannel, data(0), 1000, sampsRead, 0) <> 0 Then
            MsgBox "��ȡDIʧ��!"
            ArtDAQ_ClearTask (taskHandle)
            Exit Sub
        End If
        totalRead = totalRead + sampsRead
        Label2.Caption = "Acquired " & CStr(sampsRead) & " samples." & "total " & CStr(totalRead)
        DoEvents
    Loop
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "ͣDO���ʧ��!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "��������ʾ������β�����ʱ�ӽ���DI��ȡ�ļ�������"
    bRun = False
End Sub
