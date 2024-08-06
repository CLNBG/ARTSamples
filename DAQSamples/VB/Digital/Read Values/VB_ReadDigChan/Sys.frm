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
   StartUpPosition =   3  '窗口缺省
   Begin VB.CommandButton Command_button 
      Caption         =   "读取DI"
      Height          =   495
      Left            =   2400
      TabIndex        =   0
      Top             =   240
      Width           =   1455
   End
   Begin VB.Label Label2 
      Height          =   1695
      Left            =   1440
      TabIndex        =   2
      Top             =   960
      Width           =   3255
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   375
      Left            =   600
      TabIndex        =   1
      Top             =   2880
      Width           =   5655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim taskHandle As Long
Dim data(0 To 99) As Byte
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim str() As Byte
Dim strTmp As String
Dim strr As String
Dim length As Long
Dim index As Long
Dim ret As Long
Dim bRun As Boolean

Private Sub Command_button_Click()
    
    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/port0/line0:7" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateDIChan(taskHandle, str(0), 0, ArtDAQ_Val_ChanForAllLines)
    If ret <> 0 Then
        MsgBox "通道配置失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始DO输出失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    If ArtDAQ_ReadDigitalLines(taskHandle, 1, 10#, ArtDAQ_Val_GroupByChannel, data(0), 100, sampsRead, bytesPerSamp, 0) <> 0 Then
        MsgBox "读取DI失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    strTmp = ""
    For index = 0 To 7
        strr = "Data acquired, channel" & CStr(index) & ":" & CStr(data(index))
        strTmp = strTmp & Chr$(10) & strr
    Next
    
    Label2.Caption = strTmp
    
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停DO输出失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Command1_Click()
    bRun = False
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是从多个line进行DI读取的简易流程"
    bRun = False
End Sub
