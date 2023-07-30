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
      Caption         =   "采集"
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
      Caption         =   "量程选择"
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
' 说明： 本程序演示了如何用DMA方式读取AD数据

Option Explicit
Dim hDevice As Long         ' 设备句柄
Dim DeviceLgcID As Integer
Dim ADPara As PCI9757_PARA_AD           ' 硬件参数
Dim DMAStatus As PCI9757_STATUS_DMA     ' DMA状态参数
Dim nCurrentChannel As Long
Dim nChannel As Long
Dim ChannelCount As Long
Dim Index As Long
Dim ADData As Long
Dim fVolt As Single
Dim strTemp As String
Dim ADBuffer(0 To 4095, 0 To 63) As Long         ' 分配缓冲区(存储原始数据)
Dim hDmaEvent As Long

Private Sub Command_Start_Click()

    
    DeviceLgcID = 0
    hDevice = PCI9757_CreateDevice(DeviceLgcID) ' 创建设备对象
    If (hDevice = INVALID_HANDLE_VALUE) Then
        MsgBox "PCI9757_CreateDevice error"
        Exit Sub ' 如果创建设备对象失败，则返回
    End If


    ' 预置硬件参数
   
    For Index = 0 To 3
    
        ADPara.bChannelArray(Index) = 1     ' 允许通道采样
        ADPara.InputRange(Index) = Me.Combo_InputRange.ListIndex        ' 模拟通道输入量程
        
    Next Index
    
    ADPara.Frequency = 10000        ' 采样频率(Hz)
    ADPara.TriggerMode = 0
    ADPara.TriggerSource = PCI9757_TRIGMODE_SOFT    ' 触发源
    ADPara.TriggerType = PCI9757_TRIGTYPE_EDGE      ' 触发类型为边沿触发
    ADPara.TriggerDir = PCI9757_TRIGDIR_NEGATIVE       ' 触发方向为负向
    ADPara.ClockSource = PCI9757_CLOCKSRC_IN      ' 时钟源选用板内时钟源
    ADPara.TrigLevelVolt = 2000
    ADPara.TrigWindow = 40
  
    hDmaEvent = PCI9757_CreateSystemEvent()
    If (PCI9757_InitDeviceDmaAD(hDevice, hDmaEvent, ADBuffer(0, 0), 4096, 64, 4096, ADPara) = False) Then ' 初始化硬件
        MsgBox "PCI9757_InitDeviceDmaAD error"
       GoTo ExitRead1
    End If

    PCI9757_StartDeviceDmaAD (hDevice) ' 启动设备

    While (True) ' 查询当前物理缓冲区数据是否已准备就绪
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
                Me.TextVolt(Index).Text = Format(strTemp, "00.0000")          ' 显示电压值
            Next Index
            
            If (PCI9757_SetDevStatusDmaAD(hDevice, 0) = False) Then
                MsgBox ("PCI9757_SetDevStatusDmaAD error")
                GoTo ExitRead0
            End If
        End If


ExitRead0:
    PCI9757_ReleaseDeviceDmaAD (hDevice) ' 释放AD
    PCI9757_ReleaseSystemEvent (hDmaEvent)
ExitRead1:
    PCI9757_ReleaseDevice (hDevice) ' 释放设备对象

    
End Sub

Private Sub Form_Load()
    Me.Combo_InputRange.ListIndex = 0
End Sub
