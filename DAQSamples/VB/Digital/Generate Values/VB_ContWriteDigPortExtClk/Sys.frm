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
      Caption         =   "ֹͣDO���"
      Height          =   495
      Left            =   3240
      TabIndex        =   2
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "��ʼDO���"
      Height          =   495
      Left            =   1320
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   1575
      Left            =   600
      TabIndex        =   1
      Top             =   1680
      Width           =   5655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim taskHandle As Long
Dim data(0 To 7) As ULong
Dim read As Long
Dim str() As Byte
Dim strr() As Byte
Dim length As Long
Dim index As Long
Dim ret As Long

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "��������ʧ��!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/port0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateDOChan(taskHandle, str(0), 0, ArtDAQ_Val_ChanForAllLines)
    If ret <> 0 Then
        MsgBox "ͨ������ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    str = StrConv("/Dev1/PFI0" & vbNullChar, vbFromUnicode)
    If ArtDAQ_CfgSampClkTiming(taskHandle, str(0), 1000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000) <> 0 Then
        MsgBox "���òɼ�����ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    data(0).data(0) = 0
    data(0).data(1) = 1
    data(1).data(0) = 0
    data(1).data(1) = 2
    data(2).data(0) = 0
    data(2).data(1) = 4
    data(3).data(0) = 0
    data(3).data(1) = 8
    data(4).data(0) = 0
    data(4).data(1) = 16
    data(5).data(0) = 0
    data(5).data(1) = 32
    data(6).data(0) = 0
    data(6).data(1) = 64
    data(7).data(0) = 0
    data(7).data(1) = 128
    
    If ArtDAQ_WriteDigitalU32(taskHandle, 8, 0, 10#, ArtDAQ_Val_GroupByChannel, data(0), 0, 0) <> 0 Then
        MsgBox "DOд��ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "��ʼDO���ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
     
End Sub

Private Sub Command1_Click()
        If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "ͣDO���ʧ��!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
End Sub

Private Sub Form_Load()
    Label1.Caption = "��������ʾ������β�����ʱ���������DO"
End Sub
