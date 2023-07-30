VERSION 5.00
Begin VB.Form DIO 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "PCI8510_DIO"
   ClientHeight    =   5850
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6135
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5850
   ScaleWidth      =   6135
   StartUpPosition =   3  '����ȱʡ
   Begin VB.Frame Frame2 
      Caption         =   "����������״̬"
      Height          =   4335
      Left            =   3360
      TabIndex        =   10
      Top             =   600
      Width           =   1935
      Begin VB.CommandButton Command_DI 
         Caption         =   "����7"
         Height          =   375
         Index           =   7
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   18
         Top             =   3720
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����6"
         Height          =   375
         Index           =   6
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   17
         Top             =   3240
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����5"
         Height          =   375
         Index           =   5
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   16
         Top             =   2760
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����4"
         Height          =   375
         Index           =   4
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   15
         Top             =   2280
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����3"
         Height          =   375
         Index           =   3
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   14
         Top             =   1800
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����2"
         Height          =   375
         Index           =   2
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   13
         Top             =   1320
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����1"
         Height          =   375
         Index           =   1
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   12
         Top             =   840
         Width           =   1095
      End
      Begin VB.CommandButton Command_DI 
         Caption         =   "����0"
         Height          =   375
         Index           =   0
         Left            =   480
         Style           =   1  'Graphical
         TabIndex        =   11
         Top             =   360
         Width           =   1095
      End
   End
   Begin VB.CommandButton Command_ReadDI 
      Caption         =   "��ȡDI"
      Height          =   375
      Left            =   3840
      TabIndex        =   9
      Top             =   5280
      Width           =   1335
   End
   Begin VB.Frame Frame1 
      Caption         =   "�������������"
      Height          =   4335
      Left            =   960
      TabIndex        =   0
      Top             =   600
      Width           =   1935
      Begin VB.CommandButton Command_DO 
         Caption         =   "����7[��]"
         Height          =   375
         Index           =   7
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   8
         Top             =   3720
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����6[��]"
         Height          =   375
         Index           =   6
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   7
         Top             =   3240
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����5[��]"
         Height          =   375
         Index           =   5
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   6
         Top             =   2760
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����4[��]"
         Height          =   375
         Index           =   4
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   5
         Top             =   2280
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����3[��]"
         Height          =   375
         Index           =   3
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   4
         Top             =   1800
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����2[��]"
         Height          =   375
         Index           =   2
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   3
         Top             =   1320
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����1[��]"
         Height          =   375
         Index           =   1
         Left            =   360
         Style           =   1  'Graphical
         TabIndex        =   2
         Top             =   840
         Width           =   1095
      End
      Begin VB.CommandButton Command_DO 
         Caption         =   "����0[��]"
         Height          =   375
         Index           =   0
         Left            =   360
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


Dim hDevice As Long
Dim DeviceID As Integer
Dim Index As Long

Private Sub Command_DO_Click(Index As Integer)
  
    
            If (hDevice = -1) Then
                MsgBox "PCI8510_CreateDevice Error..."
                   Exit Sub
            End If
   
            If bDOSts(Index) = 1 Then
                Me.Command_DO(Index).BackColor = RGB(192, 192, 192)
                Me.Command_DO(Index).Caption = "����" + CStr(Index) + "(��)"
                bDOSts(Index) = 0
            Else
                Me.Command_DO(Index).Caption = "����" + CStr(Index) + "(��)"
                Me.Command_DO(Index).BackColor = RGB(0, 255, 255)
                bDOSts(Index) = 1
            End If
    
    If PCI8510_SetDeviceDO(hDevice, bDOSts(0)) = False Then        ' ���������
        
        MsgBox "PCI8510_SetDeviceDO Error.... "
        
    End If
  
    
        
End Sub

Private Sub Command_ReadDI_Click()
        If (hDevice = -1) Then
         MsgBox "PCI8510_CreateDevice Error..."
            Exit Sub
        End If

    If PCI8510_GetDeviceDI(hDevice, bDISts(0)) = False Then         ' ����������
        
        MsgBox "PCI8510_GetDeviceDI Error.... "
    End If
   For Index = 0 To 7 Step 1
   
            If bDISts(Index) = 1 Then
        Me.Command_DI(Index).Caption = "����0[��]"
        Me.Command_DI(Index).BackColor = RGB(0, 255, 255)
    Else
        Me.Command_DI(Index).Caption = "����0[��]"
        Me.Command_DI(Index).BackColor = RGB(192, 192, 192)
    End If
  Next Index
  
    
End Sub



Private Sub Form_Load()
Me.Command_ReadDI.Enabled = False
    DeviceID = 0
    hDevice = PCI8510_CreateDevice(DeviceID)
    If (hDevice = -1) Then
     MsgBox "PCI8510_CreateDevice Error..."
        Exit Sub
    End If
    Me.Command_ReadDI.Enabled = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If hDevice = PCI8510_CreateDevice(DeviceID) Then
        PCI8510_ReleaseDevice (hDevice)
    End If
End Sub



