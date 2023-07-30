VERSION 5.00
Begin VB.Form Npt 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Form1"
   ClientHeight    =   3660
   ClientLeft      =   6705
   ClientTop       =   2985
   ClientWidth     =   4845
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3660
   ScaleWidth      =   4845
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   3
      Left            =   3000
      TabIndex        =   5
      Top             =   1920
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   2
      Left            =   1320
      TabIndex        =   4
      Top             =   1920
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   1
      Left            =   3000
      TabIndex        =   3
      Top             =   1320
      Width           =   975
   End
   Begin VB.CommandButton Command_Start 
      Caption         =   "采集"
      Height          =   495
      Left            =   2040
      TabIndex        =   2
      Top             =   2640
      Width           =   1455
   End
   Begin VB.TextBox TextVolt 
      Height          =   285
      Index           =   0
      Left            =   1320
      TabIndex        =   1
      Top             =   1320
      Width           =   975
   End
   Begin VB.ComboBox Combo_InputRange 
      Height          =   300
      ItemData        =   "Npt.frx":0000
      Left            =   1920
      List            =   "Npt.frx":0013
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   480
      Width           =   2535
   End
   Begin VB.Label Label2 
      Caption         =   "CH3"
      Height          =   255
      Index           =   3
      Left            =   2640
      TabIndex        =   10
      Top             =   1920
      Width           =   495
   End
   Begin VB.Label Label2 
      Caption         =   "CH2"
      Height          =   255
      Index           =   2
      Left            =   960
      TabIndex        =   9
      Top             =   1920
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
      Caption         =   "CH0"
      Height          =   255
      Index           =   0
      Left            =   960
      TabIndex        =   7
      Top             =   1320
      Width           =   495
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "量程选择"
      Height          =   255
      Left            =   480
      TabIndex        =   6
      Top             =   480
      Width           =   1095
   End
End
Attribute VB_Name = "Npt"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' 说明： 本程序演示了如何用程序查询方式读取AD数据
Option Explicit
Dim InputRange As Integer
Dim ADBuffer(0 To 8191) As Long      ' 分配缓冲区(存储原始数据)
Dim hDevice As Long
Dim DeviceLgcID As Integer
Dim ADPara As PCI9757_PARA_AD       ' 硬件参数
Dim nReadSizeWords As Long          ' 每次读取AD数据的长度(字)
Dim nRetSizeWords As Long
Dim ADData As Long
Dim fVolt As Double
Dim ChannelCount As Integer
Dim Index As Integer
Dim strTemp As String

Private Sub Command_Start_Click()
    DeviceLgcID = 0
    hDevice = INVALID_HANDLE_VALUE
    hDevice = PCI9757_CreateDevice(DeviceLgcID) ' 创建设备对象
    If hDevice = INVALID_HANDLE_VALUE Then ' 如果创建设备对象失败，则返回
        MsgBox "Create Device Error..."
        Exit Sub
    End If
    
    InputRange = Me.Combo_InputRange.ListIndex       ' 要求用户选择输入量程

    ' 预置硬件参数
     For Index = 0 To 3
    
        ADPara.bChannelArray(Index) = 1     ' 允许通道采样
        ADPara.InputRange(Index) = InputRange        ' 模拟通道输入量程
        
    Next Index
    
    ADPara.Frequency = 10000        ' 采样频率(Hz)
    ADPara.TriggerMode = 0
    ADPara.TriggerSource = PCI9757_TRIGMODE_SOFT    ' 触发源
    ADPara.TriggerType = PCI9757_TRIGTYPE_EDGE      ' 触发类型为边沿触发
    ADPara.TriggerDir = PCI9757_TRIGDIR_NEGATIVE       ' 触发方向为负向
    ADPara.ClockSource = PCI9757_CLOCKSRC_IN      ' 时钟源选用板内时钟源
    ADPara.TrigLevelVolt = 2000
    ADPara.TrigWindow = 40
    
    If PCI9757_InitDeviceProAD(hDevice, ADPara) = False Then        ' 初始化硬件
        MsgBox "PCI9757_InitDeviceProAD..."
    End If
    
    Sleep (10)

    ChannelCount = 4
    nReadSizeWords = 4096 - 4096 Mod ChannelCount ' 将数据长度字转换为双字
    
    If PCI9757_StartDeviceProAD(hDevice) = False Then   ' 启动设备
        MsgBox "StartDeviceAD Error..."
    End If
        
    If PCI9757_ReadDeviceProAD_Npt(hDevice, ADBuffer(0), nReadSizeWords, nRetSizeWords) = False Then
        MsgBox "PCI9757_ReadDeviceProAD_Npt Error..."
        GoTo ExitRead
    End If

    For Index = 0 To 3 Step 1
            ADData = ADBuffer(Index) And 65535
        ' 将原码转换为电压值
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
        Me.TextVolt(Index).Text = Format(strTemp, "00.0000")       ' 显示电压值
    Next Index
    
ExitRead:
    PCI9757_ReleaseDeviceProAD (hDevice)  ' 释放AD
    PCI9757_ReleaseDevice (hDevice)  ' 释放设备对象

End Sub

Private Sub Form_Load()
    Me.Combo_InputRange.ListIndex = 0
End Sub
