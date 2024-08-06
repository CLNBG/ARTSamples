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
   Begin VB.CommandButton Command1 
      Caption         =   "停止AO输出"
      Height          =   495
      Left            =   3480
      TabIndex        =   2
      Top             =   720
      Width           =   1455
   End
   Begin VB.CommandButton Command_button 
      Caption         =   "开始AO输出"
      Height          =   495
      Left            =   960
      TabIndex        =   0
      Top             =   720
      Width           =   1455
   End
   Begin VB.Label Label1 
      Caption         =   "Label1"
      Height          =   615
      Left            =   600
      TabIndex        =   1
      Top             =   1800
      Width           =   5655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim taskHandle As Long
Dim data(0 To 999) As Double
Dim datatmp(0 To 1) As Double
Dim sampsRead As Long
Dim bytesPerSamp As Long
Dim bWrittern As Boolean
Dim str() As Byte
Dim index As Long
Dim written As Long

Private Sub Command_button_Click()

    If ArtDAQ_CreateTask(0, taskHandle) <> 0 Then
        MsgBox "创建任务失败!"
        Exit Sub
    End If
    
    str = StrConv("Dev1/ao0" & vbNullChar, vbFromUnicode)
    ret = ArtDAQ_CreateAOVoltageChan(taskHandle, str(0), 0, -10#, 10#, ArtDAQ_Val_Volts, 0)
    If ret <> 0 Then
        MsgBox "创建AO失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    For index = 0 To 3999
        data(index) = 9.95 * Sin(index * 2# * pi / 1000#)
    Next
    
    If ArtDAQ_StartTask(taskHandle) <> 0 Then
        MsgBox "开始AO输出失败!"
        ArtDAQ_ClearTask (taskHandle)
        Exit Sub
    End If
    
    bWrittern = True
    index = 0
    
    Do While bWrittern = True
        datatmp(0) = data(index)
        If ArtDAQ_WriteAnalogF64(taskHandle, 1, 0, 10#, ArtDAQ_Val_GroupByChannel, datatmp(0), written, 0) <> 0 Then
            MsgBox "写入AO失败!"
            ArtDAQ_ClearTask (taskHandle)
            Exit Sub
        End If
        index = index + 1
        If index >= 1000 Then
            index = 0
        End If
        DoEvents
    Loop
    
End Sub

Private Sub Command1_Click()
    bWrittern = False
    If ArtDAQ_StopTask(taskHandle) <> 0 Then
        MsgBox "停止任务失败!"
    End If
    If ArtDAQ_ClearTask(taskHandle) <> 0 Then
        MsgBox "释放任务失败!"
    End If
End Sub

Private Sub Form_Load()
    Label1.Caption = "本例程演示的是采用默认参数进行AO输出的简易流程"
    bWrittern = False
End Sub
