VERSION 5.00
Begin VB.Form Npt 
   Caption         =   "AD_Npt"
   ClientHeight    =   6795
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   6270
   LinkTopic       =   "Form1"
   ScaleHeight     =   6795
   ScaleWidth      =   6270
   StartUpPosition =   3  '����ȱʡ
   Begin VB.ComboBox Combo_InputRange 
      Height          =   300
      ItemData        =   "Npt.frx":0000
      Left            =   2160
      List            =   "Npt.frx":0010
      TabIndex        =   14
      Text            =   "Combo1"
      Top             =   480
      Width           =   2295
   End
   Begin VB.TextBox TextVolt 
      Height          =   375
      Index           =   7
      Left            =   3840
      TabIndex        =   12
      Top             =   4560
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Height          =   375
      Index           =   6
      Left            =   1080
      TabIndex        =   11
      Top             =   4560
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Height          =   375
      Index           =   5
      Left            =   3840
      TabIndex        =   10
      Top             =   3480
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Height          =   390
      Index           =   4
      Left            =   1080
      TabIndex        =   9
      Top             =   3480
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   375
      Index           =   3
      Left            =   3840
      TabIndex        =   8
      Top             =   2400
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   375
      Index           =   2
      Left            =   1080
      TabIndex        =   7
      Top             =   2520
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   375
      Index           =   1
      Left            =   3840
      TabIndex        =   6
      Top             =   1440
      Width           =   1095
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   375
      Index           =   0
      Left            =   1080
      TabIndex        =   5
      Top             =   1560
      Width           =   1095
   End
   Begin VB.CommandButton Command_Start 
      Caption         =   "�ɼ�"
      Height          =   375
      Left            =   2040
      TabIndex        =   4
      Top             =   5280
      Width           =   1695
   End
   Begin VB.Label Label8 
      Caption         =   "CH4:"
      Height          =   255
      Left            =   600
      TabIndex        =   18
      Top             =   3600
      Width           =   375
   End
   Begin VB.Label Label7 
      Caption         =   "CH5:"
      Height          =   255
      Left            =   3360
      TabIndex        =   17
      Top             =   3600
      Width           =   375
   End
   Begin VB.Label Label6 
      Caption         =   "CH6:"
      Height          =   255
      Left            =   600
      TabIndex        =   16
      Top             =   4680
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "CH7:"
      Height          =   255
      Left            =   3360
      TabIndex        =   15
      Top             =   4680
      Width           =   375
   End
   Begin VB.Label ѡ������ 
      Caption         =   "ѡ������"
      Height          =   255
      Left            =   840
      TabIndex        =   13
      Top             =   600
      Width           =   1455
   End
   Begin VB.Label Label5 
      Caption         =   "CH3:"
      Height          =   255
      Left            =   3360
      TabIndex        =   3
      Top             =   2400
      Width           =   375
   End
   Begin VB.Label Label4 
      Caption         =   "CH2:"
      Height          =   255
      Left            =   600
      TabIndex        =   2
      Top             =   2520
      Width           =   375
   End
   Begin VB.Label Label3 
      Caption         =   "CH1:"
      Height          =   255
      Left            =   3360
      TabIndex        =   1
      Top             =   1440
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "CH0:"
      Height          =   255
      Left            =   600
      TabIndex        =   0
      Top             =   1560
      Width           =   375
   End
End
Attribute VB_Name = "Npt"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim InputRange As Integer
Dim Index As Integer
Dim strTemp As String
Dim bFirstWait As Boolean
Dim hDevice As Long
Dim DeviceID As Integer
Dim ADBuffer(0 To 4095) As Long                  ' ���仺����(�洢ԭʼ����)
Dim ADPara As PCI8510_PARA_AD   ' Ӳ������
Dim nReadSizeWords As Long                  ' ÿ�ζ�ȡAD���ݵĳ���(��)
Dim nRetSizeWords As Long
Dim dwErrorCode As Long
Dim nChannel As Integer
Dim fVolt As String


Private Sub Command_Start_Click()



    DeviceID = 0
    hDevice = PCI8510_CreateDevice(DeviceID) ' �����豸����
    If (hDevice = INVALID_HANDLE_VALUE) Then
    
       MsgBox "PCI8510_CreateDevice  Error..."
         
        Exit Sub
    End If

    InputRange = Me.Combo_InputRange.ListIndex ' Ҫ���û��Ӽ�����ѡ����������

    'memset(&ADPara, sizeof(ADPara), 0x00) ' ǿ�Ƴ�ʼ��Ϊ0����ȷ��������������ȷ��״̬(ǿ�ҽ���)
   ' Ԥ��Ӳ������
    ADPara.Gains0 = PCI8510_GAINS_1MULT              ' ģ��ͨ��0��������
    ADPara.Gains1 = PCI8510_GAINS_1MULT              ' ģ��ͨ��1��������
    ADPara.Gains2 = PCI8510_GAINS_1MULT               ' ģ��ͨ��2��������
    ADPara.Gains3 = PCI8510_GAINS_1MULT               ' ģ��ͨ��3��������
    ADPara.Gains4 = PCI8510_GAINS_1MULT               ' ģ��ͨ��4��������
    ADPara.Gains5 = PCI8510_GAINS_1MULT               ' ģ��ͨ��5��������
    ADPara.Gains6 = PCI8510_GAINS_1MULT               ' ģ��ͨ��6��������
    ADPara.Gains7 = PCI8510_GAINS_1MULT               ' ģ��ͨ��7��������
    ADPara.InputRange = InputRange
    
    ADPara.TriggerMode = PCI8510_TRIGMODE_SOFT
    ADPara.TriggerSource = PCI8510_TRIGSRC_ATR         ' ����Դ
    ADPara.TriggerType = PCI8510_TRIGTYPE_EDGE           ' ��������Ϊ���ش���
    ADPara.TriggerDir = PCI8510_TRIGDIR_NEGATIVE            ' ��������Ϊ����
    ADPara.TrigLevelVolt = 0            ' ������ƽ
    ADPara.ClockSource = PCI8510_CLOCKSRC_IN           ' ʱ��Դѡ�ð���ʱ��Դ
    ADPara.bClockOutput = False          ' ��ֹʱ�����
    ADPara.OutTrigSource = PCI8510_OUTTRIGSRC_TRIGGER0
    ADPara.OutTrigSourceDir = False
        
    If (PCI8510_InitDeviceProAD(hDevice, ADPara) = False) Then ' ��ʼ��Ӳ��
    
        
        MsgBox "PCI8510_InitDeviceProAD  Error.."
         
        Return
    End If
    nReadSizeWords = 4096
    If (PCI8510_StartDeviceProAD(hDevice) = False) Then ' �����豸
    
       
       MsgBox "PCI8510_StartDeviceProAD  Error.."
         
    End If
    
     bFirstWait = True ' Ϊÿ�εȴ�ֻ��ʾһ����ʾ
   
        If (PCI8510_ReadDeviceProAD_Npt(hDevice, ADBuffer(0), nReadSizeWords, nRetSizeWords) = False) Then
        
            
             MsgBox "PCI8510_ReadDeviceProAD_Npt  Error.."
            GoTo ExitRead
        
        End If
       For Index = 0 To 7 Step 1
       
        For nChannel = 0 To 3 Step 1
               
         
            Select Case (InputRange)
            
            Case PCI8510_INPUT_0_P5000mV  ' 0V - +5V
                    fVolt = ((5000# / 65536) * ADBuffer(Index) And &HFFFF)
                    
                Case PCI8510_INPUT_0_P10000mV  ' 0V - +10V
                    fVolt = ((10000# / 65536) * ADBuffer(Index) And &HFFFF)
                   
                Case PCI8510_INPUT_N5000_P5000mV  ' -5V - +5V
                    fVolt = ((10000# / 65536) * (ADBuffer(Index) And &HFFFF)) - 5000#
                 
                Case PCI8510_INPUT_N10000_P10000mV  ' -10V - +10V
                    fVolt = ((20000# / 65536) * (ADBuffer(Index) And &HFFFF)) - 10000#
            Case Else
            MsgBox "����ѡ�����"
            End Select
                strTemp = fVolt
                Me.TextVolt(Index).Text = Format(strTemp, "00.00") ' ��ʾ��ѹֵ
               
        Next nChannel
    
    Next Index
ExitRead:
    PCI8510_ReleaseDeviceProAD (hDevice)  ' �ͷ�AD
    PCI8510_ReleaseDevice (hDevice)   ' �ͷ��豸����

  
End Sub
    

























Private Sub Form_Load()
Me.Combo_InputRange.ListIndex = 0
End Sub
