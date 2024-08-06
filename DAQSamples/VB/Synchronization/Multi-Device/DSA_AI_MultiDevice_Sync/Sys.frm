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
      Caption         =   "ֹͣͬ���ɼ�"
      Height          =   495
      Left            =   3600
      TabIndex        =   3
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "��ʼͬ���ɼ�"
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
Dim gMasterAItaskHandle As Long
Dim gSlaveAItaskHandle As Long
Dim masterData(0 To 999) As Double
Dim slaveData(0 To 999) As Double
Dim sampsRead As Long
Dim masterRead As Long
Dim slaveRead As Long
Dim str() As Byte
Dim strr As String
Dim synchronizationType As Long '0:����ʱ�� 1:�ο�ʱ��
Dim bRun As Boolean

Private Sub Command_button_Click()

    '����
    If ArtDAQ_CreateTask(0, gMasterAItaskHandle) <> 0 Then
        MsgBox "������������ʧ��!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ai0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAIVoltageChan(gMasterAItaskHandle, str(0), 0, ArtDAQ_Val_Cfg_Default, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "����AIͨ������ʧ��!"
        ArtDAQ_ClearTask (gMasterAItaskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgSampClkTiming(gMasterAItaskHandle, 0, 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 10000) <> 0 Then
        MsgBox "��������ʱ�Ӳ���ʧ��!"
        ArtDAQ_ClearTask (gMasterAItaskHandle)
        Exit Sub
    End If
    
    '�ӿ�
    If ArtDAQ_CreateTask(0, gSlaveAItaskHandle) <> 0 Then
        MsgBox "������������ʧ��!"
        Exit Sub
    End If
    
    str = StrConv("Dev2/ai0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAIVoltageChan(gSlaveAItaskHandle, str(0), 0, ArtDAQ_Val_Cfg_Default, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "�ӿ�AIͨ������ʧ��!"
        ArtDAQ_ClearTask (gSlaveAItaskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_CfgSampClkTiming(gSlaveAItaskHandle, 0, 10000#, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 10000) <> 0 Then
        MsgBox "���ôӿ�ʱ�Ӳ���ʧ��!"
        ArtDAQ_ClearTask (gSlaveAItaskHandle)
        Exit Sub
    End If
    
    synchronizationType = 0
    If synchronizationType = 1 Then
        'ѡ��ο�ʱ��ͬ�������ӿ�����Ҫ�Ӱ忨ָ������(PCI��ָ��RTSI9��PXI��ָ��PXI_10M)
        str = StrConv("RTSI9" & vbNullChar, vbFromUnicode)
        If ArtDAQ_SetRefClkSrc(gMasterAItaskHandle, str(0)) <> 0 Then
            MsgBox "���������ο�ʱ��Դ����ʧ��!"
            ArtDAQ_ClearTask (gMasterAItaskHandle)
            Exit Sub
        End If
        If ArtDAQ_SetRefClkSrc(gSlaveAItaskHandle, str(0)) <> 0 Then
            MsgBox "���ôӿ��ο�ʱ��Դ����ʧ��!"
            ArtDAQ_ClearTask (gSlaveAItaskHandle)
            Exit Sub
        End If
    Else
        '��������������ʱ��ʱ�����ӿ��������ʱ��ʱ��
        str = StrConv("RTSI8" & vbNullChar, vbFromUnicode)
        If ArtDAQ_SetExportedSampClkTimebaseOutputTerm(gMasterAItaskHandle, str(0)) <> 0 Then
            MsgBox "����������������ʱ������ʧ��!"
            ArtDAQ_ClearTask (gMasterAItaskHandle)
            Exit Sub
        End If
        If ArtDAQ_SetSampClkTimebaseSrc(gSlaveAItaskHandle, str(0)) <> 0 Then
            MsgBox "���ôӿ�����ʱ������ʧ��!"
            ArtDAQ_ClearTask (gSlaveAItaskHandle)
            Exit Sub
        End If
    End If
    
    '�������ͬ�����壬�ӿ�����ͬ�����壬����ͬ��DDC
    str = StrConv("RTSI0" & vbNullChar, vbFromUnicode)
    If ArtDAQ_SetExportedSyncPulseEventOutputTerm(gMasterAItaskHandle, str(0)) <> 0 Then
        MsgBox "�����������ͬ���������ʧ��!"
        ArtDAQ_ClearTask (gMasterAItaskHandle)
        Exit Sub
    End If
    If ArtDAQ_SetSyncPulseSrc(gSlaveAItaskHandle, str(0)) <> 0 Then
        MsgBox "���ôӿ�����ͬ���������ʧ��!"
        ArtDAQ_ClearTask (gSlaveAItaskHandle)
        Exit Sub
    End If
    
    '��������ѡ���ⲿ��������������������Ҳ���Բ����ô����������ڲ��Ĵ����ź���ͬ���ɼ�
    str = StrConv("RTSI1" & vbNullChar, vbFromUnicode)
    If ArtDAQ_ExportSignal(gMasterAItaskHandle, ArtDAQ_Val_StartTrigger, str(0)) <> 0 Then
        MsgBox "�������������ʼ��������ʧ��!"
        ArtDAQ_ClearTask (gMasterAItaskHandle)
        Exit Sub
    End If
    If ArtDAQ_CfgDigEdgeStartTrig(gSlaveAItaskHandle, str(0), ArtDAQ_Val_Rising) <> 0 Then
        MsgBox "���ôӿ����봥������ʧ��!"
        ArtDAQ_ClearTask (gSlaveAItaskHandle)
        Exit Sub
    End If
    
    '��ʼ �ӿ��ȿ�ʼ �����ٿ�ʼ
    If ArtDAQ_StartTask(slaveTaskHandle) <> 0 Then
        MsgBox "�ӿ���ʼ�ɼ�ʧ��!"
        ArtDAQ_ClearTask (slaveTaskHandle)
        Exit Sub
    End If
    If ArtDAQ_StartTask(masterTaskHandle) <> 0 Then
        MsgBox "�ӿ���ʼ�ɼ�ʧ��!"
        ArtDAQ_ClearTask (masterTaskHandle)
        Exit Sub
    End If
    
    bRun = True
    Do While bRun = True
        If ArtDAQ_ReadAnalogF64(masterTaskHandle, 1000, 10#, ArtDAQ_Val_GroupByChannel, masterData(0), 1000, masterRead, 0) <> 0 Then
            MsgBox "�������ݶ�ȡʧ��!"
            ArtDAQ_ClearTask (masterTaskHandle)
            Exit Sub
        End If
        If ArtDAQ_ReadAnalogF64(slaveTaskHandle, 1000, 10#, ArtDAQ_Val_GroupByChannel, slaveData(0), 1000, slaveRead, 0) <> 0 Then
            MsgBox "�ӿ����ݶ�ȡʧ��!"
            ArtDAQ_ClearTask (slaveTaskHandle)
            Exit Sub
        End If
        str = "�������ζ�ȡ����" & CStr(masterRead) & "�����ӿ���ȡ����" & CStr(slaveRead) & "��"
        Label2.Caption = str
    Loop
    
    If ArtDAQ_StopTask(masterTaskHandle) <> 0 Then
        MsgBox "����ֹͣ����ʧ��!"
    End If
    If ArtDAQ_ClearTask(masterTaskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
    
    If ArtDAQ_StopTask(slaveTaskHandle) <> 0 Then
        MsgBox "�ӿ�ֹͣ����ʧ��!"
    End If

    If ArtDAQ_ClearTask(slaveTaskHandle) <> 0 Then
        MsgBox "�ͷ�����ʧ��!"
    End If
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "��������ʾ�������ӿ�ͬ����ѡͬ��ʱ��Դ������Դ���вɼ��ļ�������"
    bRun = False
End Sub
