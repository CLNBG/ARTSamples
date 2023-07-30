VERSION 5.00
Begin VB.Form DIO 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "DIO"
   ClientHeight    =   6240
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   8265
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6240
   ScaleWidth      =   8265
   StartUpPosition =   3  '窗口缺省
   Begin VB.Frame Frame1 
      Caption         =   "开关量输出控制"
      Height          =   4335
      Left            =   840
      TabIndex        =   10
      Top             =   600
      Width           =   2415
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关0[关]"
         Height          =   375
         Index           =   0
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   18
         Top             =   360
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关1[关]"
         Height          =   375
         Index           =   1
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   17
         Top             =   840
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关2[关]"
         Height          =   375
         Index           =   2
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   16
         Top             =   1320
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关3[关]"
         Height          =   375
         Index           =   3
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   15
         Top             =   1800
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关4[关]"
         Height          =   375
         Index           =   4
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   14
         Top             =   2280
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关5[关]"
         Height          =   375
         Index           =   5
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   13
         Top             =   2760
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关6[关]"
         Height          =   375
         Index           =   6
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   12
         Top             =   3240
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "开关7[关]"
         Height          =   375
         Index           =   7
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   11
         Top             =   3720
         Width           =   1095
      End
   End
   Begin VB.CommandButton Command_ReadDI 
      Caption         =   "读取DI"
      Height          =   375
      Left            =   3240
      TabIndex        =   9
      Top             =   5400
      Width           =   1335
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关7"
      Enabled         =   0   'False
      Height          =   375
      Index           =   7
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   8
      Top             =   4320
      Width           =   1095
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关6"
      Enabled         =   0   'False
      Height          =   375
      Index           =   6
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   7
      Top             =   3840
      Width           =   1095
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关5"
      Enabled         =   0   'False
      Height          =   375
      Index           =   5
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   6
      Top             =   3360
      Width           =   1095
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关4"
      Enabled         =   0   'False
      Height          =   375
      Index           =   4
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   5
      Top             =   2880
      Width           =   1095
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关3"
      Enabled         =   0   'False
      Height          =   375
      Index           =   3
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   4
      Top             =   2400
      Width           =   1095
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关2"
      Enabled         =   0   'False
      Height          =   375
      Index           =   2
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   3
      Top             =   1920
      Width           =   1095
   End
   Begin VB.CommandButton Command_DI 
      Caption         =   "开关1"
      Enabled         =   0   'False
      Height          =   375
      Index           =   1
      Left            =   5040
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   1440
      Width           =   1095
   End
   Begin VB.Frame Frame2 
      Caption         =   "开关量输入状态"
      Height          =   4335
      Left            =   4560
      TabIndex        =   0
      Top             =   600
      Width           =   2295
      Begin VB.CommandButton Command_DI 
         Caption         =   "开关0"
         Enabled         =   0   'False
         Height          =   375
         Index           =   0
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   1
         Top             =   360
         Width           =   1095
      End
   End
End
Attribute VB_Name = "DIO"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim bDISts(0 To 7) As Byte
Dim bDOSts(0 To 7) As Byte
Dim i As Long
Dim hDevice As Long
Dim DeviceLgcID As Integer

Private Sub Command_DO_Click(Index As Integer)
   If hDevice <> INVALID_HANDLE_VALUE Then
        If bDOSts(Index) = 1 Then
            Me.Command_DO(Index).BackColor = RGB(192, 192, 192)
            Me.Command_DO(Index).Caption = "开关" + CStr(Index) + "[关]"
            bDOSts(Index) = 0
        Else
            Me.Command_DO(Index).Caption = "开关" + CStr(Index) + "[开]"
            Me.Command_DO(Index).BackColor = RGB(0, 255, 255)
            bDOSts(Index) = 1
        End If
    If PCI8501_SetDeviceDO(hDevice, bDOSts(0)) = False Then        ' 开关量输出
        MsgBox "PCI8501_SetDeviceDO error "
    End If
   End If
End Sub

Private Sub Command_ReadDI_Click()
   If hDevice <> INVALID_HANDLE_VALUE Then
    If PCI8501_GetDeviceDI(hDevice, bDISts(0)) = False Then        ' 开关量输入
        MsgBox "PCI8501_GetDeviceDI error "
    End If
    For i = 0 To 7 Step 1
      If bDISts(i) = 1 Then
        Me.Command_DI(i).Caption = "开关" + CStr(i) + "[开]"
        Me.Command_DI(i).BackColor = RGB(0, 255, 255)
      Else
        Me.Command_DI(i).Caption = "开关" + CStr(i) + "[关]"
        Me.Command_DI(i).BackColor = RGB(192, 192, 192)
      End If
    Next i
   End If
End Sub

Private Sub Form_Load()
    DeviceLgcID = 0
    hDevice = PCI8501_CreateDevice(DeviceLgcID)
    If hDevice = INVALID_HANDLE_VALUE Then
        MsgBox "PCI8501_CreateDevice error"
        Exit Sub
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
   If hDevice <> INVALID_HANDLE_VALUE Then
    If PCI8501_ReleaseDevice(hDevice) = False Then
        MsgBox "PCI8501_ReleaseDevice Error..."
    End If
   End If
End Sub



