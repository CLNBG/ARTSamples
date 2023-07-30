VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1_Half"
   ClientHeight    =   4530
   ClientLeft      =   60
   ClientTop       =   600
   ClientWidth     =   4815
   LinkTopic       =   "Form1"
   ScaleHeight     =   4530
   ScaleWidth      =   4815
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   0
      Left            =   900
      TabIndex        =   7
      Top             =   1050
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   1
      Left            =   2940
      TabIndex        =   6
      Top             =   1050
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   2
      Left            =   900
      TabIndex        =   5
      Top             =   1770
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   3
      Left            =   2940
      TabIndex        =   4
      Top             =   1770
      Width           =   975
   End
   Begin VB.CommandButton CmdStart 
      Caption         =   "Start"
      Height          =   450
      Left            =   600
      TabIndex        =   3
      Top             =   3840
      Width           =   3135
   End
   Begin VB.ComboBox Cmb_InputRange 
      Height          =   300
      ItemData        =   "Form1.frx":0000
      Left            =   1440
      List            =   "Form1.frx":0010
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   420
      Width           =   2430
   End
   Begin VB.Frame Frame1 
      Height          =   3525
      Left            =   315
      TabIndex        =   1
      Top             =   210
      Width           =   3900
      Begin VB.TextBox TextVolt 
         Enabled         =   0   'False
         Height          =   270
         Index           =   7
         Left            =   2640
         TabIndex        =   19
         Top             =   2880
         Width           =   975
      End
      Begin VB.TextBox TextVolt 
         Enabled         =   0   'False
         Height          =   270
         Index           =   6
         Left            =   600
         TabIndex        =   18
         Top             =   2880
         Width           =   975
      End
      Begin VB.TextBox TextVolt 
         Enabled         =   0   'False
         Height          =   270
         Index           =   5
         Left            =   2640
         TabIndex        =   17
         Top             =   2160
         Width           =   975
      End
      Begin VB.TextBox TextVolt 
         Enabled         =   0   'False
         Height          =   270
         Index           =   4
         Left            =   600
         TabIndex        =   16
         Top             =   2160
         Width           =   975
      End
      Begin VB.Label Label13 
         Caption         =   "CH7"
         Height          =   255
         Left            =   2160
         TabIndex        =   25
         Top             =   2880
         Width           =   375
      End
      Begin VB.Label Label12 
         Caption         =   "CH6"
         Height          =   255
         Left            =   120
         TabIndex        =   24
         Top             =   2880
         Width           =   375
      End
      Begin VB.Label Label11 
         Caption         =   "CH5"
         Height          =   255
         Index           =   1
         Left            =   2160
         TabIndex        =   21
         Top             =   2160
         Width           =   375
      End
      Begin VB.Label Label10 
         Caption         =   "CH4"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   20
         Top             =   2160
         Width           =   375
      End
      Begin VB.Label Label9 
         Caption         =   "CH3"
         Height          =   225
         Left            =   2100
         TabIndex        =   15
         Top             =   1575
         Width           =   435
      End
      Begin VB.Label Label8 
         Caption         =   "CH2"
         Height          =   330
         Left            =   105
         TabIndex        =   14
         Top             =   1575
         Width           =   540
      End
      Begin VB.Label Label7 
         Caption         =   "CH1"
         Height          =   225
         Left            =   2100
         TabIndex        =   13
         Top             =   840
         Width           =   540
      End
      Begin VB.Label Label6 
         Caption         =   "CH0"
         Height          =   225
         Left            =   105
         TabIndex        =   12
         Top             =   840
         Width           =   435
      End
      Begin VB.Label Label1 
         Caption         =   "选择量程:"
         Height          =   330
         Index           =   0
         Left            =   105
         TabIndex        =   2
         Top             =   210
         Width           =   960
      End
   End
   Begin VB.Label Label10 
      Caption         =   "CH4"
      Height          =   255
      Index           =   2
      Left            =   2400
      TabIndex        =   23
      Top             =   3120
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "CH6"
      Height          =   255
      Index           =   1
      Left            =   360
      TabIndex        =   22
      Top             =   3120
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "CH0:"
      Height          =   255
      Left            =   420
      TabIndex        =   11
      Top             =   1050
      Width           =   375
   End
   Begin VB.Label Label3 
      Caption         =   "CH1:"
      Height          =   255
      Left            =   2460
      TabIndex        =   10
      Top             =   1050
      Width           =   375
   End
   Begin VB.Label Label4 
      Caption         =   "CH2:"
      Height          =   255
      Left            =   420
      TabIndex        =   9
      Top             =   1770
      Width           =   375
   End
   Begin VB.Label Label5 
      Caption         =   "CH3:"
      Height          =   255
      Left            =   2460
      TabIndex        =   8
      Top             =   1770
      Width           =   375
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim InputRange As Integer
Dim Index As Integer
Dim strTemp As String
Dim bFirstWait As Boolean
Dim ADStatus As PCI8510_STATUS_AD
Dim hDevice As Long
Dim DeviceID As Integer
Dim ADPara As PCI8510_PARA_AD ' 硬件参数
Dim nReadSizeWords As Long    ' 每次读取AD数据的长度(字)
Dim nRetSizeWords As Long
Dim ADBuffer(0 To 4095) As Long
Dim nChannel  As Integer
Dim fVolt As Double


Private Sub CmdStart_Click()



    DeviceID = 0
    hDevice = PCI8510_CreateDevice(DeviceID)  ' 创建设备对象
    If (hDevice = INVALID_HANDLE_VALUE) Then
    
       
        MsgBox "PCI8510_CreateDevice  Error...."
        
        Exit Sub   ' 如果创建设备对象失败，则返回
    End If

    InputRange = Cmb_InputRange.ListIndex  ' 要求用户从键盘上选择输入量程

    ''memset(&ADPara, sizeof(ADPara), 0x00)  ' 强制初始化为0，以确保各个参数处于确定状态(强烈建议)
    ' 预置硬件参数
    ADPara.Gains0 = PCI8510_GAINS_1MULT              ' 模拟通道0输入量程
    ADPara.Gains1 = PCI8510_GAINS_1MULT              ' 模拟通道1输入量程
    ADPara.Gains2 = PCI8510_GAINS_1MULT               ' 模拟通道2输入量程
    ADPara.Gains3 = PCI8510_GAINS_1MULT               ' 模拟通道3输入量程
    ADPara.Gains4 = PCI8510_GAINS_1MULT               ' 模拟通道4输入量程
    ADPara.Gains5 = PCI8510_GAINS_1MULT               ' 模拟通道5输入量程
    ADPara.Gains6 = PCI8510_GAINS_1MULT               ' 模拟通道6输入量程
    ADPara.Gains7 = PCI8510_GAINS_1MULT               ' 模拟通道7输入量程
    ADPara.Frequency = 10000             ' 采样频率(Hz)
    ADPara.InputRange = InputRange
    ADPara.TriggerMode = PCI8510_TRIGMODE_SOFT
    ADPara.TriggerSource = PCI8510_TRIGSRC_ATR         ' 触发源
    ADPara.TriggerType = PCI8510_TRIGTYPE_EDGE           ' 触发类型为边沿触发
    ADPara.TriggerDir = PCI8510_TRIGDIR_NEGATIVE            ' 触发方向为负向
    ADPara.TrigLevelVolt = 0            ' 触发电平
    ADPara.ClockSource = PCI8510_CLOCKSRC_IN           ' 时钟源选用板内时钟源
    ADPara.bClockOutput = False          ' 禁止时钟输出
    ADPara.OutTrigSource = PCI8510_OUTTRIGSRC_TRIGGER0
    ADPara.OutTrigSourceDir = False
        
    If (PCI8510_InitDeviceProAD(hDevice, ADPara) = False) Then ' 初始化硬件
    
      
        MsgBox "PCI8510_InitDeviceProAD  Error...."
        
        Return
   End If
    nReadSizeWords = 4096
    If (PCI8510_StartDeviceProAD(hDevice) = False) Then ' 启动设备
    
         MsgBox "PCI8510_StartDeviceProAD  Error...."
        
  End If
  
    
     bFirstWait = True  ' 为每次等待只显示一次提示
   
        While (True) ' 查询当前物理缓冲区数据是否已准备就绪
        
            If (PCI8510_GetDevStatusProAD(hDevice, ADStatus) = False) Then
            
                
                MsgBox "PCI8510_GetDevStatusProAD Error. "
                GoTo ExitRead
            End If
            
            If (ADStatus.bHalf) = 1 Then
            GoTo read
            End If
            
        Wend
read:
        If (PCI8510_ReadDeviceProAD_Half(hDevice, ADBuffer(0), 4096, nRetSizeWords) = False) Then
        
           
             MsgBox "PCI8510_ReadDeviceProAD_Half  Error...."
            GoTo ExitRead
        End If
        
       
        For Index = 0 To 7 Step 1
        
           For nChannel = 0 To 7 Step 1
            ' 处理同步通道对中的AIxA的数据(x表示某一通道对的序列号)
            
            ' 将AIxA原码转换为电压值(mV)
            Select Case (InputRange)
            
           
                
            Case PCI8510_INPUT_0_P5000mV: ' -5V - +5V
                    fVolt = ((5000# / 65536) * (ADBuffer(Index) And &HFFFF))
                  
                Case PCI8510_INPUT_0_P10000mV: ' -10V - +10V
                    fVolt = ((10000# / 65536) * (ADBuffer(Index) And &HFFFF))
                    
                Case PCI8510_INPUT_N5000_P5000mV: ' -5V - +5V
                    fVolt = ((10000# / 65536) * (ADBuffer(Index) And &HFFFF) - 5000#)
                   
                Case PCI8510_INPUT_N10000_P10000mV: ' -10V - +10V
                    fVolt = ((20000# / 65536) * (ADBuffer(Index) And &HFFFF) - 10000#)
                
Case Else
        MsgBox "量程选择错误  "
        End Select
            
           strTemp = fVolt
                Me.TextVolt(Index).Text = Format(strTemp, "00.00") ' 显示电压值
                 Next nChannel
            Next Index
    
ExitRead:
    PCI8510_ReleaseDeviceProAD (hDevice) ' 释放AD
    PCI8510_ReleaseDevice (hDevice)    ' 释放设备对象


  
End Sub
    
Private Sub Form_Load()
    Cmb_InputRange.ListIndex = 0
    nChannel = 0
   
End Sub















