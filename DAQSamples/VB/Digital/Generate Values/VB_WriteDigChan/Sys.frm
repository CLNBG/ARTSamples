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
   Begin VB.CommandButton Command_button 
      Caption         =   "��ʼDO���"
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
Dim data(0 To 7) As Byte
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
    
    str = StrConv("Dev1/port0/line0:7" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateDOChan(taskHandle, str(0), 0, ArtDAQ_Val_ChanForAllLines)
    If ret <> 0 Then
        MsgBox "ͨ������ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    data(0) = 1
    data(1) = 0
    data(2) = 0
    data(3) = 1
    data(4) = 1
    data(5) = 1
    data(6) = 1
    data(7) = 1
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "��ʼDO���ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_WriteDigitalLines(taskHandle, 1, 1, 10#, ArtDAQ_Val_GroupByChannel, data(0), 0, 0) <> 0 Then
        MsgBox "DOд��ʧ��!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "ͣDO���ʧ��!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
     
End Sub

Private Sub Form_Load()
    Label1.Caption = "��������ʾ�������д����ֵ��DO��line0��line7"
End Sub
