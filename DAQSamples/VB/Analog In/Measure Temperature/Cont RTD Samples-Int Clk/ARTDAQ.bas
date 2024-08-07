Attribute VB_Name = "ArtDAQ"
'自定义无符号16位整型
Public Type UInteger
    data(0 To 1) As Byte
End Type

'自定义无符号32位整型
Public Type ULong
    data(0 To 3) As Byte
End Type

'********** Buffer Attributes **********
Public Const ArtDAQ_Buf_Input_BufSize = &H186C
Public Const ArtDAQ_Buf_Input_OnbrdBufSize = &H230A
Public Const ArtDAQ_Buf_Output_BufSize = &H186D
Public Const ArtDAQ_Buf_Output_OnbrdBufSize = &H230B

'********** Calibration Info Attributes **********
Public Const ArtDAQ_SelfCal_Supported = &H1860
'********** System Attributes **********
Public Const ArtDAQ_Sys_Tasks = &H1267
Public Const ArtDAQ_Sys_DevNames = &H193B
Public Const ArtDAQ_Sys_MajorVersion = &H1272
Public Const ArtDAQ_Sys_MinorVersion = &H1923
Public Const ArtDAQ_Sys_UpdateVersion = &H2F22

'********** Task Attributes **********
Public Const ArtDAQ_Task_Name = &H1276
Public Const ArtDAQ_Task_Channels = &H1273
Public Const ArtDAQ_Task_NumChans = &H2181
Public Const ArtDAQ_Task_Devices = &H230E
Public Const ArtDAQ_Task_NumDevices = &H29BA
Public Const ArtDAQ_Task_Complete = &H1274

'********** Device Attributes **********
Public Const ArtDAQ_Dev_IsSimulated = &H22CA
Public Const ArtDAQ_Dev_ProductCategory = &H29A9
Public Const ArtDAQ_Dev_ProductType = &H631
Public Const ArtDAQ_Dev_ProductNum = &H231D
Public Const ArtDAQ_Dev_SerialNum = &H632
Public Const ArtDAQ_Dev_AI_PhysicalChans = &H231E
Public Const ArtDAQ_Dev_AI_SupportedMeasTypes = &H2FD2
Public Const ArtDAQ_Dev_AI_MaxSingleChanRate = &H298C
Public Const ArtDAQ_Dev_AI_MaxMultiChanRate = &H298D
Public Const ArtDAQ_Dev_AI_MinRate = &H298E
Public Const ArtDAQ_Dev_AI_SampTimingTypes = &H3163
Public Const ArtDAQ_Dev_AI_SampModes = &H2FDC
Public Const ArtDAQ_Dev_AI_TrigUsage = &H2986
Public Const ArtDAQ_Dev_AI_VoltageRngs = &H2990
Public Const ArtDAQ_Dev_AI_VoltageIntExcitDiscreteVals = &H29C9
Public Const ArtDAQ_Dev_AI_VoltageIntExcitRangeVals = &H29CA
Public Const ArtDAQ_Dev_AI_ChargeRngs = &H3111
Public Const ArtDAQ_Dev_AI_CurrentRngs = &H2991
Public Const ArtDAQ_Dev_AI_CurrentIntExcitDiscreteVals = &H29CB
Public Const ArtDAQ_Dev_AI_BridgeRngs = &H2FD0
Public Const ArtDAQ_Dev_AI_ResistanceRngs = &H2A15
Public Const ArtDAQ_Dev_AI_FreqRngs = &H2992
Public Const ArtDAQ_Dev_AI_Couplings = &H2994


Public Const ArtDAQ_Dev_AO_PhysicalChans = &H231F
Public Const ArtDAQ_Dev_AO_SupportedOutputTypes = &H2FD3
Public Const ArtDAQ_Dev_AO_SampTimingTypes = &H3165
Public Const ArtDAQ_Dev_AO_SampModes = &H2FDD
Public Const ArtDAQ_Dev_AO_MaxRate = &H2997
Public Const ArtDAQ_Dev_AO_MinRate = &H2998
Public Const ArtDAQ_Dev_AO_TrigUsage = &H2987
Public Const ArtDAQ_Dev_AO_VoltageRngs = &H299B
Public Const ArtDAQ_Dev_AO_CurrentRngs = &H299C
Public Const ArtDAQ_Dev_DI_Lines = &H2320
Public Const ArtDAQ_Dev_DI_Ports = &H2321
Public Const ArtDAQ_Dev_DI_MaxRate = &H2999
Public Const ArtDAQ_Dev_DI_SampTimingEngines = &H3167
Public Const ArtDAQ_Dev_DI_TrigUsage = &H2988
Public Const ArtDAQ_Dev_DO_Lines = &H2322
Public Const ArtDAQ_Dev_DO_Ports = &H2323
Public Const ArtDAQ_Dev_DO_MaxRate = &H299A
Public Const ArtDAQ_Dev_DO_SampTimingTypes = &H3168
Public Const ArtDAQ_Dev_DO_TrigUsage = &H2989
Public Const ArtDAQ_Dev_CI_PhysicalChans = &H2324
Public Const ArtDAQ_Dev_CI_SupportedMeasTypes = &H2FD4
Public Const ArtDAQ_Dev_CI_TrigUsage = &H298A
Public Const ArtDAQ_Dev_CI_SampTimingTypes = &H299E
Public Const ArtDAQ_Dev_CI_SampModes = &H2FDE
Public Const ArtDAQ_Dev_CI_MaxSize = &H299F
Public Const ArtDAQ_Dev_CI_MaxTimebase = &H29A0
Public Const ArtDAQ_Dev_CO_PhysicalChans = &H2325
Public Const ArtDAQ_Dev_CO_SupportedOutputTypes = &H2FD5
Public Const ArtDAQ_Dev_CO_SampTimingTypes = &H2F5B
Public Const ArtDAQ_Dev_CO_SampModes = &H2FDF
Public Const ArtDAQ_Dev_CO_TrigUsage = &H298B
Public Const ArtDAQ_Dev_CO_MaxSize = &H29A1
Public Const ArtDAQ_Dev_CO_MaxTimebase = &H29A2
Public Const ArtDAQ_Dev_BusType = &H2326
Public Const ArtDAQ_Dev_PCI_BusNum = &H2327
Public Const ArtDAQ_Dev_PCI_DevNum = &H2328
Public Const ArtDAQ_Dev_PXI_ChassisNum = &H2329
Public Const ArtDAQ_Dev_PXI_SlotNum = &H232A
Public Const ArtDAQ_Dev_TCPIP_Hostname = &H2A8B
Public Const ArtDAQ_Dev_TCPIP_EthernetIP = &H2A8C
Public Const ArtDAQ_Dev_TCPIP_WirelessIP = &H2A8D
Public Const ArtDAQ_Dev_Terminals = &H2A40

'********** Physical Channel Attributes **********
Public Const ArtDAQ_PhysicalChan_AI_SupportedMeasTypes = &H2FD7
Public Const ArtDAQ_PhysicalChan_AI_TermCfgs = &H2342
Public Const ArtDAQ_PhysicalChan_AI_InputSrcs = &H2FD8
Public Const ArtDAQ_PhysicalChan_AO_SupportedOutputTypes = &H2FD9
Public Const ArtDAQ_PhysicalChan_AO_TermCfgs = &H29A3
Public Const ArtDAQ_PhysicalChan_DI_PortWidth = &H29A4
Public Const ArtDAQ_PhysicalChan_DI_SampClkSupported = &H29A5
Public Const ArtDAQ_PhysicalChan_DI_SampModes = &H2FE0
Public Const ArtDAQ_PhysicalChan_DI_ChangeDetectSupported = &H29A6
Public Const ArtDAQ_PhysicalChan_DO_PortWidth = &H29A7
Public Const ArtDAQ_PhysicalChan_DO_SampClkSupported = &H29A8
Public Const ArtDAQ_PhysicalChan_DO_SampModes = &H2FE1
Public Const ArtDAQ_PhysicalChan_CI_SupportedMeasTypes = &H2FDA
Public Const ArtDAQ_PhysicalChan_CO_SupportedOutputTypes = &H2FDB

'********** Channel Attributes **********
Public Const ArtDAQ_AI_Max = &H17DD
Public Const ArtDAQ_AI_Min = &H17DE
Public Const ArtDAQ_AI_MeasType = &H695
Public Const ArtDAQ_AI_TermCfg = &H1097
Public Const ArtDAQ_AI_InputSrc = &H2198
Public Const ArtDAQ_AI_Voltage_Units = &H1094
Public Const ArtDAQ_AI_Current_Units = &H701
Public Const ArtDAQ_AI_Temp_Units = &H1033
Public Const ArtDAQ_AI_Thrmcpl_Type = &H1050
Public Const ArtDAQ_AI_Thrmcpl_ScaleType = &H29D0
Public Const ArtDAQ_AI_Thrmcpl_CJCSrc = &H1035
Public Const ArtDAQ_AI_Thrmcpl_CJCVal = &H1036
Public Const ArtDAQ_AI_Thrmcpl_CJCChan = &H1034
Public Const ArtDAQ_AI_RTD_Type = &H1032
Public Const ArtDAQ_AI_RTD_R0 = &H1030
Public Const ArtDAQ_AI_RTD_A = &H1010
Public Const ArtDAQ_AI_RTD_B = &H1011
Public Const ArtDAQ_AI_RTD_C = &H1013
Public Const ArtDAQ_AI_Thrmstr_A = &H18C9
Public Const ArtDAQ_AI_Thrmstr_B = &H18CB
Public Const ArtDAQ_AI_Thrmstr_C = &H18CA
Public Const ArtDAQ_AI_Thrmstr_R1 = &H1061
Public Const ArtDAQ_AI_ResistanceCfg = &H1881

Public Const ArtDAQ_AI_Strain_Units = &H981
Public Const ArtDAQ_AI_StrainGage_ForceReadFromChan = &H2FFA
Public Const ArtDAQ_AI_StrainGage_GageFactor = &H994
Public Const ArtDAQ_AI_StrainGage_PoissonRatio = &H998
Public Const ArtDAQ_AI_StrainGage_Cfg = &H982

Public Const ArtDAQ_AI_Excit_Src = &H17F4
Public Const ArtDAQ_AI_Excit_Val = &H17F5
Public Const ArtDAQ_AI_AutoZeroMode = &H1760
Public Const ArtDAQ_AI_Coupling = &H64

Public Const ArtDAQ_AI_Bridge_Units = &H2F92
Public Const ArtDAQ_AI_Bridge_Cfg = &H87
Public Const ArtDAQ_AI_Bridge_NomResistance = &H17EC
Public Const ArtDAQ_AI_Bridge_InitialVoltage = &H17ED
Public Const ArtDAQ_AI_Bridge_InitialRatio = &H2F86
Public Const ArtDAQ_AI_Bridge_ShuntCal_Enable = &H94
Public Const ArtDAQ_AI_CurrentShunt_Resistance = &H17F3
Public Const ArtDAQ_AI_OpenThrmcplDetectEnable = &H2F72
Public Const ArtDAQ_AI_Resistance_Units = &H955


Public Const ArtDAQ_AO_Max = &H1186
Public Const ArtDAQ_AO_Min = &H1187
Public Const ArtDAQ_AO_OutputType = &H1108
Public Const ArtDAQ_AO_Voltage_Units = &H1184
Public Const ArtDAQ_AO_Current_Units = &H1109
Public Const ArtDAQ_DI_NumLines = &H2178
Public Const ArtDAQ_DO_NumLines = &H2179
Public Const ArtDAQ_CI_Max = &H189C
Public Const ArtDAQ_CI_Min = &H189D
Public Const ArtDAQ_CI_MeasType = &H18A0
Public Const ArtDAQ_CI_Freq_Units = &H18A1
Public Const ArtDAQ_CI_Freq_StartingEdge = &H799
Public Const ArtDAQ_CI_Freq_MeasMeth = &H144
Public Const ArtDAQ_CI_Freq_MeasTime = &H145
Public Const ArtDAQ_CI_Freq_Div = &H147
Public Const ArtDAQ_CI_Period_Units = &H18A3
Public Const ArtDAQ_CI_Period_StartingEdge = &H852
Public Const ArtDAQ_CI_Period_MeasMeth = &H192C
Public Const ArtDAQ_CI_Period_MeasTime = &H192D
Public Const ArtDAQ_CI_Period_Div = &H192E
Public Const ArtDAQ_CI_CountEdges_InitialCnt = &H698
Public Const ArtDAQ_CI_CountEdges_ActiveEdge = &H697
Public Const ArtDAQ_CI_CountEdges_Term = &H18C7
Public Const ArtDAQ_CI_CountEdges_Dir = &H696
Public Const ArtDAQ_CI_CountEdges_DirTerm = &H21E1
Public Const ArtDAQ_CI_CountEdges_CountReset_Enable = &H2FAF
Public Const ArtDAQ_CI_CountEdges_CountReset_ResetCount = &H2FB0
Public Const ArtDAQ_CI_CountEdges_CountReset_Term = &H2FB1
Public Const ArtDAQ_CI_CountEdges_CountReset_DigFltr_MinPulseWidth = &H2FB4
Public Const ArtDAQ_CI_CountEdges_CountReset_ActiveEdge = &H2FB2
Public Const ArtDAQ_CI_PulseWidth_Units = &H823
Public Const ArtDAQ_CI_PulseWidth_Term = &H18AA
Public Const ArtDAQ_CI_PulseWidth_StartingEdge = &H825
Public Const ArtDAQ_CI_DutyCycle_Term = &H308D
Public Const ArtDAQ_CI_DutyCycle_StartingEdge = &H3092
Public Const ArtDAQ_CI_SemiPeriod_Units = &H18AF
Public Const ArtDAQ_CI_SemiPeriod_Term = &H18B0
Public Const ArtDAQ_CI_SemiPeriod_StartingEdge = &H22FE
Public Const ArtDAQ_CI_TwoEdgeSep_Units = &H18AC
Public Const ArtDAQ_CI_TwoEdgeSep_FirstTerm = &H18AD
Public Const ArtDAQ_CI_TwoEdgeSep_FirstEdge = &H833
Public Const ArtDAQ_CI_TwoEdgeSep_SecondTerm = &H18AE
Public Const ArtDAQ_CI_TwoEdgeSep_SecondEdge = &H834
Public Const ArtDAQ_CI_Pulse_Freq_Units = &H2F0B
Public Const ArtDAQ_CI_Pulse_Freq_Term = &H2F04
Public Const ArtDAQ_CI_Pulse_Freq_Start_Edge = &H2F05
Public Const ArtDAQ_CI_Pulse_Time_Units = &H2F13
Public Const ArtDAQ_CI_Pulse_Time_Term = &H2F0C
Public Const ArtDAQ_CI_Pulse_Time_StartEdge = &H2F0D
Public Const ArtDAQ_CI_Pulse_Ticks_Term = &H2F14
Public Const ArtDAQ_CI_Pulse_Ticks_StartEdge = &H2F15
Public Const ArtDAQ_CI_AngEncoder_Units = &H18A6
Public Const ArtDAQ_CI_AngEncoder_PulsesPerRev = &H875
Public Const ArtDAQ_CI_AngEncoder_InitialAngle = &H881
Public Const ArtDAQ_CI_LinEncoder_Units = &H18A9
Public Const ArtDAQ_CI_LinEncoder_DistPerPulse = &H911
Public Const ArtDAQ_CI_LinEncoder_InitialPos = &H915
Public Const ArtDAQ_CI_Encoder_DecodingType = &H21E6
Public Const ArtDAQ_CI_Encoder_AInputTerm = &H219D
Public Const ArtDAQ_CI_Encoder_AInputInvert = &H21FD
Public Const ArtDAQ_CI_Encoder_BInputTerm = &H219E
Public Const ArtDAQ_CI_Encoder_BInputInvert = &H21FE
Public Const ArtDAQ_CI_Encoder_ZInputTerm = &H219F
Public Const ArtDAQ_CI_Encoder_ZInputInvert = &H21FF
Public Const ArtDAQ_CI_Encoder_ZIndexEnable = &H890
Public Const ArtDAQ_CI_Encoder_ZIndexVal = &H888
Public Const ArtDAQ_CI_Encoder_ZIndexPhase = &H889
Public Const ArtDAQ_CI_Source_DigFltr_MinPulseWidth = &H21FC
Public Const ArtDAQ_CI_Gate_DigFltr_MinPulseWidth = &H2201
Public Const ArtDAQ_CI_Aux_DigFltr_MinPulseWidth = &H2206
Public Const ArtDAQ_CI_CtrTimebaseSrc = &H143
Public Const ArtDAQ_CI_CtrTimebaseRate = &H18B2
Public Const ArtDAQ_CI_CtrTimebaseActiveEdge = &H142
Public Const ArtDAQ_CO_OutputType = &H18B5
Public Const ArtDAQ_CO_Pulse_IdleState = &H1170
Public Const ArtDAQ_CO_Pulse_Term = &H18E1
Public Const ArtDAQ_CO_Pulse_Time_Units = &H18D6
Public Const ArtDAQ_CO_Pulse_HighTime = &H18BA
Public Const ArtDAQ_CO_Pulse_LowTime = &H18BB
Public Const ArtDAQ_CO_Pulse_Time_InitialDelay = &H18BC
Public Const ArtDAQ_CO_Pulse_DutyCyc = &H1176
Public Const ArtDAQ_CO_Pulse_Freq_Units = &H18D5
Public Const ArtDAQ_CO_Pulse_Freq = &H1178
Public Const ArtDAQ_CO_Pulse_Freq_InitialDelay = &H299
Public Const ArtDAQ_CO_Pulse_HighTicks = &H1169
Public Const ArtDAQ_CO_Pulse_LowTicks = &H1171
Public Const ArtDAQ_CO_Pulse_Ticks_InitialDelay = &H298
Public Const ArtDAQ_CO_CtrTimebaseSrc = &H339
Public Const ArtDAQ_CO_CtrTimebaseRate = &H18C2
Public Const ArtDAQ_CO_CtrTimebaseActiveEdge = &H341
Public Const ArtDAQ_CO_Count = &H293
Public Const ArtDAQ_CO_OutputState = &H294
Public Const ArtDAQ_CO_EnableInitialDelayOnRetrigger = &H2EC9
Public Const ArtDAQ_ChanType = &H187F
Public Const ArtDAQ_PhysicalChanName = &H18F5
Public Const ArtDAQ_ChanDescr = &H1926
Public Const ArtDAQ_ChanIsGlobal = &H2304

'********** Read Attributes **********
Public Const ArtDAQ_Read_AutoStart = &H1826
Public Const ArtDAQ_Read_OverWrite = &H1211
Public Const ArtDAQ_Read_NumChans = &H217B
Public Const ArtDAQ_Read_DigitalLines_BytesPerChan = &H217C

'********** Write Attributes **********
Public Const ArtDAQ_Write_RegenMode = &H1453
Public Const ArtDAQ_Write_NumChans = &H217E
Public Const ArtDAQ_Write_DigitalLines_BytesPerChan = &H217F
'********** Timing Attributes **********
Public Const ArtDAQ_Sample_Mode = &H1300
Public Const ArtDAQ_Sample_SampPerChan = &H1310
Public Const ArtDAQ_SampTimingType = &H1347
Public Const ArtDAQ_SampClk_Rate = &H1344
Public Const ArtDAQ_SampClk_MaxRate = &H22C8
Public Const ArtDAQ_SampClk_Src = &H1852
Public Const ArtDAQ_SampClk_ActiveEdge = &H1301
Public Const ArtDAQ_SampClk_Timebase_Src = &H1308
Public Const ArtDAQ_AIConv_Src = &H1502
Public Const ArtDAQ_RefClk_Src = &H1316
Public Const ArtDAQ_SyncPulse_Src = &H223D

'********** Trigger Attributes **********
Public Const ArtDAQ_StartTrig_Type = &H1393
Public Const ArtDAQ_StartTrig_Term = &H2F1E
Public Const ArtDAQ_DigEdge_StartTrig_Src = &H1407
Public Const ArtDAQ_DigEdge_StartTrig_Edge = &H1404
Public Const ArtDAQ_AnlgEdge_StartTrig_Src = &H1398
Public Const ArtDAQ_AnlgEdge_StartTrig_Slope = &H1397
Public Const ArtDAQ_AnlgEdge_StartTrig_Lvl = &H1396
Public Const ArtDAQ_AnlgEdge_StartTrig_Hyst = &H1395
Public Const ArtDAQ_AnlgWin_StartTrig_Src = &H1400
Public Const ArtDAQ_AnlgWin_StartTrig_When = &H1401
Public Const ArtDAQ_AnlgWin_StartTrig_Top = &H1403
Public Const ArtDAQ_AnlgWin_StartTrig_Btm = &H1402
Public Const ArtDAQ_StartTrig_Delay = &H1856
Public Const ArtDAQ_StartTrig_DelayUnits = &H18C8
Public Const ArtDAQ_StartTrig_DigFltr_MinPulseWidth = &H2224
Public Const ArtDAQ_StartTrig_Retriggerable = &H190F
Public Const ArtDAQ_RefTrig_Type = &H1419
Public Const ArtDAQ_RefTrig_PretrigSamples = &H1445
Public Const ArtDAQ_RefTrig_Term = &H2F1F
Public Const ArtDAQ_DigEdge_RefTrig_Src = &H1434
Public Const ArtDAQ_DigEdge_RefTrig_Edge = &H1430
Public Const ArtDAQ_AnlgEdge_RefTrig_Src = &H1424
Public Const ArtDAQ_AnlgEdge_RefTrig_Slope = &H1423
Public Const ArtDAQ_AnlgEdge_RefTrig_Lvl = &H1422
Public Const ArtDAQ_AnlgEdge_RefTrig_Hyst = &H1421
Public Const ArtDAQ_AnlgWin_RefTrig_Src = &H1426
Public Const ArtDAQ_AnlgWin_RefTrig_When = &H1427
Public Const ArtDAQ_AnlgWin_RefTrig_Top = &H1429
Public Const ArtDAQ_AnlgWin_RefTrig_Btm = &H1428
Public Const ArtDAQ_RefTrig_AutoTrigEnable = &H2EC1
Public Const ArtDAQ_RefTrig_AutoTriggered = &H2EC2
Public Const ArtDAQ_RefTrig_Delay = &H1483
Public Const ArtDAQ_RefTrig_DigFltr_MinPulseWidth = &H2ED8
Public Const ArtDAQ_PauseTrig_Type = &H1366
Public Const ArtDAQ_PauseTrig_Term = &H2F20
Public Const ArtDAQ_AnlgLvl_PauseTrig_Src = &H1370
Public Const ArtDAQ_AnlgLvl_PauseTrig_When = &H1371
Public Const ArtDAQ_AnlgLvl_PauseTrig_Lvl = &H1369
Public Const ArtDAQ_AnlgLvl_PauseTrig_Hyst = &H1368
Public Const ArtDAQ_AnlgWin_PauseTrig_Src = &H1373
Public Const ArtDAQ_AnlgWin_PauseTrig_When = &H1374
Public Const ArtDAQ_AnlgWin_PauseTrig_Top = &H1376
Public Const ArtDAQ_AnlgWin_PauseTrig_Btm = &H1375
Public Const ArtDAQ_DigLvl_PauseTrig_Src = &H1379
Public Const ArtDAQ_DigLvl_PauseTrig_When = &H1380
Public Const ArtDAQ_PauseTrig_DigFltr_MinPulseWidth = &H2229

'********** Export Signal Attributes **********
Public Const ArtDAQ_Exported_AIConvClk_OutputTerm = &H1687
Public Const ArtDAQ_Exported_RefClk_OutputTerm = &H226E
Public Const ArtDAQ_Exported_SampClk_OutputTerm = &H1663
Public Const ArtDAQ_Exported_PauseTrig_OutputTerm = &H1615
Public Const ArtDAQ_Exported_RefTrig_OutputTerm = &H590
Public Const ArtDAQ_Exported_StartTrig_OutputTerm = &H584
Public Const ArtDAQ_Exported_ChangeDetectEvent_OutputTerm = &H2197
Public Const ArtDAQ_Exported_ChangeDetectEvent_Pulse_Polarity = &H2303
Public Const ArtDAQ_Exported_CtrOutEvent_OutputTerm = &H1717
Public Const ArtDAQ_Exported_CtrOutEvent_OutputBehavior = &H174F
Public Const ArtDAQ_Exported_CtrOutEvent_Pulse_Polarity = &H1718
Public Const ArtDAQ_Exported_CtrOutEvent_Toggle_IdleState = &H186A
Public Const ArtDAQ_Exported_SampClkTimebase_OutputTerm = &H18F9
Public Const ArtDAQ_Exported_SyncPulseEvent_OutputTerm = &H223C
Public Const ArtDAQ_Exported_PositionComparsionEvent_PulseWidthMode = &H2973

'*** ArtDAQ Values **********************************
Public Const ArtDAQ_Val_Cfg_Default = -1

'*** Values for the Mode parameter of ArtDAQ_TaskControl ***
Public Const ArtDAQ_Val_Task_Start = 0
Public Const ArtDAQ_Val_Task_Stop = 1
Public Const ArtDAQ_Val_Task_Verify = 2
Public Const ArtDAQ_Val_Task_Commit = 3
Public Const ArtDAQ_Val_Task_Reserve = 4
Public Const ArtDAQ_Val_Task_Unreserve = 5
Public Const ArtDAQ_Val_Task_Abort = 6


'*** Values for ArtDAQ_AI_MeasType ***
'*** Values for the Line Grouping parameter of ArtDAQ_CreateDIChan and ArtDAQ_CreateDOChan ***
Public Const ArtDAQ_Val_ChanPerLine = 0
Public Const ArtDAQ_Val_ChanForAllLines = 1


'*** Values for Run Mode
'*** Value set RunMode ***
Public Const ArtDAQ_Val_ServiceOn = 1
Public Const ArtDAQ_Val_ServiceOff = 0

'*** Values for the Trigger Usage parameter - set of trigger types a device may support
'*** Values for TriggerUsageTypeBits
Public Const ArtDAQ_Val_Bit_TriggerUsageTypes_Start = 1 * 2 ^ 0
Public Const ArtDAQ_Val_Bit_TriggerUsageTypes_Reference = 1 * 2 ^ 1
Public Const ArtDAQ_Val_Bit_TriggerUsageTypes_Pause = 1 * 2 ^ 2

'*** Values for the Coupling Types parameter - set of coupling types a device may support
'*** Values for CouplingTypeBits
Public Const ArtDAQ_Val_Bit_CouplingTypes_AC = 1 * 2 ^ 0
Public Const ArtDAQ_Val_Bit_CouplingTypes_DC = 1 * 2 ^ 1
Public Const ArtDAQ_Val_Bit_CouplingTypes_Ground = 1 * 2 ^ 2

'*** Values for DAQmx_PhysicalChan_AI_TermCfgs and DAQmx_PhysicalChan_AO_TermCfgs
'*** Value set TerminalConfigurationBits ***
Public Const ArtDAQ_Val_Bit_TermCfg_RSE = 1 * 2 ^ 0
Public Const ArtDAQ_Val_Bit_TermCfg_NRSE = 1 * 2 ^ 1
Public Const ArtDAQ_Val_Bit_TermCfg_Diff = 1 * 2 ^ 2
Public Const ArtDAQ_Val_Bit_TermCfg_PseudoDIFF = 1 * 2 ^ 3



'***      Attribute Values  ***/

''*** Values for ArtDAQ_Dev_ProductCategory ***
''*** Value set ProductCategory ***
Public Const ArtDAQ_Val_MultiFunc_Asyn = 14643
Public Const ArtDAQ_Val_MultiFunc_Sync = 15858
Public Const ArtDAQ_Val_AOSeries = 14647
Public Const ArtDAQ_Val_DigitalIO = 14648
Public Const ArtDAQ_Val_TIOSeries = 14661
Public Const ArtDAQ_Val_DSA = 14649
Public Const ArtDAQ_Val_NetworkDAQ = 14829
Public Const ArtDAQ_Val_SCExpress = 15886
Public Const ArtDAQ_Val_Unknown = 12588

'*** Values for ArtDAQ_Dev_BusType ***
'*** Value set BusType ***
Public Const ArtDAQ_Val_PCI = 12582
Public Const ArtDAQ_Val_PCIe = 13612
Public Const ArtDAQ_Val_PXI = 12583
Public Const ArtDAQ_Val_PXIe = 14706
Public Const ArtDAQ_Val_USB = 12586
Public Const ArtDAQ_Val_TCPIP = 14828


'*** Values for ArtDAQ_AI_MeasType ***
'*** Values for ArtDAQ_Dev_AI_SupportedMeasTypes ***
'*** Values for ArtDAQ_PhysicalChan_AI_SupportedMeasTypes ***
'*** Value set AIMeasurementType ***
Public Const ArtDAQ_Val_Voltage = 10322
Public Const ArtDAQ_Val_Current = 10134
Public Const ArtDAQ_Val_Resistance = 10278
Public Const ArtDAQ_Val_Bridge = 15908
Public Const ArtDAQ_Val_Strain_Gage = 10300
Public Const ArtDAQ_Val_Voltage_IEPESensor = 15966
Public Const ArtDAQ_Val_Temp_TC = 10303
Public Const ArtDAQ_Val_Temp_Thrmstr = 10302
Public Const ArtDAQ_Val_Temp_RTD = 10301

'*** Values for ArtDAQ_AI_ResistanceCfg ***
'*** Value set ResistanceConfiguration ***
Public Const ArtDAQ_Val_2Wire = 2
Public Const ArtDAQ_Val_3Wire = 3
Public Const ArtDAQ_Val_4Wire = 4

'*** Values for ArtDAQ_AI_Resistance_Units ***
'*** Value set ResistanceUnits1 ***
Public Const ArtDAQ_Val_Ohms = 10384
Public Const ArtDAQ_Val_FromCustomScale = 10065

'*** Value set for the Units parameter of ArtDAQ_CreateAIThrmcplChan ***
Public Const ArtDAQ_Val_DegC = 10143
Public Const ArtDAQ_Val_DegF = 10144
Public Const ArtDAQ_Val_Kelvins = 10325
Public Const ArtDAQ_Val_DegR = 10145

'*** Values for ArtDAQ_AI_Thrmcpl_Type ***
'*** Value set ThermocoupleType1 ***
Public Const ArtDAQ_Val_J_Type_TC = 10072
Public Const ArtDAQ_Val_K_Type_TC = 10073
Public Const ArtDAQ_Val_N_Type_TC = 10077
Public Const ArtDAQ_Val_R_Type_TC = 10082
Public Const ArtDAQ_Val_S_Type_TC = 10085
Public Const ArtDAQ_Val_T_Type_TC = 10086
Public Const ArtDAQ_Val_B_Type_TC = 10047
Public Const ArtDAQ_Val_E_Type_TC = 10055

'*** Values for ArtDAQ_AI_RTD_Type ***
'*** Value set RTDType1 ***
Public Const ArtDAQ_Val_Pt3750 = 12481
Public Const ArtDAQ_Val_Pt3851 = 10071
Public Const ArtDAQ_Val_Pt3911 = 12482
Public Const ArtDAQ_Val_Pt3916 = 10069
Public Const ArtDAQ_Val_Pt3920 = 10053
Public Const ArtDAQ_Val_Pt3928 = 12483
Public Const ArtDAQ_Val_Custom = 10137

'*** Values for ArtDAQ_AI_Thrmcpl_CJCSrc ***
'*** Value set CJCSource1 ***
Public Const ArtDAQ_Val_BuiltIn = 10200
Public Const ArtDAQ_Val_ConstVal = 10116
Public Const ArtDAQ_Val_Chan = 10113
'*** Values for ArtDAQ_AI_AutoZeroMode ***
'*** Value set AutoZeroType1 ***
Public Const ArtDAQ_Val_Once = 10244
Public Const ArtDAQ_Val_EverySample = 10164


'*** Values for ArtDAQ_AI_Bridge_Cfg ***
'*** Value set BridgeConfiguration1 ***
Public Const ArtDAQ_Val_FullBridge = 10182
Public Const ArtDAQ_Val_HalfBridge = 10187
Public Const ArtDAQ_Val_QuarterBridge = 10270
Public Const ArtDAQ_Val_NoBridge = 10228
'*** Values for ArtDAQ_AI_Bridge_Units ***
'*** Value set BridgeUnits ***
Public Const ArtDAQ_Val_VoltsPerVolt = 15896
Public Const ArtDAQ_Val_mVoltsPerVolt = 15897
'Public Const ArtDAQ_Val_FromCustomScale = 10065


'*** Value set StrainGageBridgeType1 ***
Public Const ArtDAQ_Val_FullBridgeI = 10183
Public Const ArtDAQ_Val_FullBridgeII = 10184
Public Const ArtDAQ_Val_FullBridgeIII = 10185
Public Const ArtDAQ_Val_HalfBridgeI = 10188
Public Const ArtDAQ_Val_HalfBridgeII = 10189
Public Const ArtDAQ_Val_QuarterBridgeI = 10271
Public Const ArtDAQ_Val_QuarterBridgeII = 10272

'*** Value set StrainUnits1 ***
Public Const ArtDAQ_Val_Strain = 10299
'Public Const ArtDAQ_Val_FromCustomScale = 10065

'*** Values for ArtDAQ_CI_MeasType ***
'*** Values for ArtDAQ_Dev_CI_SupportedMeasTypes ***
'*** Values for ArtDAQ_PhysicalChan_CI_SupportedMeasTypes ***
'*** Value set CIMeasurementType ***
Public Const ArtDAQ_Val_Freq = 10179
Public Const ArtDAQ_Val_Period = 10256
Public Const ArtDAQ_Val_CountEdges = 10125
Public Const ArtDAQ_Val_PulseWidth = 10359
Public Const ArtDAQ_Val_SemiPeriod = 10289
Public Const ArtDAQ_Val_PulseFrequency = 15864
Public Const ArtDAQ_Val_PulseTime = 15865
Public Const ArtDAQ_Val_PulseTicks = 15866
Public Const ArtDAQ_Val_DutyCycle = 16070
Public Const ArtDAQ_Val_Position_AngEncoder = 10360
Public Const ArtDAQ_Val_Position_LinEncoder = 10361
Public Const ArtDAQ_Val_TwoEdgeSep = 10267

'*** Values for ArtDAQ_CO_OutputType ***
'*** Values for ArtDAQ_Dev_CO_SupportedOutputTypes ***
'*** Values for ArtDAQ_PhysicalChan_CO_SupportedOutputTypes ***
'*** Value set COOutputType ***
Public Const ArtDAQ_Val_Pulse_Time = 10269
Public Const ArtDAQ_Val_Pulse_Freq = 10119
Public Const ArtDAQ_Val_Pulse_Ticks = 10268

'*** Values for ArtDAQ_ChanType ***
'*** Value set ChannelType ***
Public Const ArtDAQ_Val_AI = 10100
Public Const ArtDAQ_Val_AO = 10102
Public Const ArtDAQ_Val_DI = 10151
Public Const ArtDAQ_Val_DO = 10153
Public Const ArtDAQ_Val_CI = 10131
Public Const ArtDAQ_Val_CO = 10132

'*** Values for ArtDAQ_AI_TermCfg ***
'*** Value set InputTermCfg ***
Public Const ArtDAQ_Val_RSE = 10083
Public Const ArtDAQ_Val_NRSE = 10078
Public Const ArtDAQ_Val_Diff = 10106
Public Const ArtDAQ_Val_PseudoDiff = 12529

'*** Values for ArtDAQ_AI_Coupling ***
'*** Value set Coupling1 ***
Public Const ArtDAQ_Val_AC = 10045
Public Const ArtDAQ_Val_DC = 10050
Public Const ArtDAQ_Val_GND = 10066

'*** Values for ArtDAQ_AI_Excit_Src ***
'*** Value set ExcitationSource ***
Public Const ArtDAQ_Val_Internal = 10200
Public Const ArtDAQ_Val_External = 10167
Public Const ArtDAQ_Val_None = 10230

'*** Values for ArtDAQ_AI_Voltage_Units ***
'*** Values for ArtDAQ_AO_Voltage_Units ***
'*** Value set VoltageUnits1 ***
Public Const ArtDAQ_Val_Volts = 10348

'*** Values for ArtDAQ_AI_Current_Units ***
'*** Values for ArtDAQ_AO_Current_Units ***
'*** Value set CurrentUnits1 ***
Public Const ArtDAQ_Val_Amps = 10342

'*** Values for ArtDAQ_CI_Freq_Units ***
'*** Value set FrequencyUnits3 ***
Public Const ArtDAQ_Val_Hz = 10373
Public Const ArtDAQ_Val_Ticks = 10304

'*** Values for ArtDAQ_CI_Period_Units ***
'*** Values for ArtDAQ_CI_PulseWidth_Units ***
'*** Values for ArtDAQ_CI_TwoEdgeSep_Units ***
'*** Values for ArtDAQ_CI_SemiPeriod_Units ***
'*** Value set TimeUnits3 ***
Public Const ArtDAQ_Val_Seconds = 10364


'*** Values for ArtDAQ_CI_AngEncoder_Units ***
'*** Value set AngleUnits2 ***
Public Const ArtDAQ_Val_Degrees = 10146
Public Const ArtDAQ_Val_Radians = 10273
'Public Const ArtDAQ_Val_Ticks = 10304

'*** Values for ArtDAQ_CI_LinEncoder_Units ***
'*** Value set LengthUnits3 ***
Public Const ArtDAQ_Val_Meters = 10219
Public Const ArtDAQ_Val_Inches = 10379
'Public Const ArtDAQ_Val_Ticks = 10304

'*** Values for ArtDAQ_CI_Freq_MeasMeth ***
'*** Values for ArtDAQ_CI_Period_MeasMeth ***
'*** Value set CounterFrequencyMethod ***
Public Const ArtDAQ_Val_LowFreq1Ctr = 10105
Public Const ArtDAQ_Val_HighFreq2Ctr = 10157
Public Const ArtDAQ_Val_LargeRng2Ctr = 10205

'*** Values for AArtDQ_CI_CountEdges_Dir ***
'*** Value set CountDirection1 ***
Public Const ArtDAQ_Val_CountUp = 10128
Public Const ArtDAQ_Val_CountDown = 10124
Public Const ArtDAQ_Val_ExtControlled = 10326

'*** Values for ArtDAQ_CI_Encoder_DecodingType ***
'*** Values for ArtDAQ_CI_Velocity_Encoder_DecodingType ***
'*** Value set EncoderType2 ***
Public Const ArtDAQ_Val_X1 = 10090
Public Const ArtDAQ_Val_X2 = 10091
Public Const ArtDAQ_Val_X4 = 10092
Public Const ArtDAQ_Val_TwoPulseCounting = 10313

'*** Values for ArtDAQ_CI_Encoder_ZIndexPhase ***
'*** Value set EncoderZIndexPhase1 ***
Public Const ArtDAQ_Val_AHighBHigh = 10040
Public Const ArtDAQ_Val_AHighBLow = 10041
Public Const ArtDAQ_Val_ALowBHigh = 10042
Public Const ArtDAQ_Val_ALowBLow = 10043

'*** Values for ArtDAQ_Exported_CtrOutEvent_OutputBehavior ***
'*** Value set ExportActions2 ***
Public Const ArtDAQ_Val_Pulse = 10265
Public Const ArtDAQ_Val_Toggle = 10307

'*** Values for ArtDAQ_Exported_CtrOutEvent_Pulse_Polarity ***
'*** Value set Polarity2 ***
Public Const ArtDAQ_Val_ActiveHigh = 10095
Public Const ArtDAQ_Val_ActiveLow = 10096

'*** Values for ArtDAQ_CI_Freq_StartingEdge ***
'*** Values for ArtDAQ_CI_Period_StartingEdge ***
'*** Values for ArtDAQ_CI_CountEdges_ActiveEdge ***
'*** Values for ArtDAQ_CI_CountEdges_CountReset_ActiveEdge ***
'*** Values for ArtDAQ_CI_DutyCycle_StartingEdge ***
'*** Values for ArtDAQ_CI_PulseWidth_StartingEdge ***
'*** Values for ArtDAQ_CI_TwoEdgeSep_FirstEdge ***
'*** Values for ArtDAQ_CI_TwoEdgeSep_SecondEdge ***
'*** Values for ArtDAQ_CI_SemiPeriod_StartingEdge ***
'*** Values for ArtDAQ_CI_Pulse_Freq_Start_Edge ***
'*** Values for ArtDAQ_CI_Pulse_Time_StartEdge ***
'*** Values for ArtDAQ_CI_Pulse_Ticks_StartEdge ***
'*** Values for ArtDAQ_SampClk_ActiveEdge ***
'*** Values for ArtDAQ_DigEdge_StartTrig_Edge ***
'*** Values for ArtDAQ_DigEdge_RefTrig_Edge ***
'*** Values for ArtDAQ_AnlgEdge_StartTrig_Slope ***
'*** Values for ArtDAQ_AnlgEdge_RefTrig_Slope ***
'*** Value set Edge1 ***
Public Const ArtDAQ_Val_Rising = 10280
Public Const ArtDAQ_Val_Falling = 10171

'*** Values for ArtDAQ_CI_CountEdges_GateWhen ***
'*** Values for ArtDAQ_CI_OutputState ***
'*** Values for ArtDAQ_CO_Pulse_IdleState ***
'*** Values for ArtDAQ_CO_OutputState ***
'*** Values for ArtDAQ_DigLvl_PauseTrig_When ***
'*** Values for ArtDAQ_Exported_CtrOutEvent_Toggle_IdleState ***
'*** Value set Level1 ***
Public Const ArtDAQ_Val_High = 10192
Public Const ArtDAQ_Val_Low = 10214

'*** Value set for the state parameter of ArtDAQ_SetDigitalPowerUpStates ***
'Public Const ArtDAQ_Val_High = 10192
'Public Const ArtDAQ_Val_Low = 10214
Public Const ArtDAQ_Val_Input = 10310

'*** Value set for the state parameter of ArtDAQ_ExportPositionComparsionEvent pulseWidthMode ***
Public Const ArtDAQ_Val_PulseWidth_Implicit = 10601
Public Const ArtDAQ_Val_PulseWidth_PreSet = 10602
'*** Value set AcquisitionType ***
Public Const ArtDAQ_Val_FiniteSamps = 10178
Public Const ArtDAQ_Val_ContSamps = 10123
Public Const ArtDAQ_Val_HWTimedSinglePoint = 12522


'*** Value for the Number of Samples per Channel parameter of ArtDAQ_ReadAnalogF64, ArtDAQ_ReadBinaryI16
Public Const ArtDAQ_Val_Auto = -1

'*** Values for the Fill Mode parameter of ArtDAQ_Readxxxx ***
Public Const ArtDAQ_Val_GroupByChannel = 0
Public Const ArtDAQ_Val_GroupByScanNumber = 1

'*** Values for ArtDAQ_Read_OverWrite ***
'*** Value set OverwriteMode1 ***
Public Const ArtDAQ_Val_OverwriteUnreadSamps = 10252
Public Const ArtDAQ_Val_DoNotOverwriteUnreadSamps = 10159

'*** Values for ArtDAQ_Write_RegenMode ***
'*** Value set RegenerationMode1 ***
Public Const ArtDAQ_Val_AllowRegen = 10097
Public Const ArtDAQ_Val_DoNotAllowRegen = 10158


'*** Values for ArtDAQ_SampTimingType ***
'*** Value set SampleTimingType ***
Public Const ArtDAQ_Val_SampOnClk = 10388
Public Const ArtDAQ_Val_Implicit = 10451
Public Const ArtDAQ_Val_OnDemand = 10390
Public Const ArtDAQ_Val_ChangeDetection = 12504

'*** Value set Signal ***
Public Const ArtDAQ_Val_AIConvertClock = 12484
Public Const ArtDAQ_Val_SampleClock = 12487
Public Const ArtDAQ_Val_RefClock = 12535
Public Const ArtDAQ_Val_PauseTrigger = 12489
Public Const ArtDAQ_Val_ReferenceTrigger = 12490
Public Const ArtDAQ_Val_StartTrigger = 12491
Public Const ArtDAQ_Val_CounterOutputEvent = 12494
Public Const ArtDAQ_Val_SampClkTimebase = 12495
Public Const ArtDAQ_Val_SyncPulseEvent = 12496
Public Const ArtDAQ_Val_PositionComparsionEvent = 10610
Public Const ArtDAQ_Val_ChangeDetectionEvent = 12511

'*** Values for ArtDAQ_PauseTrig_Type ***
'*** Value set TriggerType6 ***
Public Const ArtDAQ_Val_AnlgLvl = 10101
Public Const ArtDAQ_Val_AnlgWin = 10103
Public Const ArtDAQ_Val_DigLvl = 10152
Public Const ArtDAQ_Val_DigPattern = 10398

'*** Values for ArtDAQ_RefTrig_Type ***
'*** Value set TriggerType8 ***
Public Const ArtDAQ_Val_AnlgEdge = 10099
Public Const ArtDAQ_Val_DigEdge = 10150

'*** Values for ArtDAQ_AnlgWin_StartTrig_When ***
'*** Values for ArtDAQ_AnlgWin_RefTrig_When ***
'*** Value set WindowTriggerCondition1 ***
Public Const ArtDAQ_Val_EnteringWin = 10163
Public Const ArtDAQ_Val_LeavingWin = 10208

'*** Values for ArtDAQ_AnlgLvl_PauseTrig_When ***
'*** Value set ActiveLevel ***
Public Const ArtDAQ_Val_AboveLvl = 10093
Public Const ArtDAQ_Val_BelowLvl = 10107

'*** Values for ArtDAQ_AnlgWin_PauseTrig_When ***
'*** Value set WindowTriggerCondition2 ***
Public Const ArtDAQ_Val_InsideWin = 10199
Public Const ArtDAQ_Val_OutsideWin = 10251

'*** Values for ArtDAQ_StartTrig_DelayUnits ***
'*** Value set DigitalWidthUnits1 ***
Public Const ArtDAQ_Val_SampClkPeriods = 10286


'*** Values for Read/Write Data Format
Public Const ArtDAQ_Val_Binary_U32 = 1
Public Const ArtDAQ_Val_Voltage_F64 = 2
Public Const ArtDAQ_Val_CounterDutyCycleAndFrequency_F64 = 3
Public Const ArtDAQ_Val_CounterHighAndLowTimes_F64 = 4
Public Const ArtDAQ_Val_CounterHighAndLowTicks_U32 = 5

'*** ArtDAQ Function Declarations ****************
Declare Function ArtDAQ_LoadTask Lib "Art_DAQ" (ByRef taskName As Byte, ByRef TaskHandle As Long) As Long
Declare Function ArtDAQ_SaveTask Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef saveAs As Byte, ByRef author As Byte, ByVal options As ULong) As Long
Declare Function ArtDAQ_CreateTask Lib "Art_DAQ" (ByRef taskName As Byte, ByRef TaskHandle As Long) As Long
Declare Function ArtDAQ_StartTask Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_StopTask Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_ClearTask Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_TaskControl Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal action As Long) As Long

Declare Function ArtDAQ_WaitUntilTaskDone Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeToWait As Double) As Long
Declare Function ArtDAQ_IsTaskDone Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef isTaskDone As Long) As Long

Declare Function ArtDAQ_GetTaskAttribute Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal attributeType As Long, ByRef attributes As Long, ByVal size As Long) As Long
'******************************************************
'***Channel Configuration/Creation  ***
'******************************************************
Declare Function ArtDAQ_CreateAIVoltageChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal terminalConfig As Long, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateAICurrentChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal terminalConfig As Long, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal shuntResistorLoc As Long, ByVal extShuntResistorVal As Double, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateAIVoltageIEPEChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal terminalConfig As Long, ByVal coupling As Long, ByVal minVal As Double, ByVal maxVal As Double, ByVal currentExcitSource As Long, ByVal currentExcitVal As Double) As Long
Declare Function ArtDAQ_CreateAIThrmcplChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal thermocoupleType As Long, ByVal cjcSource As Long, ByVal cjcVal As Long, ByRef cjcChannel As Byte) As Long
Declare Function ArtDAQ_CreateAIRTDChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal rtdType As Long, ByVal resistanceConfig As Long, ByVal currentExcitSource As Long, ByVal currentExcitVal As Double, ByVal r0 As Double) As Long
Declare Function ArtDAQ_CreateAIThrmstrChanIex Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal resistanceConfig As Long, ByVal currentExcitSource As Long, ByVal currentExcitVal As Double, ByVal a As Double, ByVal b As Double, ByVal c As Double) As Long
Declare Function ArtDAQ_CreateAIThrmstrChanVex Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal resistanceConfig As Long, ByVal voltageExcitSource As Long, ByVal voltageExcitVal As Double, ByVal a As Double, ByVal b As Double, ByVal c As Double, ByVal r1 As Double) As Long
Declare Function ArtDAQ_CreateAIResistanceChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal resistanceConfig As Long, ByVal currentExcitSource As Long, ByVal currentExcitVal As Double, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateAIStrainGageChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal strainConfig As Long, ByVal voltageExcitSource As Long, ByVal voltageExcitVal As Double, ByVal gageFactor As Double, ByVal initialBridgeVoltage As Double, ByVal nominalGageResistance As Double, ByVal poissonRatio, ByVal leadWireResistance As Double, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateAIBridgeChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal bridgeConfig As Long, ByVal voltageExcitSource As Long, ByVal voltageExcitVal As Double, ByVal nominalBridgeResistance As Double, ByRef customScaleName As Byte) As Long

Declare Function ArtDAQ_CreateAOVoltageChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateAOCurrentChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef physicalChannel As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByRef customScaleName As Byte) As Long

Declare Function ArtDAQ_CreateDIChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef lines As Byte, ByRef nameToAssignToLines As Byte, ByVal lineGrouping As Long) As Long
Declare Function ArtDAQ_CreateDOChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef lines As Byte, ByRef nameToAssignToLines As Byte, ByVal lineGrouping As Long) As Long
Declare Function ArtDAQ_CreateCIFreqChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal edge As Long, ByVal measMethod As Long, ByVal measTime As Double, ByVal divisor As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateCIPeriodChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal edge As Long, ByVal measMethod As Long, ByVal measTime As Double, ByVal divisor As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateCICountEdgesChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal edge As Long, ByVal initialCount As Long, ByVal countDirection As Long) As Long
Declare Function ArtDAQ_CreateCIPulseWidthChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal startingEdge As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateCISemiPeriodChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateCITwoEdgeSepChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long, ByVal firstEdge As Long, ByVal secondEdge As Long, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateCIPulseChanFreq Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long) As Long
Declare Function ArtDAQ_CreateCIPulseChanTime Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal minVal As Double, ByVal maxVal As Double, ByVal units As Long) As Long
Declare Function ArtDAQ_CreateCIPulseChanTicks Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByRef sourceTerminal As Byte, ByVal minVal As Double, ByVal maxVal As Double) As Long
Declare Function ArtDAQ_CreateCILinEncoderChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal decodingType As Long, ByVal ZidxEnable As Long, ByVal ZidxVal As Double, ByVal ZidxPhase As Long, ByVal units As Long, ByVal distPerPulse As Double, ByVal initialPos As Double, ByRef customScaleName As Byte) As Long
Declare Function ArtDAQ_CreateCIAngEncoderChan Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal decodingType As Long, ByVal ZidxEnable As Long, ByVal ZidxVal As Double, ByVal ZidxPhase As Long, ByVal units As Long, ByVal pulsesPerRev As Long, ByVal initialAngle As Double, ByRef customScaleName As Byte) As Long

Declare Function ArtDAQ_CreateCOPulseChanFreq Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal units As Long, ByVal idleState As Long, ByVal initialDelay As Double, ByVal freq As Double, ByVal dutyCycle As Double) As Long
Declare Function ArtDAQ_CreateCOPulseChanTime Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByVal units As Long, ByVal idleState As Long, ByVal initialDelay As Double, ByVal lowTime As Double, ByVal highTime As Double) As Long
Declare Function ArtDAQ_CreateCOPulseChanTicks Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef counter As Byte, ByRef nameToAssignToChannel As Byte, ByRef sourceTerminal As Byte, ByVal idleState As Long, ByVal initialDelay As Double, ByVal lowTicks As Long, ByVal highTicks As Long) As Long

Declare Function ArtDAQ_GetChanAttribute Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal attributes As Long, ByRef value As Long, ByVal bufferSize As Long) As Long
'******************************************************
'***    Timing      ***
'******************************************************
Declare Function ArtDAQ_CfgSampClkTiming Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal source As Byte, ByVal rate As Double, ByVal activeEdge As Long, ByVal sampleMode As Long, ByVal sampsPerChan As Long) As Long
Declare Function ArtDAQ_CfgChangeDetectionTiming Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef risingEdgeChan As Byte, ByRef fallingEdgeChan As Byte, ByVal sampleMode As Long, ByVal sampsPerChan As Currency) As Long
Declare Function ArtDAQ_CfgImplicitTiming Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal sampleMode As Long, ByVal sampsPerChan As Currency) As Long
Declare Function ArtDAQ_GetTimingAttribute Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal attributes As Long, ByRef value As Any, ByVal bufferSize As Long) As Long
'******************************************************
'***  Triggering    ***
'******************************************************
Declare Function ArtDAQ_DisableStartTrig Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_CfgDigEdgeStartTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerEdge As Long) As Long
Declare Function ArtDAQ_CfgAnlgEdgeStartTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerSlope As Long, ByVal triggerLevel As Double) As Long
Declare Function ArtDAQ_CfgAnlgWindowStartTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerWhen As Long, ByVal windowTop As Double, ByVal windowBottom As Double) As Long

Declare Function ArtDAQ_DisableRefTrig Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_CfgDigEdgeRefTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerEdge As Long, ByVal pretriggerSamples As Long) As Long
Declare Function ArtDAQ_CfgAnlgEdgeRefTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerSlope As Long, ByVal triggerLevel As Double, ByVal pretriggerSamples As Long) As Long
Declare Function ArtDAQ_CfgAnlgWindowRefTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerWhen As Long, ByVal windowTop As Double, ByVal windowBottom As Double, ByVal pretriggerSamples As Long) As Long

Declare Function ArtDAQ_DisablePauseTrig Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_CfgDigLvlPauseTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerWhen As Long) As Long
Declare Function ArtDAQ_CfgAnlgLvlPauseTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerWhen As Long, ByVal triggerLevel As Double) As Long
Declare Function ArtDAQ_CfgAnlgWindowPauseTrig Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef triggerSource As Byte, ByVal triggerWhen As Long, ByVal windowTop As Double, ByVal windowBottom As Double) As Long

Declare Function ArtDAQ_SendSoftwareTrigger Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal triggerID As Long) As Long

Declare Function ArtDAQ_GetTrigAttribute Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal attributes As Long, ByRef value As Long, ByVal bufferSize As Long) As Long
'******************************************************
'*** Read Data      ***
'******************************************************
Declare Function ArtDAQ_ReadAnalogF64 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As Double, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadAnalogScalarF64 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef value As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadBinaryI16 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As Integer, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadBinaryU16 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As UInteger, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadBinaryI32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As Long, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadBinaryU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As ULong, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadDigitalU8 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As Byte, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadDigitalU16 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As UInteger, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadDigitalU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As ULong, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadDigitalScalarU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef value As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadDigitalLines Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByVal fillMode As Long, ByRef readArray As Byte, ByVal arraySizeInBytes As Long, ByRef sampsPerChanRead As Long, ByRef numBytesPerSamp As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadDigitalPort Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal portIndex As Long, ByRef portVal As Long) As Long

Declare Function ArtDAQ_ReadCounterF64 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByRef readArray As Double, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCounterU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByRef readArray As ULong, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCounterScalarF64 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef value As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCounterScalarU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef value As ULong, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCtrFreq Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByRef readArrayFrequency As Double, ByRef readArrayDutyCycle As Double, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCtrTime Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByRef readArrayHighTime As Double, ByRef readArrayLowTime As Double, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCtrTicks Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal timeout As Double, ByRef readArrayHighTicks As Long, ByRef readArrayLowTicks As Long, ByVal arraySizeInSamps As Long, ByRef sampsPerChanRead As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCtrFreqScalar Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef frequency As Double, ByRef dutyCycle As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCtrTimeScalar Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef highTime As Double, ByRef lowTime As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_ReadCtrTicksScalar Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal timeout As Double, ByRef highTicks As Long, ByRef lowTicks As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_GetReadAttribute Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal attributes As Long, ByRef value As Long, ByVal bufferSize As Long) As Long
'******************************************************
'*** Write Data     ***
'******************************************************
Declare Function ArtDAQ_WriteAnalogF64 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As Double, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteAnalogScalarF64 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal value As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteBinaryI16 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As Integer, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteBinaryU16 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As UInteger, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long

Declare Function ArtDAQ_WriteDigitalU8 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As Byte, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteDigitalU16 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As UInteger, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteDigitalU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As ULong, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteDigitalScalarU32 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal value As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteDigitalLines Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal dataLayout As Long, ByRef writeArray As Byte, ByRef sampsPerChanWritten As Long, ByRef reserved As Long) As Long

Declare Function ArtDAQ_WriteCtrFreq Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByRef frequency As Double, ByRef dutyCycle As Double, ByRef numSampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteCtrTime Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByRef highTime As Double, ByRef lowTime As Double, ByRef numSampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteCtrTicks Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long, ByVal autoStart As Long, ByVal timeout As Double, ByRef highTicks As Long, ByRef lowTicks As Long, ByRef numSampsPerChanWritten As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteCtrFreqScalar Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal frequency As Double, ByVal dutyCycle As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteCtrTimeScalar Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal highTime As Double, ByVal lowTime As Double, ByRef reserved As Long) As Long
Declare Function ArtDAQ_WriteCtrTicksScalar Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal autoStart As Long, ByVal timeout As Double, ByVal highTicks As Double, ByVal lowTicks As Double, ByRef reserved As Long) As Long

Declare Function ArtDAQ_WritePositionComparsionData Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSamps As Long, ByRef positionArray As Long, ByRef pulseWidthArray As Long, ByRef sampsWritten As Long, ByRef reserved As Boolean) As Long
Declare Function ArtDAQ_GetWriteAttribute Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal attributes As Long, ByRef value As Any, ByVal bufferSize As Long) As Long
'******************************************************
'***       Events & Signals ***
'******************************************************
Declare Function ArtDAQ_ExportSignal Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal signalID As Long, ByVal outputTerminal As Byte) As Long
Declare Function ArtDAQ_ExportCtrOutEvent Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef outputTerminal As Byte, ByVal outputBehavior As Long, ByVal pulsePolarity As Long, ByVal toggleIdleState As Long) As Long
Declare Function ArtDAQ_ExportPositionComparsionEvent Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef outputTerminal As Byte, ByVal outputBehavior As Long, ByVal pulsePolarity As Long, ByVal toggleIdleState As Long, ByVal pulseWidthMode As Long) As Long

'******************************************************
'***     Buffer Configurations      ***
'******************************************************
Declare Function ArtDAQ_CfgInputBuffer Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long) As Long
Declare Function ArtDAQ_CfgOutputBuffer Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal numSampsPerChan As Long) As Long

'******************************************************
Declare Function ArtDAQ_GetSystemAttribute Lib "Art_DAQ" (ByVal attributeType As Long, ByRef attributes As Any, ByVal bufferSize As Long) As Long
'***   Device Configuration ***
'******************************************************
Declare Function ArtDAQ_ResetDevice Lib "Art_DAQ" (ByRef deviceName As Byte) As Long
Declare Function ArtDAQ_SelfTestDevice Lib "Art_DAQ" (ByRef deviceName As Byte) As Long
Declare Function ArtDAQ_GetDeviceAttribute Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal attributeType As Long, ByRef attributes As Any, ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_GetDeviceAttributeEx Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal attributeType As Long, ByRef attributes As Any, ByVal bufferSize As Long, ByVal TaskHandle As Long) As Long
Declare Function ArtDAQ_GetDeviceAttributeByType Lib "Art_DAQ" (ByRef deviceType As Byte, ByVal attributeType As Long, ByRef attributes As Any, ByVal bufferSize As Long) As Long

Declare Function ArtDAQ_SetDigitalPowerUpStates Lib "Art_DAQ" (ByRef deviceName As Byte, ByRef channelNames As Byte, ByVal state As Long) As Long
Declare Function ArtDAQ_GetDigitalPowerUpStates Lib "Art_DAQ" (ByRef deviceName As Byte, ByRef channelNames As Byte, ByRef state As Long, ByVal arraySize As Long) As Long
Declare Function ArtDAQ_Set5VPowerOutputStates Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal outputEnable As Long) As Long
Declare Function ArtDAQ_Get5VPowerOutputStates Lib "Art_DAQ" (ByRef deviceName As Byte, ByRef outputEnable As Long, ByRef reserved As Long) As Long
Declare Function ArtDAQ_Set5VPowerPowerUpStates Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal outputEnable As Long) As Long
Declare Function ArtDAQ_Get5VPowerPowerUpStates Lib "Art_DAQ" (ByRef deviceName As Byte, ByRef outputEnable As Long) As Long

'******************************************************
'*** Calibration    ***
'******************************************************
Declare Function ArtDAQ_SelfCal Lib "Art_DAQ" (ByRef deviceName As Byte) As Long

Declare Function ArtDAQ_GetAICalOffsetAndGain Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal channel As ULong, ByVal minVal As Double, ByVal maxVal As Double, ByVal sampClock As Double, ByRef offset As Double, ByRef codeWidth As Double) As Long
Declare Function ArtDAQ_GetAOCalOffsetAndGain Lib "Art_DAQ" (ByRef deviceName As Byte, ByVal channel As ULong, ByVal minVal As Double, ByVal maxVal As Double, ByVal sampClock As Double, ByRef offset As Double, ByRef codeWidth As Double) As Long
Declare Function ArtDAQ_PerformBridgeOffsetNullingCal Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long
Declare Function ArtDAQ_PerformStrainShuntCal Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal shuntResistorValue As Double, ByVal shuntResistorLocation As Long, ByVal skipUnsupportedChannels As ULong) As Long
Declare Function ArtDAQ_PerformBridgeShuntCal Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal shuntResistorValue As Double, ByVal shuntResistorLocation As Long, ByVal bridgeResistance As Double, ByVal skipUnsupportedChannels As ULong) As Long
'******************************************************
'*** Error Handling ***
'******************************************************
Declare Function ArtDAQ_GetErrorString Lib "Art_DAQ" (ByVal errorCode As Long, ByRef errorString As Byte, ByVal bufferSize As Long) As Long
Declare Function ArtDAQ_GetExtendedErrorInfo Lib "Art_DAQ" (ByRef errorString As Byte, ByVal bufferSize As Long) As Long

'******************************************************************************
'*** ART-DAQ Specific Attribute Get/Set/Reset Function Declarations ***********
'******************************************************************************/
'*** Set functions for Run Mode ***
' Uses value set RunMode
Declare Function ArtDAQ_SetRunMode Lib "Art_DAQ" (ByVal mode As Long) As Long

'********** Timing **********
'*** Set/Get functions for ArtDAQ_SampTimingType ***
' Uses value set SampleTimingType
Declare Function ArtDAQ_GetSampTimingType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Long) As Long
Declare Function ArtDAQ_SetSampTimingType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Long) As Long
Declare Function ArtDAQ_ResetSampTimingType Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long

'*** Set functions for ArtDAQ_AIConv_Src ***
Declare Function ArtDAQ_SetAIConvClk Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef source As Byte, ByVal activeEdge As Long) As Long

'*** Set functions for ArtDAQ_SampClk_Timebase_Src ***
Declare Function ArtDAQ_SetSampClkTimebaseSrc Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Byte) As Long

'*** Set functions for ArtDAQ_Exported_SampClkTimebase_OutputTerm ***
Declare Function ArtDAQ_SetExportedSampClkTimebaseOutputTerm Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Byte) As Long

'*** Set functions for ArtDAQ_RefClk_Src ***
Declare Function ArtDAQ_SetRefClkSrc Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Byte) As Long

'*** Set functions for ArtDAQ_SyncPulse_Src ***
Declare Function ArtDAQ_SetSyncPulseSrc Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Byte) As Long

'*** Set functions for ArtDAQ_Exported_SyncPulseEvent_OutputTerm ***
Declare Function ArtDAQ_SetExportedSyncPulseEventOutputTerm Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Byte) As Long

'********** Trigger **********
'*** Set/Get functions for ArtDAQ_AnlgEdge_StartTrig_Hyst ***
Declare Function ArtDAQ_GetAnlgEdgeStartTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAnlgEdgeStartTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAnlgEdgeStartTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long

'*** Set/Get functions for ArtDAQ_AnlgEdge_RefTrig_Hyst ***
Declare Function ArtDAQ_GetAnlgEdgeRefTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAnlgEdgeRefTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAnlgEdgeRefTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long

'*** Set/Get functions for ArtDAQ_AnlgLvl_PauseTrig_Hyst ***
Declare Function ArtDAQ_GetAnlgLvlPauseTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAnlgLvlPauseTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAnlgLvlPauseTrigHyst Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long

'*** Set/Get functions for ArtDAQ_StartTrig_Delay ***
Declare Function ArtDAQ_GetStartTrigDelay Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetStartTrigDelay Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long

'*** Set/Get functions for ArtDAQ_StartTrig_DelayUnits ***
' Uses value set DigitalWidthUnits1
Declare Function ArtDAQ_GetStartTrigDelayUnits Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Long) As Long
Declare Function ArtDAQ_SetStartTrigDelayUnits Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Long) As Long

'*** Set/Get functions for ArtDAQ_StartTrig_DigFltr_MinPulseWidth ***
Declare Function ArtDAQ_GetStartTrigDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetStartTrigDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long

'*** Set/Get functions for ArtDAQ_StartTrig_Retriggerable ***
Declare Function ArtDAQ_GetStartTrigRetriggerable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Long) As Long
Declare Function ArtDAQ_SetStartTrigRetriggerable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Long) As Long

'*** Set/Get functions for ArtDAQ_RefTrig_DigFltr_MinPulseWidth ***
Declare Function ArtDAQ_GetRefTrigDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetRefTrigDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long

'*** Set/Get functions for ArtDAQ_PauseTrig_DigFltr_MinPulseWidth ***
Declare Function ArtDAQ_GetPauseTrigDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Double) As Long
Declare Function ArtDAQ_SetPauseTrigDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Double) As Long

'********** Read **********
'*** Set/Get functions for ArtDAQ_Read_OverWrite ***
' Uses value set OverwriteMode1
Declare Function ArtDAQ_GetReadOverWrite Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Long) As Long
Declare Function ArtDAQ_SetReadOverWrite Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Long) As Long

'*** Set/Get functions for ArtDAQ_Read_AutoStart ***
Declare Function ArtDAQ_GetReadAutoStart Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Long) As Long
Declare Function ArtDAQ_SetReadAutoStart Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Long) As Long

'********** Write **********
'*** Set/Get functions for ArtDAQ_Write_RegenMode ***
' Uses value set RegenerationMode1
Declare Function ArtDAQ_GetWriteRegenMode Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef data As Long) As Long
Declare Function ArtDAQ_SetWriteRegenMode Lib "Art_DAQ" (ByVal TaskHandle As Long, ByVal data As Long) As Long

' AI
'*** Set/Get functions for ArtDAQ_AI_Max ***
Declare Function ArtDAQ_GetAIMax Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIMax Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIMax Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_Min ***
Declare Function ArtDAQ_GetAIMin Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIMin Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIMin Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_CustomScaleName ***
Declare Function ArtDAQ_GetAICustomScaleName Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Byte, ByVal bufferSize As Long) As Long
Declare Function ArtDAQ_SetAICustomScaleName Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Byte) As Long
Declare Function ArtDAQ_ResetAICustomScaleName Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_MeasType ***
' Uses value set AIMeasurementType
Declare Function ArtDAQ_GetAIMeasType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long
'*** Set/Get functions for ArtDAQ_AI_TermCfg ***
' Uses value set InputTermCfg
Declare Function ArtDAQ_GetAITermCfg Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long
Declare Function ArtDAQ_SetAITermCfg Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Long) As Long
Declare Function ArtDAQ_ResetAITermCfg Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_InputSrc ***
Declare Function ArtDAQ_GetAIInputSrc Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef device As Byte, ByRef data As Byte, ByVal bufferSize As ULong) As Long
Declare Function ArtDAQ_SetAIInputSrc Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef device As Byte, ByRef data As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_AutoZeroMode ***
' Uses value set AutoZeroType1
Declare Function ArtDAQ_GetAIAutoZeroMode Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long
Declare Function ArtDAQ_SetAIAutoZeroMode Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Long) As Long
Declare Function ArtDAQ_ResetAIAutoZeroMode Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_OpenThrmcplDetectEnable ***
Declare Function ArtDAQ_GetAIOpenThrmcplDetectEnable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As ULong) As Long
Declare Function ArtDAQ_SetAIOpenThrmcplDetectEnable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As ULong) As Long
Declare Function ArtDAQ_ResetAIOpenThrmcplDetectEnable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long
'*** Set/Get functions for ArtDAQ_AI_RTD_Type ***
' Uses value set RTDType1
Declare Function ArtDAQ_GetAIRTDType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long
Declare Function ArtDAQ_SetAIRTDType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Long) As Long
Declare Function ArtDAQ_ResetAIRTDType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_RTD_R0 ***
Declare Function ArtDAQ_GetAIRTDR0 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIRTDR0 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIRTDR0 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_RTD_A ***
Declare Function ArtDAQ_GetAIRTDA Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIRTDA Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIRTDA Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_RTD_B ***
Declare Function ArtDAQ_GetAIRTDB Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIRTDB Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIRTDB Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_RTD_C ***
Declare Function ArtDAQ_GetAIRTDC Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIRTDC Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIRTDC Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_Thrmstr_A ***
Declare Function ArtDAQ_GetAIThrmstrA Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIThrmstrA Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIThrmstrA Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_Thrmstr_B ***
Declare Function ArtDAQ_GetAIThrmstrB Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIThrmstrB Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIThrmstrB Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_Thrmstr_C ***
Declare Function ArtDAQ_GetAIThrmstrC Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIThrmstrC Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIThrmstrC Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_Thrmstr_R1 ***
Declare Function ArtDAQ_GetAIThrmstrR1 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAIThrmstrR1 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAIThrmstrR1 Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AI_Bridge_ShuntCal_Enable ***
Declare Function ArtDAQ_GetAIBridgeShuntCalEnable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As ULong) As Long
Declare Function ArtDAQ_SetAIBridgeShuntCalEnable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As ULong) As Long
Declare Function ArtDAQ_ResetAIBridgeShuntCalEnable Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long
' AO
'*** Set/Get functions for ArtDAQ_AO_Max ***
Declare Function ArtDAQ_GetAOMax Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAOMax Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAOMax Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AO_Min ***
Declare Function ArtDAQ_GetAOMin Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetAOMin Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetAOMin Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_AO_CustomScaleName ***
Declare Function ArtDAQ_GetAOCustomScaleName Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Byte, ByVal bufferSize As ULong) As Long
Declare Function ArtDAQ_SetAOCustomScaleName Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Byte) As Long
Declare Function ArtDAQ_ResetAOCustomScaleName Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long
'*** Set/Get functions for ArtDAQ_AO_OutputType ***
' Uses value set AOOutputChannelType
Declare Function ArtDAQ_GetAOOutputType Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long
'********** CTR **********
'*** Set/Get functions for CI CountEdges CountReset ***
Declare Function ArtDAQ_CfgCICountEdgesCountReset Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef sourceTerminal As Byte, ByVal resetCount As Long, ByVal activeEdge As Long, ByVal digFltrMinPulseWidth As Double) As Long
Declare Function ArtDAQ_DisableCICountEdgesCountReset Lib "Art_DAQ" (ByVal TaskHandle As Long) As Long

'*** Set/Get functions for ArtDAQ_CI_Source_DigFltr_MinPulseWidth ***
Declare Function ArtDAQ_GetCISourceDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetCISourceDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetCISourceDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CI_Gate_DigFltr_MinPulseWidth ***
Declare Function ArtDAQ_GetCIGateDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetCIGateDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetCIGateDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CI_Aux_DigFltr_MinPulseWidth ***
Declare Function ArtDAQ_GetCIAuxDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetCIAuxDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Double) As Long
Declare Function ArtDAQ_ResetCIAuxDigFltrMinPulseWidth Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CI_Encoder_AInputInvert ***
Declare Function ArtDAQ_GetCIEncoderAInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetCIEncoderAInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As ULong) As Long
Declare Function ArtDAQ_ResetCIEncoderAInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CI_Encoder_BInputInvert ***
Declare Function ArtDAQ_GetCIEncoderBInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetCIEncoderBInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As ULong) As Long
Declare Function ArtDAQ_ResetCIEncoderBInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CI_Encoder_ZInputInvert ***
Declare Function ArtDAQ_GetCIEncoderZInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Double) As Long
Declare Function ArtDAQ_SetCIEncoderZInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As ULong) As Long
Declare Function ArtDAQ_ResetCIEncoderZInputInvert Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CO_Pulse_Term ***
Declare Function ArtDAQ_GetCOPulseTerm Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Byte, ByVal bufferSize As Long) As Long
Declare Function ArtDAQ_SetCOPulseTerm Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Byte) As Long
Declare Function ArtDAQ_ResetCOPulseTerm Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte) As Long

'*** Set/Get functions for ArtDAQ_CO_Count ***
Declare Function ArtDAQ_GetCOCount Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long

'*** Set/Get functions for ArtDAQ_CO_OutputState ***
' Uses value set Level1
Declare Function ArtDAQ_GetCOOutputState Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long

'*** Set/Get functions for ArtDAQ_CO_EnableInitialDelayOnRetrigger ***
Declare Function ArtDAQ_GetCOEnableInitialDelayOnRetrigger Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByRef data As Long) As Long
Declare Function ArtDAQ_SetCOEnableInitialDelayOnRetrigger Lib "Art_DAQ" (ByVal TaskHandle As Long, ByRef channel As Byte, ByVal data As Long) As Long


