VERSION 5.00
Begin VB.Form Half 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   3555
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4875
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3555
   ScaleWidth      =   4875
   StartUpPosition =   3  '����ȱʡ
   Begin VB.ComboBox Combo_InputRange 
      Height          =   300
      ItemData        =   "Half.frx":0000
      Left            =   1920
      List            =   "Half.frx":0013
      Style           =   2  'Dropdown List
      TabIndex        =   5
      Top             =   480
      Width           =   2535
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   0
      Left            =   1320
      TabIndex        =   4
      Top             =   1320
      Width           =   975
   End
   Begin VB.CommandButton Command_Start 
      Caption         =   "�ɼ�"
      Height          =   495
      Left            =   2040
      TabIndex        =   3
      Top             =   2640
      Width           =   1455
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   1
      Left            =   3000
      TabIndex        =   2
      Top             =   1320
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   2
      Left            =   1320
      TabIndex        =   1
      Top             =   1920
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   3
      Left            =   3000
      TabIndex        =   0
      Top             =   1920
      Width           =   975
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "����ѡ��"
      Height          =   255
      Left            =   600
      TabIndex        =   10
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label Label2 
      Caption         =   "CH0"
      Height          =   255
      Index           =   0
      Left            =   960
      TabIndex        =   9
      Top             =   1320
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH1"
      Height          =   255
      Index           =   1
      Left            =   2640
      TabIndex        =   8
      Top             =   1320
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH2"
      Height          =   255
      Index           =   2
      Left            =   960
      TabIndex        =   7
      Top             =   1920
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH3"
      Height          =   255
      Index           =   3
      Left            =   2640
      TabIndex        =   6
      Top             =   1920
      Width           =   495
   End
End
Attribute VB_Name = "Half"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' ˵���� ��������ʾ������ó����ѯ��ʽ��ȡAD����
Option Explicit

Dim hDevice As Long
Dim ADStatus As PCI9757_STATUS_AD
Dim ADPara As PCI9757_PARA_AD       ' Ӳ������
Dim DeviceID As Long
Dim nReadSizeWords As Long  ' ÿ�ζ�ȡAD���ݵĳ���(��)
Dim nRetSizeWords As Long
Dim Index As Integer
Dim ADData As Long
Dim fVolt As Double
Dim strTemp As String
Dim InputRange As Long
Dim ADBuffer(0 To 4095) As Long     ' ���仺����(�洢ԭʼ����)


Private Sub Command_Start_Click()

    DeviceID = 0
    hDevice = INVALID_HANDLE_VALUE
    hDevice = PCI9757_CreateDevice(DeviceID)         ' �����豸����
    If hDevice = INVALID_HANDLE_VALUE Then
        MsgBox "PCI9757_CreateDevice Error...."
        Exit Sub
    End If

    InputRange = Me.Combo_InputRange.ListIndex              ' Ҫ���û�ѡ����������

      ' Ԥ��Ӳ������
    For Index = 0 To 3
    
        ADPara.bChannelArray(Index) = 1     ' ����ͨ��0����
        ADPara.InputRange(Index) = InputRange        ' ģ��ͨ����������
    
    Next Index
    
    ADPara.Frequency = 25000        ' ����Ƶ��(Hz)
    ADPara.TriggerMode = 0
    ADPara.TriggerSource = PCI9757_TRIGMODE_SOFT    ' ����Դ
    ADPara.TriggerType = PCI9757_TRIGTYPE_EDGE      ' ��������Ϊ���ش���
    ADPara.TriggerDir = PCI9757_TRIGDIR_POSITIVE       ' ��������Ϊ����
    ADPara.ClockSource = PCI9757_CLOCKSRC_IN      ' ʱ��Դѡ�ð���ʱ��Դ
    ADPara.TrigLevelVolt = 2000      ' ��������Ϊ���ش���
    ADPara.TrigWindow = 40       ' ����������
   
    ' ��ʼ��Ӳ��
    If PCI9757_InitDeviceProAD(hDevice, ADPara) = False Then
        MsgBox "PCI9757_InitDeviceProAD Error...."
    End If
    
    nReadSizeWords = 1024 * 4 ' �����ݳ�����ת��Ϊ˫��
    
    If PCI9757_StartDeviceProAD(hDevice) = False Then ' �����豸
        MsgBox "StartDeviceAD Error..."
    End If
    

    While (True) ' ��ѯ��ǰ�������������Ƿ���׼������
        If PCI9757_GetDevStatusProAD(hDevice, ADStatus) = False Then
            MsgBox "PCI9757_GetDevStatusProAD Error...."
            GoTo ExitRead
        End If
            
        If ADStatus.bHalf = 1 Then
            GoTo Read        ' ������FIFO�洢���������ﵽ�������ϣ����˳�״̬��ѯ,��ʼ��ȡ��������
        End If
    Wend
    
Read:
    If PCI9757_ReadDeviceProAD_Half(hDevice, ADBuffer(0), nReadSizeWords, nRetSizeWords) = False Then
        MsgBox "PCI9757_ReadDeviceProAD_Half"
        GoTo ExitRead
    End If
        
    For Index = 0 To 3 Step 1
            ADData = ADBuffer(Index) And 65535
        
        ' ��ԭ��ת��Ϊ��ѹֵ
        Select Case InputRange
         
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
        Me.TextVolt(Index).Text = Format(strTemp, "00.0000")      ' ��ʾ��ѹֵ
    Next Index
    
ExitRead:
    PCI9757_ReleaseDeviceProAD (hDevice)  ' �ͷ�AD
    PCI9757_ReleaseDevice (hDevice)  ' �ͷ��豸����

End Sub

Private Sub Form_Load()
    Me.Combo_InputRange.ListIndex = 0
End Sub
