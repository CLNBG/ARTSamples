VERSION 5.00
Begin VB.Form Dma 
   Caption         =   "Form1"
   ClientHeight    =   4665
   ClientLeft      =   60
   ClientTop       =   420
   ClientWidth     =   6180
   LinkTopic       =   "Form1"
   ScaleHeight     =   4665
   ScaleWidth      =   6180
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   7
      Left            =   3480
      TabIndex        =   14
      Top             =   3240
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   6
      Left            =   1440
      TabIndex        =   13
      Top             =   3240
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   5
      Left            =   3480
      TabIndex        =   12
      Top             =   2640
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   4
      Left            =   1440
      TabIndex        =   11
      Top             =   2640
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   3
      Left            =   3480
      TabIndex        =   5
      Top             =   2040
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   2
      Left            =   1440
      TabIndex        =   4
      Top             =   2040
      Width           =   975
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   1
      Left            =   3480
      TabIndex        =   3
      Top             =   1320
      Width           =   975
   End
   Begin VB.CommandButton Command_Start 
      Caption         =   "采集"
      Height          =   375
      Left            =   1800
      TabIndex        =   2
      Top             =   4080
      Width           =   1695
   End
   Begin VB.TextBox TextVolt 
      Enabled         =   0   'False
      Height          =   270
      Index           =   0
      Left            =   1440
      TabIndex        =   1
      Top             =   1320
      Width           =   975
   End
   Begin VB.ComboBox Combo_InputRange 
      Height          =   300
      ItemData        =   "Dma.frx":0000
      Left            =   2280
      List            =   "Dma.frx":0010
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   480
      Width           =   1695
   End
   Begin VB.Label Label9 
      Caption         =   "CH7"
      Height          =   255
      Left            =   3000
      TabIndex        =   18
      Top             =   3240
      Width           =   375
   End
   Begin VB.Label Label8 
      Caption         =   "CH6"
      Height          =   255
      Left            =   960
      TabIndex        =   17
      Top             =   3240
      Width           =   375
   End
   Begin VB.Label Label7 
      Caption         =   "CH5"
      Height          =   255
      Left            =   3000
      TabIndex        =   16
      Top             =   2640
      Width           =   375
   End
   Begin VB.Label Label6 
      Caption         =   "CH4"
      Height          =   255
      Left            =   960
      TabIndex        =   15
      Top             =   2640
      Width           =   375
   End
   Begin VB.Label Label5 
      Caption         =   "CH3:"
      Height          =   255
      Left            =   3000
      TabIndex        =   10
      Top             =   2040
      Width           =   375
   End
   Begin VB.Label Label4 
      Caption         =   "CH2:"
      Height          =   255
      Left            =   960
      TabIndex        =   9
      Top             =   2040
      Width           =   375
   End
   Begin VB.Label Label3 
      Caption         =   "CH1:"
      Height          =   255
      Left            =   3000
      TabIndex        =   8
      Top             =   1320
      Width           =   375
   End
   Begin VB.Label Label2 
      Caption         =   "CH0:"
      Height          =   255
      Left            =   960
      TabIndex        =   7
      Top             =   1320
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "选择量程:"
      Height          =   255
      Left            =   1320
      TabIndex        =   6
      Top             =   480
      Width           =   855
   End
End
Attribute VB_Name = "Dma"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim ADBuffer(0 To 31, 0 To 4095) As Long
Dim g_ulSegmentID As Long
Dim InputRange As Integer
Dim hDevice As Long
Dim DeviceLgcID As Integer
Dim ADPara As PCI8510_PARA_AD
Dim DMAStatus As PCI8510_STATUS_DMA
Dim bFirstWait As Boolean
Dim nCurrentChannel As Integer
Dim nChannel As Integer
Dim ADData As Long
Dim fVolt As Single
Dim hDmaEvent As Long
Dim nTChannel As Long
Dim i As Integer
Dim Index As Integer
Dim strTemp As String

Private Sub Command_Start_Click()
    g_ulSegmentID = 0
    nChannel = 0
    DeviceLgcID = 0
    hDevice = INVALID_HANDLE_VALUE
    hDevice = PCI8510_CreateDevice(DeviceLgcID) ' 创建设备对象
     If hDevice = INVALID_HANDLE_VALUE Then
        MsgBox "CreateDevice Error"
        Exit Sub
    End If
        
    InputRange = Me.Combo_InputRange.ListIndex  ' 要求用户从键盘上选择输入量程
    

    ' 预置硬件参数
    ADPara.Gains0 = PCI8510_GAINS_1MULT             ' 模拟通道0输入量程
    ADPara.Gains1 = PCI8510_GAINS_1MULT             ' 模拟通道1输入量程
    ADPara.Gains2 = PCI8510_GAINS_1MULT             ' 模拟通道2输入量程
    ADPara.Gains3 = PCI8510_GAINS_1MULT             ' 模拟通道3输入量程
    ADPara.Gains4 = PCI8510_GAINS_1MULT             ' 模拟通道4输入量程
    ADPara.Gains5 = PCI8510_GAINS_1MULT             ' 模拟通道5输入量程
    ADPara.Gains6 = PCI8510_GAINS_1MULT             ' 模拟通道6输入量程
    ADPara.Gains7 = PCI8510_GAINS_1MULT             ' 模拟通道7输入量程
    ADPara.Frequency = 10000            ' 采样频率(Hz)
    ADPara.InputRange = InputRange
    ADPara.TriggerMode = PCI8510_TRIGMODE_SOFT
    ADPara.TriggerSource = PCI8510_TRIGSRC_ATR        ' 触发源
    ADPara.TriggerType = PCI8510_TRIGTYPE_EDGE          ' 触发类型为边沿触发
    ADPara.TriggerDir = PCI8510_TRIGDIR_NEGATIVE           ' 触发方向为负向
    ADPara.TrigLevelVolt = 0           ' 触发电平
    ADPara.ClockSource = PCI8510_CLOCKSRC_IN          ' 时钟源选用板内时钟源
    ADPara.bClockOutput = False         ' 禁止时钟输出
    ADPara.OutTrigSource = PCI8510_OUTTRIGSRC_TRIGGER0
    ADPara.OutTrigSourceDir = False
    
    hDmaEvent = PCI8510_CreateSystemEvent()
    If PCI8510_InitDeviceDmaAD(hDevice, hDmaEvent, ADBuffer(0, 0), 4096, 32, 4096, ADPara) = False Then  ' 初始化硬件
    
        MsgBox "PCI8510_InitDeviceDmaAD Error..."
       Exit Sub
    End If

    PCI8510_StartDeviceDmaAD (hDevice) ' 启动设备
    
        While (True) ' 查询当前物理缓冲区数据是否已准备就绪
       
            If (WaitForSingleObject(hDmaEvent, 100) = 0) Then ' 等待DMA事件
                GoTo Read
            Else
                MsgBox "Wait"
            End If
         Wend
Read:
        
        If PCI8510_GetDevStatusDmaAD(hDevice, DMAStatus) = False Then
        
            MsgBox ("Get Device Status Error...")
            Exit Sub
        End If

        If DMAStatus.bBufferOverflow Then
        
            MsgBox "DMA Overflow..."
            ' Sleep(3000) ' 当溢出时,为了让用户看得清楚,延时3秒
        End If
        
        For i = 0 To 32 Step 1
        
            If (DMAStatus.bSegmentSts(g_ulSegmentID)) Then
            
                For Index = 0 To 7 Step 1
                
                   ' For nChannel = 0 To 7 Step 1
                    
                        Select Case InputRange
                        
                        Case PCI8510_INPUT_0_P5000mV: ' -5V - +5V
                            fVolt = ((5000# / 65536) * (ADBuffer(Index, g_ulSegmentID) And &HFFFF))
                            
                        Case PCI8510_INPUT_0_P10000mV: ' -10V - +10V
                            fVolt = ((10000# / 65536) * (ADBuffer(Index, g_ulSegmentID) And &HFFFF))
                            
                        Case PCI8510_INPUT_N5000_P5000mV: ' -5V - +5V
                            fVolt = ((10000# / 65536) * (ADBuffer(Index, g_ulSegmentID) And &HFFFF) - 5000#)
                            
                        Case PCI8510_INPUT_N10000_P10000mV: ' -10V - +10V
                            fVolt = ((20000# / 65536) * (ADBuffer(Index, g_ulSegmentID) And &HFFFF) - 10000#)
                    
                        Case Else
                            MsgBox "量程选择错误"
                         
                        End Select
                       strTemp = fVolt
                     
                     Me.TextVolt(Index).Text = Format(strTemp, "00.00")          ' 显示电压值
                        
                        
                   ' Next nChannel
                Next Index ' for(Index=0 Index<64 Index++)
                
                If PCI8510_SetDevStatusDmaAD(hDevice, g_ulSegmentID) = False Then
                
                     MsgBox "SetDevStatusDmaAD Error..."
                
                    Exit Sub
                End If
                
                g_ulSegmentID = (g_ulSegmentID + 1) Mod 64
           End If  ' for(Index=0 Index<64 Index++)
        Next i ' end of for(SegmentID=0 SegmentID<SEGMENT_COUNT SegmentID++)
  

End Sub
Private Sub Form_Load()
     Me.Combo_InputRange.ListIndex = 0
End Sub
Private Sub Form_Unload(Cancel As Integer)

    PCI8510_ReleaseDeviceDmaAD (hDevice) ' 释放AD
    PCI8510_ReleaseSystemEvent (hDmaEvent)

    PCI8510_ReleaseDevice (hDevice) ' 释放设备对象
   
End Sub


