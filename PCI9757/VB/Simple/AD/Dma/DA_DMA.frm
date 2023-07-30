VERSION 5.00
Begin VB.Form DA_DMA 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   3600
   ClientLeft      =   5985
   ClientTop       =   4080
   ClientWidth     =   4815
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3600
   ScaleWidth      =   4815
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   3
      Left            =   2880
      TabIndex        =   5
      Top             =   1800
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   2
      Left            =   1200
      TabIndex        =   4
      Top             =   1800
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   1
      Left            =   2880
      TabIndex        =   3
      Top             =   1200
      Width           =   975
   End
   Begin VB.CommandButton Command_Start 
      Caption         =   "�ɼ�"
      Height          =   495
      Left            =   1920
      TabIndex        =   2
      Top             =   2520
      Width           =   1455
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   0
      Left            =   1200
      TabIndex        =   1
      Top             =   1200
      Width           =   975
   End
   Begin VB.ComboBox Combo_InputRange 
      Height          =   300
      ItemData        =   "DA_DMA.frx":0000
      Left            =   1800
      List            =   "DA_DMA.frx":0013
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   360
      Width           =   2535
   End
   Begin VB.Label Label2 
      Caption         =   "CH3"
      Height          =   255
      Index           =   3
      Left            =   2520
      TabIndex        =   10
      Top             =   1800
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH2"
      Height          =   255
      Index           =   2
      Left            =   840
      TabIndex        =   9
      Top             =   1800
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH1"
      Height          =   255
      Index           =   1
      Left            =   2520
      TabIndex        =   8
      Top             =   1200
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH0"
      Height          =   255
      Index           =   0
      Left            =   840
      TabIndex        =   7
      Top             =   1200
      Width           =   495
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "����ѡ��"
      Height          =   255
      Left            =   600
      TabIndex        =   6
      Top             =   360
      Width           =   1095
   End
End
Attribute VB_Name = "DA_DMA"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ˵���� ��������ʾ�������DMA��ʽ��ȡAD����

Option Explicit
Dim hDevice As Long         ' �豸���
Dim DeviceLgcID As Integer
Dim ADPara As PCI9757_PARA_AD           ' Ӳ������
Dim DMAStatus As PCI9757_STATUS_DMA     ' DMA״̬����
Dim nCurrentChannel As Long
Dim nChannel As Long
Dim ChannelCount As Long
Dim Index As Long
Dim ADData As Long
Dim fVolt As Single
Dim strTemp As String
Dim ADBuffer(0 To 4095, 0 To 63) As Long         ' ���仺����(�洢ԭʼ����)
Dim hDmaEvent As Long

Private Sub Command_Start_Click()

    
    DeviceLgcID = 0
    hDevice = PCI9757_CreateDevice(DeviceLgcID) ' �����豸����
    If (hDevice = INVALID_HANDLE_VALUE) Then
        MsgBox "PCI9757_CreateDevice error"
        Exit Sub ' ��������豸����ʧ�ܣ��򷵻�
    End If


    ' Ԥ��Ӳ������
   
    For Index = 0 To 3
    
        ADPara.bChannelArray(Index) = 1     ' ����ͨ������
        ADPara.InputRange(Index) = Me.Combo_InputRange.ListIndex        ' ģ��ͨ����������
        
    Next Index
    
    ADPara.Frequency = 10000        ' ����Ƶ��(Hz)
    ADPara.TriggerMode = 0
    ADPara.TriggerSource = PCI9757_TRIGMODE_SOFT    ' ����Դ
    ADPara.TriggerType = PCI9757_TRIGTYPE_EDGE      ' ��������Ϊ���ش���
    ADPara.TriggerDir = PCI9757_TRIGDIR_NEGATIVE       ' ��������Ϊ����
    ADPara.ClockSource = PCI9757_CLOCKSRC_IN      ' ʱ��Դѡ�ð���ʱ��Դ
    ADPara.TrigLevelVolt = 2000
    ADPara.TrigWindow = 40
  
    hDmaEvent = PCI9757_CreateSystemEvent()
    If (PCI9757_InitDeviceDmaAD(hDevice, hDmaEvent, ADBuffer(0, 0), 4096, 64, 4096, ADPara) = False) Then ' ��ʼ��Ӳ��
        MsgBox "PCI9757_InitDeviceDmaAD error"
       GoTo ExitRead1
    End If

    PCI9757_StartDeviceDmaAD (hDevice) ' �����豸

    While (True) ' ��ѯ��ǰ�������������Ƿ���׼������
        If (WaitForSingleObject(hDmaEvent, 100) = 0) Then
        GoTo Read
        End If
    Wend

Read:
    If PCI9757_GetDevStatusDmaAD(hDevice, DMAStatus) = False Then

        MsgBox ("Get Device Status Error...")
        GoTo ExitRead0
    End If

    If (DMAStatus.bBufferOverflow = True) Then
        MsgBox ("DMA Overflow...")
    End If
     
        If (DMAStatus.bSegmentSts(0)) Then
            For Index = 0 To 3 Step 1
            
                ADData = ADBuffer(Index, 0) And 65535
                Select Case Combo_InputRange.ListIndex
                Case PCI9757_INPUT_N10000_P10000mV: ' -10V - +10V
                    fVolt = (20000# / 65536) * ADData - 10000#
                    
                Case PCI9757_INPUT_N5000_P5000mV: ' -5V - +5V
                    fVolt = (10000# / 65536) * ADData - 5000#
                    
                Case PCI9757_INPUT_N2500_P2500mV: ' -2.5V - +2.5V
                    fVolt = (5000# / 65536) * ADData - 2500#
                    
                Case PCI9757_INPUT_0_P10000mV: ' -0V - +10V
                    fVolt = (10000# / 65536) * ADData
                    
                Case PCI9757_INPUT_0_P5000mV: ' -0V - +5V
                    fVolt = (5000# / 65536) * ADData

                End Select
                
                strTemp = fVolt
                Me.TextVolt(Index).Text = Format(strTemp, "00.0000")          ' ��ʾ��ѹֵ
            Next Index
            
            If (PCI9757_SetDevStatusDmaAD(hDevice, 0) = False) Then
                MsgBox ("PCI9757_SetDevStatusDmaAD error")
                GoTo ExitRead0
            End If
        End If


ExitRead0:
    PCI9757_ReleaseDeviceDmaAD (hDevice) ' �ͷ�AD
    PCI9757_ReleaseSystemEvent (hDmaEvent)
ExitRead1:
    PCI9757_ReleaseDevice (hDevice) ' �ͷ��豸����

    
End Sub

Private Sub Form_Load()
    Me.Combo_InputRange.ListIndex = 0
End Sub
