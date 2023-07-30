VERSION 5.00
Begin VB.Form DMA 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "PCI8501_AD_DMA"
   ClientHeight    =   4155
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6285
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4155
   ScaleWidth      =   6285
   StartUpPosition =   3  '����ȱʡ
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   5400
      Top             =   3000
   End
   Begin VB.CommandButton Command_Stop 
      Caption         =   "ֹͣ"
      Height          =   495
      Left            =   3240
      TabIndex        =   11
      Top             =   3000
      Width           =   1215
   End
   Begin VB.CommandButton Command_Start 
      Caption         =   "��ʼ�ɼ�"
      Height          =   495
      Left            =   960
      TabIndex        =   10
      Top             =   3000
      Width           =   1335
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   3
      Left            =   3840
      TabIndex        =   9
      Top             =   2280
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   2
      Left            =   1920
      TabIndex        =   8
      Top             =   2280
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   1
      Left            =   3840
      TabIndex        =   7
      Top             =   1680
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   0
      Left            =   1920
      TabIndex        =   6
      Top             =   1680
      Width           =   975
   End
   Begin VB.ComboBox Combo_InputRange 
      Height          =   300
      ItemData        =   "DMA.frx":0000
      Left            =   2400
      List            =   "DMA.frx":0013
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   840
      Width           =   2055
   End
   Begin VB.Label Label5 
      Caption         =   "CH2:"
      Height          =   255
      Left            =   1440
      TabIndex        =   5
      Top             =   2280
      Width           =   375
   End
   Begin VB.Label Label4 
      Caption         =   "CH3:"
      Height          =   255
      Left            =   3360
      TabIndex        =   4
      Top             =   2280
      Width           =   375
   End
   Begin VB.Label Label3 
      Caption         =   "CH1:"
      Height          =   255
      Left            =   3360
      TabIndex        =   3
      Top             =   1680
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "CH0:"
      Height          =   255
      Left            =   1440
      TabIndex        =   2
      Top             =   1680
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "ѡ������:"
      Height          =   255
      Left            =   1440
      TabIndex        =   0
      Top             =   840
      Width           =   855
   End
End
Attribute VB_Name = "DMA"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim InputRange As Integer
Dim ADBuffer(0 To 1048575) As Integer        ' ���仺����(�洢ԭʼ����)
Dim hDevice As Long
Dim DeviceLgcID As Integer
Dim i As Integer
Dim ADPara As PCI8501_PARA_AD            ' Ӳ������
Dim ADStatus As PCI8501_STATUS_AD      ' AD״̬����
Dim nReadSizeWords As Long         ' ÿ�ζ�ȡAD���ݵĳ���(��)
Dim nRetSizeWords As Long
Dim nReadSize As Long
Dim bFirstWait As Boolean
Dim ulStartAddr As Long
Dim offsetAddr As Long
Dim nCurrentChannel As Integer
Dim nADChannel As Integer
Dim Index As Integer
Dim ADData As Long
Dim fVolt As Single
Dim strTemp As String
Dim ulDDR2Length As Long

Private Sub Command_Start_Click()
    DeviceLgcID = 0
    hDevice = PCI8501_CreateDevice(DeviceLgcID) ' �����豸����
    If hDevice = INVALID_HANDLE_VALUE Then
         MsgBox "PCI8501_CreateDevice Error.."
        Exit Sub
    End If
    PCI8501_GetDDR2Length hDevice, ulDDR2Length
    InputRange = Me.Combo_InputRange.ListIndex       ' �û�ѡ����������

    ' Ԥ��Ӳ������
    ADPara.Frequency = 100000 ' ����Ƶ��(Hz)
    ADPara.InputRange(0) = InputRange  ' ģ��ͨ��0��������
    ADPara.InputRange(1) = InputRange  ' ģ��ͨ��1��������
    ADPara.InputRange(2) = InputRange  ' ģ��ͨ��2��������
    ADPara.InputRange(3) = InputRange  ' ģ��ͨ��3��������
    ADPara.InputRange(4) = InputRange  ' ģ��ͨ��4��������
    ADPara.InputRange(5) = InputRange  ' ģ��ͨ��5��������
    ADPara.InputRange(6) = InputRange  ' ģ��ͨ��6��������
    ADPara.InputRange(7) = InputRange  ' ģ��ͨ��7��������
    ADPara.Gains(0) = 0
    ADPara.Gains(1) = 0
    ADPara.Gains(2) = 0
    ADPara.Gains(3) = 0
    ADPara.Gains(4) = 0
    ADPara.Gains(5) = 0
    ADPara.Gains(6) = 0
    ADPara.Gains(7) = 0
    ADPara.M_Length = 0                   ' M����
    ADPara.N_Length = 1024                ' N����
    ADPara.TriggerMode = PCI8501_TRIGMODE_MIDL   'Ӳ���м䴥��(����Ԥ�������󴥷�����)
    ADPara.TriggerSource = PCI8501_TRIGMODE_SOFT ' �������
    ADPara.TriggerDir = PCI8501_TRIGDIR_NEGATIVE  ' ��������Ϊ����
    ADPara.TrigLevelVolt = 0
    ADPara.ClockSource = PCI8501_CLOCKSRC_IN ' ʱ��Դѡ�ð���ʱ��Դ
    ADPara.bClockSourceDir = False
    ADPara.OutClockSource = PCI8501_OUTCLOCKSRC_TRIGGER0
    
    
    If PCI8501_InitDeviceAD(hDevice, ADPara) = False Then ' ��ʼ��Ӳ��
         MsgBox "PCI8501_InitDeviceAD Error.."
    End If

    nReadSizeWords = (ADPara.M_Length + ADPara.N_Length) * 8
   
    bFirstWait = True
    Me.Command_Start.Enabled = False
    Me.Timer1.Enabled = True
    Me.Command_Stop.Enabled = True
    Me.Combo_InputRange.Enabled = False
End Sub

Private Sub Command_Stop_Click()
    Me.Timer1.Enabled = False
    If hDevice <> INVALID_HANDLE_VALUE Then
    If PCI8501_ReleaseDeviceAD(hDevice) = False Then          ' �ͷ�AD
        MsgBox "PCI8501_ReleaseDeviceAD Error.."
    End If
   
    If PCI8501_ReleaseDevice(hDevice) = False Then       ' �ͷ��豸����
        MsgBox "PCI8501_ReleaseDevice Error.."
    End If
    Me.Command_Stop.Enabled = False
    Me.Command_Start.Enabled = True
    Me.Combo_InputRange.Enabled = True
    End If
End Sub

Private Sub Form_Load()
    Me.Combo_InputRange.ListIndex = 0
    Me.Command_Start.Enabled = True
    Me.Timer1.Enabled = False
    Me.Command_Stop.Enabled = False

End Sub

Private Sub Timer1_Timer()

    If PCI8501_StartDeviceAD(hDevice) = False Then    ' �����豸
        MsgBox "PCI8501_StartDeviceAD Error..."
    End If
    If PCI8501_SetDeviceTrigAD(hDevice) = False Then    ' ����AD
        MsgBox "PCI8501_SetDeviceTrigAD Error..."
    End If
     nReadSize = nReadSizeWords
    While (1)
        If PCI8501_GetDevStatusAD(hDevice, ADStatus) = False Then
            MsgBox "GetDevStatusAD Error..."
             Call Command_Stop_Click
        End If
        If (ADStatus.bComplete = 1) Then
          GoTo read
        End If
    Wend
read:
    If PCI8501_StopDeviceAD(hDevice) = False Then    ' �����豸
        MsgBox "PCI8501_StartDeviceAD Error..."
         Call Command_Stop_Click
    End If
    offsetAddr = ADStatus.lEndAddr - nReadSizeWords * 2 + 4
        If (offsetAddr < 0) Then
            ulStartAddr = ulDDR2Length * 1048576 + (ADStatus.lEndAddr - nReadSizeWords * 2 + 4)
        Else
            ulStartAddr = offsetAddr
        End If
        While (nReadSize > 0)
          If (PCI8501_ReadDeviceAD(hDevice, ADBuffer(0), 1048576, ulStartAddr, nRetSizeWords) = False) Then
                MsgBox ("ReadDeviceDmaAD error...")
                Call Command_Stop_Click
          End If
            nReadSize = nReadSize - 1048576
            ulStartAddr = ulStartAddr + 1048576 * 2
            Index = 0
            While (Index < 64)
                    For nADChannel = 0 To 7 Step 1
                        ADData = ADBuffer(Index) And 65535
                        Select Case InputRange
                            Case PCI8501_INPUT_N10000_P10000mV      '-10000mV - +10000mV
                                fVolt = ((20000# / 65536) * ADData - 10000#)
                            Case PCI8501_INPUT_N5000_P5000mV ' -5V - +5V
                                fVolt = ((10000# / 65536) * ADData - 5000#)
    
                            Case PCI8501_INPUT_N2500_P2500mV  ' -2.5V - +2.5V
                                fVolt = ((5000# / 65536) * ADData - 2500#)
                                
                            Case PCI8501_INPUT_0_P10000mV  '0mV - +10000mV
                                fVolt = ((10000# / 65536) * ADData)
    
                            Case PCI8501_INPUT_0_P5000mV    '0mV - +5000mV
                                fVolt = ((5000# / 65536) * ADData)
                        End Select
                        strTemp = fVolt
                        If nADChannel <= 3 Then
                           Me.TextVolt(nADChannel).Text = Format(strTemp, "00.00")
                           'nADChannel = 0
                        End If
                        Index = Index + 1
                        'nADChannel = nADChannel + 1
                    Next nADChannel
            Wend
        Wend
End Sub
