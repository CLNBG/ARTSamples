unit Art_DAQ;

interface
uses Windows;

// 自定义数据类型
type
UINT64 = record
  low32 : Longword;
  hig32 : Longword;
end;

//********** System Attributes **********
const ArtDAQ_Sys_Tasks                                      : Longint = $1267;
const ArtDAQ_Sys_DevNames                                   : Longint = $193B;
const ArtDAQ_Sys_MajorVersion                               : Longint = $1272;
const ArtDAQ_Sys_MinorVersion                               : Longint = $1923;
const ArtDAQ_Sys_UpdateVersion                              : Longint = $2F22;

//********** Task Attributes **********
const ArtDAQ_Task_Name                                      : Longint = $1276;
const ArtDAQ_Task_Channels                                  : Longint = $1273;
const ArtDAQ_Task_NumChans                                  : Longint = $2181;
const ArtDAQ_Task_Devices                                   : Longint = $230E;
const ArtDAQ_Task_NumDevices                                : Longint = $29BA;
const ArtDAQ_Task_Complete                                  : Longint = $1274;

//********** Device Attributes **********
const ArtDAQ_Dev_ProductType                                : Longint = $0631;
const ArtDAQ_Dev_ProductNum                                 : Longint = $231D;
const ArtDAQ_Dev_SerialNum                                  : Longint = $0632;
const ArtDAQ_Dev_AI_PhysicalChans                           : Longint = $231E;
const ArtDAQ_Dev_AI_SupportedMeasTypes                      : Longint = $2FD2;
const ArtDAQ_Dev_AI_MaxSingleChanRate                       : Longint = $298C;
const ArtDAQ_Dev_AI_MaxMultiChanRate                        : Longint = $298D;
const ArtDAQ_Dev_AI_MinRate                                 : Longint = $298E;
const ArtDAQ_Dev_AI_SampModes                               : Longint = $2FDC;
const ArtDAQ_Dev_AI_TrigUsage                               : Longint = $2986;
const ArtDAQ_Dev_AI_VoltageRngs                             : Longint = $2990;
const ArtDAQ_Dev_AO_PhysicalChans                           : Longint = $231F;
const ArtDAQ_Dev_AO_SupportedOutputTypes                    : Longint = $2FD3;
const ArtDAQ_Dev_AO_SampModes                               : Longint = $2FDD;
const ArtDAQ_Dev_AO_MaxRate                                 : Longint = $2997;
const ArtDAQ_Dev_AO_MinRate                                 : Longint = $2998;
const ArtDAQ_Dev_AO_TrigUsage                               : Longint = $2987;
const ArtDAQ_Dev_AO_VoltageRngs                             : Longint = $299B;
const ArtDAQ_Dev_AO_CurrentRngs                             : Longint = $299C;
const ArtDAQ_Dev_AO_Gains                                   : Longint = $299D;
const ArtDAQ_Dev_DI_Lines                                   : Longint = $2320;
const ArtDAQ_Dev_DI_Ports                                   : Longint = $2321;
const ArtDAQ_Dev_DI_MaxRate                                 : Longint = $2999;
const ArtDAQ_Dev_DI_TrigUsage                               : Longint = $2988;
const ArtDAQ_Dev_DO_Lines                                   : Longint = $2322;
const ArtDAQ_Dev_DO_Ports                                   : Longint = $2323;
const ArtDAQ_Dev_DO_MaxRate                                 : Longint = $299A;
const ArtDAQ_Dev_DO_TrigUsage                               : Longint = $2989;
const ArtDAQ_Dev_CI_PhysicalChans                           : Longint = $2324;
const ArtDAQ_Dev_CI_SupportedMeasTypes                      : Longint = $2FD4;
const ArtDAQ_Dev_CI_TrigUsage                               : Longint = $298A;
const ArtDAQ_Dev_CI_SampModes                               : Longint = $2FDE;
const ArtDAQ_Dev_CI_MaxSize                                 : Longint = $299F;
const ArtDAQ_Dev_CI_MaxTimebase                             : Longint = $29A0;
const ArtDAQ_Dev_CO_PhysicalChans                           : Longint = $2325;
const ArtDAQ_Dev_CO_SupportedOutputTypes                    : Longint = $2FD5;
const ArtDAQ_Dev_CO_SampModes                               : Longint = $2FDF;
const ArtDAQ_Dev_CO_TrigUsage                               : Longint = $298B;
const ArtDAQ_Dev_CO_MaxSize                                 : Longint = $29A1;
const ArtDAQ_Dev_CO_MaxTimebase                             : Longint = $29A2;

//********** Physical Channel Attributes **********
const ArtDAQ_PhysicalChan_AI_InputSrcs                      : Longint = $2FD8;

//********** Channel Attributes **********
const ArtDAQ_AI_Max                                         : Longint = $17DD;
const ArtDAQ_AI_Min                                         : Longint = $17DE;
const ArtDAQ_AI_MeasType                                    : Longint = $0695;
const ArtDAQ_AI_TermCfg                                     : Longint = $1097;
const ArtDAQ_AI_InputSrc                                    : Longint = $2198;
const ArtDAQ_AI_Voltage_Units                               : Longint = $1094;
const ArtDAQ_AI_Current_Units                               : Longint = $0701;
const ArtDAQ_AI_Strain_Units                                : Longint = $0981;
const ArtDAQ_AI_Excit_Src                                   : Longint = $17F4;
const ArtDAQ_AI_Excit_Val                                   : Longint = $17F5;
const ArtDAQ_AI_Coupling                                    : Longint = $0064;
const ArtDAQ_AO_Max                                         : Longint = $1186;
const ArtDAQ_AO_Min                                         : Longint = $1187;
const ArtDAQ_AO_OutputType                                  : Longint = $1108;
const ArtDAQ_AO_Voltage_Units                               : Longint = $1184;
const ArtDAQ_AO_Current_Units                               : Longint = $1109;
const ArtDAQ_DI_NumLines                                    : Longint = $2178;
const ArtDAQ_DO_NumLines                                    : Longint = $2179;
const ArtDAQ_CI_Max                                         : Longint = $189C;
const ArtDAQ_CI_Min                                         : Longint = $189D;
const ArtDAQ_CI_MeasType                                    : Longint = $18A0;
const ArtDAQ_CI_Freq_Units                                  : Longint = $18A1;
const ArtDAQ_CI_Freq_StartingEdge                           : Longint = $0799;
const ArtDAQ_CI_Freq_MeasMeth                               : Longint = $0144;
const ArtDAQ_CI_Freq_MeasTime                               : Longint = $0145;
const ArtDAQ_CI_Freq_Div                                    : Longint = $0147;
const ArtDAQ_CI_Period_Units                                : Longint = $18A3;
const ArtDAQ_CI_Period_StartingEdge                         : Longint = $0852;
const ArtDAQ_CI_Period_MeasMeth                             : Longint = $192C;
const ArtDAQ_CI_Period_MeasTime                             : Longint = $192D;
const ArtDAQ_CI_Period_Div                                  : Longint = $192E;
const ArtDAQ_CI_CountEdges_InitialCnt                       : Longint = $0698;
const ArtDAQ_CI_CountEdges_ActiveEdge                       : Longint = $0697;
const ArtDAQ_CI_CountEdges_Term                             : Longint = $18C7;
const ArtDAQ_CI_CountEdges_Dir                              : Longint = $0696;
const ArtDAQ_CI_CountEdges_DirTerm                          : Longint = $21E1;
const ArtDAQ_CI_CountEdges_CountReset_Enable                : Longint = $2FAF;
const ArtDAQ_CI_CountEdges_CountReset_ResetCount            : Longint = $2FB0;
const ArtDAQ_CI_CountEdges_CountReset_Term                  : Longint = $2FB1;
const ArtDAQ_CI_CountEdges_CountReset_DigFltr_MinPulseWidth : Longint = $2FB4;
const ArtDAQ_CI_CountEdges_CountReset_ActiveEdge            : Longint = $2FB2;
const ArtDAQ_CI_PulseWidth_Units                            : Longint = $0823;
const ArtDAQ_CI_PulseWidth_Term                             : Longint = $18AA;
const ArtDAQ_CI_PulseWidth_StartingEdge                     : Longint = $0825;
const ArtDAQ_CI_DutyCycle_Term                              : Longint = $308D;
const ArtDAQ_CI_DutyCycle_StartingEdge                      : Longint = $3092;
const ArtDAQ_CI_SemiPeriod_Units                            : Longint = $18AF;
const ArtDAQ_CI_SemiPeriod_Term                             : Longint = $18B0;
const ArtDAQ_CI_SemiPeriod_StartingEdge                     : Longint = $22FE;
const ArtDAQ_CI_TwoEdgeSep_Units                            : Longint = $18AC;
const ArtDAQ_CI_TwoEdgeSep_FirstTerm                        : Longint = $18AD;
const ArtDAQ_CI_TwoEdgeSep_FirstEdge                        : Longint = $0833;
const ArtDAQ_CI_TwoEdgeSep_SecondTerm                       : Longint = $18AE;
const ArtDAQ_CI_TwoEdgeSep_SecondEdge                       : Longint = $0834;
const ArtDAQ_CI_Pulse_Freq_Units                            : Longint = $2F0B;
const ArtDAQ_CI_Pulse_Freq_Term                             : Longint = $2F04;
const ArtDAQ_CI_Pulse_Freq_Start_Edge                       : Longint = $2F05;
const ArtDAQ_CI_Pulse_Time_Units                            : Longint = $2F13;
const ArtDAQ_CI_Pulse_Time_Term                             : Longint = $2F0C;
const ArtDAQ_CI_Pulse_Time_StartEdge                        : Longint = $2F0D;
const ArtDAQ_CI_Pulse_Ticks_Term                            : Longint = $2F14;
const ArtDAQ_CI_Pulse_Ticks_StartEdge                       : Longint = $2F15;
const ArtDAQ_CI_AngEncoder_Units                            : Longint = $18A6;
const ArtDAQ_CI_AngEncoder_PulsesPerRev                     : Longint = $0875;
const ArtDAQ_CI_AngEncoder_InitialAngle                     : Longint = $0881;
const ArtDAQ_CI_LinEncoder_Units                            : Longint = $18A9;
const ArtDAQ_CI_LinEncoder_DistPerPulse                     : Longint = $0911;
const ArtDAQ_CI_LinEncoder_InitialPos                       : Longint = $0915;
const ArtDAQ_CI_Encoder_DecodingType                        : Longint = $21E6;
const ArtDAQ_CI_Encoder_AInputTerm                          : Longint = $219D;
const ArtDAQ_CI_Encoder_BInputTerm                          : Longint = $219E;
const ArtDAQ_CI_Encoder_ZInputTerm                          : Longint = $219F;
const ArtDAQ_CI_Encoder_ZIndexEnable                        : Longint = $0890;
const ArtDAQ_CI_Encoder_ZIndexVal                           : Longint = $0888;
const ArtDAQ_CI_Encoder_ZIndexPhase                         : Longint = $0889;
const ArtDAQ_CI_CtrTimebaseSrc                              : Longint = $0143;
const ArtDAQ_CI_CtrTimebaseRate                             : Longint = $18B2;
const ArtDAQ_CI_CtrTimebaseActiveEdge                       : Longint = $0142;
const ArtDAQ_CO_OutputType                                  : Longint = $18B5;
const ArtDAQ_CO_Pulse_IdleState                             : Longint = $1170;
const ArtDAQ_CO_Pulse_Term                                  : Longint = $18E1;
const ArtDAQ_CO_Pulse_Time_Units                            : Longint = $18D6;
const ArtDAQ_CO_Pulse_HighTime                              : Longint = $18BA;
const ArtDAQ_CO_Pulse_LowTime                               : Longint = $18BB;
const ArtDAQ_CO_Pulse_Time_InitialDelay                     : Longint = $18BC;
const ArtDAQ_CO_Pulse_DutyCyc                               : Longint = $1176;
const ArtDAQ_CO_Pulse_Freq_Units                            : Longint = $18D5;
const ArtDAQ_CO_Pulse_Freq                                  : Longint = $1178;
const ArtDAQ_CO_Pulse_Freq_InitialDelay                     : Longint = $0299;
const ArtDAQ_CO_Pulse_HighTicks                             : Longint = $1169;
const ArtDAQ_CO_Pulse_LowTicks                              : Longint = $1171;
const ArtDAQ_CO_Pulse_Ticks_InitialDelay                    : Longint = $0298;
const ArtDAQ_CO_CtrTimebaseSrc                              : Longint = $0339;
const ArtDAQ_CO_CtrTimebaseRate                             : Longint = $18C2;
const ArtDAQ_CO_CtrTimebaseActiveEdge                       : Longint = $0341;
const ArtDAQ_CO_Count                                       : Longint = $0293;
const ArtDAQ_CO_OutputState                                 : Longint = $0294;
const ArtDAQ_CO_EnableInitialDelayOnRetrigger               : Longint = $2EC9;
const ArtDAQ_ChanType                                       : Longint = $187F;
const ArtDAQ_PhysicalChanName                               : Longint = $18F5;
const ArtDAQ_ChanDescr                                      : Longint = $1926;
const ArtDAQ_ChanIsGlobal                                   : Longint = $2304;

//********** Read Attributes **********
const ArtDAQ_Read_AutoStart                                 : Longint = $1826;
const ArtDAQ_Read_OverWrite                                 : Longint = $1211;

//********** Timing Attributes **********
const ArtDAQ_Sample_Mode                                    : Longint = $1300;
const ArtDAQ_Sample_SampPerChan                             : Longint = $1310;
const ArtDAQ_SampTimingType                                 : Longint = $1347;
const ArtDAQ_SampClk_Rate                                   : Longint = $1344;
const ArtDAQ_SampClk_MaxRate                                : Longint = $22C8;
const ArtDAQ_SampClk_Src                                    : Longint = $1852;
const ArtDAQ_SampClk_ActiveEdge                             : Longint = $1301;
const ArtDAQ_SampClk_Timebase_Src                           : Longint = $1308;
const ArtDAQ_AIConv_Src                                     : Longint = $1502;
const ArtDAQ_RefClk_Src                                     : Longint = $1316;
const ArtDAQ_SyncPulse_Src                                  : Longint = $223D;

//********** Trigger Attributes **********
const ArtDAQ_StartTrig_Type                                 : Longint = $1393;
const ArtDAQ_StartTrig_Term                                 : Longint = $2F1E;
const ArtDAQ_DigEdge_StartTrig_Src                          : Longint = $1407;
const ArtDAQ_DigEdge_StartTrig_Edge                         : Longint = $1404;
const ArtDAQ_AnlgEdge_StartTrig_Src                         : Longint = $1398;
const ArtDAQ_AnlgEdge_StartTrig_Slope                       : Longint = $1397;
const ArtDAQ_AnlgEdge_StartTrig_Lvl                         : Longint = $1396;
const ArtDAQ_AnlgWin_StartTrig_Src                          : Longint = $1400;
const ArtDAQ_AnlgWin_StartTrig_When                         : Longint = $1401;
const ArtDAQ_AnlgWin_StartTrig_Top                          : Longint = $1403;
const ArtDAQ_AnlgWin_StartTrig_Btm                          : Longint = $1402;
const ArtDAQ_StartTrig_Delay                                : Longint = $1856;
const ArtDAQ_StartTrig_DelayUnits                           : Longint = $18C8;
const ArtDAQ_StartTrig_DigFltr_MinPulseWidth                : Longint = $2224;
const ArtDAQ_StartTrig_Retriggerable                        : Longint = $190F;
const ArtDAQ_RefTrig_Type                                   : Longint = $1419;
const ArtDAQ_RefTrig_PretrigSamples                         : Longint = $1445;
const ArtDAQ_RefTrig_Term                                   : Longint = $2F1F;
const ArtDAQ_DigEdge_RefTrig_Src                            : Longint = $1434;
const ArtDAQ_DigEdge_RefTrig_Edge                           : Longint = $1430;
const ArtDAQ_AnlgEdge_RefTrig_Src                           : Longint = $1424;
const ArtDAQ_AnlgEdge_RefTrig_Slope                         : Longint = $1423;
const ArtDAQ_AnlgEdge_RefTrig_Lvl                           : Longint = $1422;
const ArtDAQ_AnlgWin_RefTrig_Src                            : Longint = $1426;
const ArtDAQ_AnlgWin_RefTrig_When                           : Longint = $1427;
const ArtDAQ_AnlgWin_RefTrig_Top                            : Longint = $1429;
const ArtDAQ_AnlgWin_RefTrig_Btm                            : Longint = $1428;
const ArtDAQ_RefTrig_AutoTrigEnable                         : Longint = $2EC1;
const ArtDAQ_RefTrig_AutoTriggered                          : Longint = $2EC2;
const ArtDAQ_RefTrig_Delay                                  : Longint = $1483;
const ArtDAQ_RefTrig_DigFltr_MinPulseWidth                  : Longint = $2ED8;
const ArtDAQ_PauseTrig_Type                                 : Longint = $1366;
const ArtDAQ_PauseTrig_Term                                 : Longint = $2F20;
const ArtDAQ_AnlgLvl_PauseTrig_Src                          : Longint = $1370;
const ArtDAQ_AnlgLvl_PauseTrig_When                         : Longint = $1371;
const ArtDAQ_AnlgLvl_PauseTrig_Lvl                          : Longint = $1369;
const ArtDAQ_AnlgWin_PauseTrig_Src                          : Longint = $1373;
const ArtDAQ_AnlgWin_PauseTrig_When                         : Longint = $1374;
const ArtDAQ_AnlgWin_PauseTrig_Top                          : Longint = $1376;
const ArtDAQ_AnlgWin_PauseTrig_Btm                          : Longint = $1375;
const ArtDAQ_DigLvl_PauseTrig_Src                           : Longint = $1379;
const ArtDAQ_DigLvl_PauseTrig_When                          : Longint = $1380;
const ArtDAQ_PauseTrig_DigFltr_MinPulseWidth                : Longint = $2229;

//********** Export Signal Attributes **********
const ArtDAQ_Exported_AIConvClk_OutputTerm                  : Longint = $1687;
const ArtDAQ_Exported_SampClk_OutputTerm                    : Longint = $1663;
const ArtDAQ_Exported_PauseTrig_OutputTerm                  : Longint = $1615;
const ArtDAQ_Exported_RefTrig_OutputTerm                    : Longint = $0590;
const ArtDAQ_Exported_StartTrig_OutputTerm                  : Longint = $0584;
const ArtDAQ_Exported_CtrOutEvent_OutputTerm                : Longint = $1717;
const ArtDAQ_Exported_CtrOutEvent_OutputBehavior            : Longint = $174F;
const ArtDAQ_Exported_CtrOutEvent_Pulse_Polarity            : Longint = $1718;
const ArtDAQ_Exported_CtrOutEvent_Toggle_IdleState          : Longint = $186A;
const ArtDAQ_Exported_SampClkTimebase_OutputTerm            : Longint = $18F9;
const ArtDAQ_Exported_SyncPulseEvent_OutputTerm             : Longint = $223C;

//******************************************************************************
//*** ArtDAQ Values ************************************************************
//******************************************************************************/
const ArtDAQ_Val_Cfg_Default                                : Longint = -1;

//*** Values for the Mode parameter of ArtDAQ_TaskControl ***
const ArtDAQ_Val_Task_Start                                 : Longint = 0;
const ArtDAQ_Val_Task_Stop                                  : Longint = 1;
const ArtDAQ_Val_Task_Verify                                : Longint = 2;
const ArtDAQ_Val_Task_Commit                                : Longint = 3;
const ArtDAQ_Val_Task_Reserve                               : Longint = 4;
const ArtDAQ_Val_Task_Unreserve                             : Longint = 5;
const ArtDAQ_Val_Task_Abort                                 : Longint = 6;

//*** Values for ArtDAQ_AI_MeasType ***
//*** Values for ArtDAQ_Dev_AI_SupportedMeasTypes ***
//*** Values for ArtDAQ_PhysicalChan_AI_SupportedMeasTypes ***
//*** Value set AIMeasurementType ***
const ArtDAQ_Val_Voltage                                    : Longint = 10322;
const ArtDAQ_Val_Current                                    : Longint = 10134;
const ArtDAQ_Val_Resistance                                 : Longint = 10278;
const ArtDAQ_Val_Strain_Gage                                : Longint = 10300;
const ArtDAQ_Val_Voltage_IEPESensor                         : Longint = 15966;

//*** Values for ArtDAQ_AO_OutputType ***
//*** Values for ArtDAQ_Dev_AO_SupportedOutputTypes ***
//*** Values for ArtDAQ_PhysicalChan_AO_SupportedOutputTypes ***
//*** Value set AOOutputChannelType ***
//const ArtDAQ_Val_Voltage                                    : Longint = 10322;
//const ArtDAQ_Val_Current                                    : Longint = 10134;

//*** Values for ArtDAQ_CI_MeasType ***
//*** Values for ArtDAQ_Dev_CI_SupportedMeasTypes ***
//*** Values for ArtDAQ_PhysicalChan_CI_SupportedMeasTypes ***
//*** Value set CIMeasurementType ***
const ArtDAQ_Val_Freq                                       : Longint = 10179;
const ArtDAQ_Val_Period                                     : Longint = 10256;
const ArtDAQ_Val_CountEdges                                 : Longint = 10125;
const ArtDAQ_Val_PulseWidth                                 : Longint = 10359;
const ArtDAQ_Val_SemiPeriod                                 : Longint = 10289;
const ArtDAQ_Val_PulseFrequency                             : Longint = 15864;
const ArtDAQ_Val_PulseTime                                  : Longint = 15865;
const ArtDAQ_Val_PulseTicks                                 : Longint = 15866;
const ArtDAQ_Val_DutyCycle                                  : Longint = 16070;
const ArtDAQ_Val_Position_AngEncoder                        : Longint = 10360;
const ArtDAQ_Val_Position_LinEncoder                        : Longint = 10361;
const ArtDAQ_Val_TwoEdgeSep                                 : Longint = 10267;

//*** Values for ArtDAQ_CO_OutputType ***
//*** Values for ArtDAQ_Dev_CO_SupportedOutputTypes ***
//*** Values for ArtDAQ_PhysicalChan_CO_SupportedOutputTypes ***
//*** Value set COOutputType ***
const ArtDAQ_Val_Pulse_Time                                 : Longint = 10269;
const ArtDAQ_Val_Pulse_Freq                                 : Longint = 10119;
const ArtDAQ_Val_Pulse_Ticks                                : Longint = 10268;

//*** Values for ArtDAQ_ChanType ***
//*** Value set ChannelType ***
const ArtDAQ_Val_AI                                         : Longint = 10100;
const ArtDAQ_Val_AO                                         : Longint = 10102;
const ArtDAQ_Val_DI                                         : Longint = 10151;
const ArtDAQ_Val_DO                                         : Longint = 10153;
const ArtDAQ_Val_CI                                         : Longint = 10131;
const ArtDAQ_Val_CO                                         : Longint = 10132;

//*** Values for ArtDAQ_AI_TermCfg ***
//*** Value set InputTermCfg ***
const ArtDAQ_Val_RSE                                        : Longint = 10083;
const ArtDAQ_Val_NRSE                                       : Longint = 10078;
const ArtDAQ_Val_Diff                                       : Longint = 10106;
const ArtDAQ_Val_PseudoDiff                                 : Longint = 12529;

//*** Values for ArtDAQ_AI_Coupling ***
//*** Value set Coupling1 ***
const ArtDAQ_Val_AC                                         : Longint = 10045;
const ArtDAQ_Val_DC                                         : Longint = 10050;
const ArtDAQ_Val_GND                                        : Longint = 10066;

//*** Values for ArtDAQ_AI_Excit_Src ***
//*** Value set ExcitationSource ***
const ArtDAQ_Val_Internal                                   : Longint = 10200;
const ArtDAQ_Val_External                                   : Longint = 10167;
const ArtDAQ_Val_None                                       : Longint = 10230;

//*** Values for ArtDAQ_AI_Voltage_Units ***
//*** Values for ArtDAQ_AO_Voltage_Units ***
//*** Value set VoltageUnits1 ***
const ArtDAQ_Val_Volts                                      : Longint = 10348;

//*** Values for ArtDAQ_AI_Current_Units ***
//*** Values for ArtDAQ_AO_Current_Units ***
//*** Value set CurrentUnits1 ***
const ArtDAQ_Val_Amps                                       : Longint = 10342;

//*** Values for ArtDAQ_CI_Freq_Units ***
//*** Value set FrequencyUnits3 ***
const ArtDAQ_Val_Hz                                         : Longint = 10373;
const ArtDAQ_Val_Ticks                                      : Longint = 10304;

//*** Values for ArtDAQ_CI_Pulse_Freq_Units ***
//*** Values for ArtDAQ_CO_Pulse_Freq_Units ***
//*** Value set FrequencyUnits2 ***
//const ArtDAQ_Val_Hz                                         : Longint = 10373;

//*** Values for ArtDAQ_CI_Period_Units ***
//*** Values for ArtDAQ_CI_PulseWidth_Units ***
//*** Values for ArtDAQ_CI_TwoEdgeSep_Units ***
//*** Values for ArtDAQ_CI_SemiPeriod_Units ***
//*** Value set TimeUnits3 ***
const ArtDAQ_Val_Seconds                                    : Longint = 10364;
//const ArtDAQ_Val_Ticks                                      : Longint = 10304;

//*** Values for ArtDAQ_CI_Pulse_Time_Units ***
//*** Values for ArtDAQ_CO_Pulse_Time_Units ***
//*** Value set TimeUnits2 ***
//const ArtDAQ_Val_Seconds                                    : Longint = 10364;

//*** Values for ArtDAQ_CI_AngEncoder_Units ***
//*** Value set AngleUnits2 ***
const ArtDAQ_Val_Degrees                                    : Longint = 10146;
const ArtDAQ_Val_Radians                                    : Longint = 10273;
//const ArtDAQ_Val_Ticks                                      : Longint = 10304;

//*** Values for ArtDAQ_CI_LinEncoder_Units ***
//*** Value set LengthUnits3 ***
const ArtDAQ_Val_Meters                                     : Longint = 10219;
const ArtDAQ_Val_Inches                                     : Longint = 10379;
//const ArtDAQ_Val_Ticks                                      : Longint = 10304;

//*** Values for ArtDAQ_CI_Freq_MeasMeth ***
//*** Values for ArtDAQ_CI_Period_MeasMeth ***
//*** Value set CounterFrequencyMethod ***
const ArtDAQ_Val_LowFreq1Ctr                                : Longint = 10105;
const ArtDAQ_Val_HighFreq2Ctr                               : Longint = 10157;
const ArtDAQ_Val_LargeRng2Ctr                               : Longint = 10205;

//*** Values for AArtDQ_CI_CountEdges_Dir ***
//*** Value set CountDirection1 ***
const ArtDAQ_Val_CountUp                                    : Longint = 10128;
const ArtDAQ_Val_CountDown                                  : Longint = 10124;
const ArtDAQ_Val_ExtControlled                              : Longint = 10326;

//*** Values for ArtDAQ_CI_Encoder_DecodingType ***
//*** Values for ArtDAQ_CI_Velocity_Encoder_DecodingType ***
//*** Value set EncoderType2 ***
const ArtDAQ_Val_X1                                         : Longint = 10090;
const ArtDAQ_Val_X2                                         : Longint = 10091;
const ArtDAQ_Val_X4                                         : Longint = 10092;
const ArtDAQ_Val_TwoPulseCounting                           : Longint = 10313;

//*** Values for ArtDAQ_CI_Encoder_ZIndexPhase ***
//*** Value set EncoderZIndexPhase1 ***
const ArtDAQ_Val_AHighBHigh                                 : Longint = 10040;
const ArtDAQ_Val_AHighBLow                                  : Longint = 10041;
const ArtDAQ_Val_ALowBHigh                                  : Longint = 10042;
const ArtDAQ_Val_ALowBLow                                   : Longint = 10043;

//*** Values for ArtDAQ_Exported_CtrOutEvent_OutputBehavior ***
//*** Value set ExportActions2 ***
const ArtDAQ_Val_Pulse                                      : Longint = 10265;
const ArtDAQ_Val_Toggle                                     : Longint = 10307;

//*** Values for ArtDAQ_Exported_CtrOutEvent_Pulse_Polarity ***
//*** Value set Polarity2 ***
const ArtDAQ_Val_ActiveHigh                                 : Longint = 10095;
const ArtDAQ_Val_ActiveLow                                  : Longint = 10096;

//*** Values for ArtDAQ_CI_Freq_StartingEdge ***
//*** Values for ArtDAQ_CI_Period_StartingEdge ***
//*** Values for ArtDAQ_CI_CountEdges_ActiveEdge ***
//*** Values for ArtDAQ_CI_CountEdges_CountReset_ActiveEdge ***
//*** Values for ArtDAQ_CI_DutyCycle_StartingEdge ***
//*** Values for ArtDAQ_CI_PulseWidth_StartingEdge ***
//*** Values for ArtDAQ_CI_TwoEdgeSep_FirstEdge ***
//*** Values for ArtDAQ_CI_TwoEdgeSep_SecondEdge ***
//*** Values for ArtDAQ_CI_SemiPeriod_StartingEdge ***
//*** Values for ArtDAQ_CI_Pulse_Freq_Start_Edge ***
//*** Values for ArtDAQ_CI_Pulse_Time_StartEdge ***
//*** Values for ArtDAQ_CI_Pulse_Ticks_StartEdge ***
//*** Values for ArtDAQ_SampClk_ActiveEdge ***
//*** Values for ArtDAQ_DigEdge_StartTrig_Edge ***
//*** Values for ArtDAQ_DigEdge_RefTrig_Edge ***
//*** Values for ArtDAQ_AnlgEdge_StartTrig_Slope ***
//*** Values for ArtDAQ_AnlgEdge_RefTrig_Slope ***
//*** Value set Edge1 ***
const ArtDAQ_Val_Rising                                    : Longint = 10280;
const ArtDAQ_Val_Falling                                   : Longint = 10171;

//*** Values for ArtDAQ_CI_CountEdges_GateWhen ***
//*** Values for ArtDAQ_CI_OutputState ***
//*** Values for ArtDAQ_CO_Pulse_IdleState ***
//*** Values for ArtDAQ_CO_OutputState ***
//*** Values for ArtDAQ_DigLvl_PauseTrig_When ***
//*** Values for ArtDAQ_Exported_CtrOutEvent_Toggle_IdleState ***
//*** Value set Level1 ***
const ArtDAQ_Val_High                                     : Longint = 10192;
const ArtDAQ_Val_Low                                      : Longint = 10214;

//*** Value set for the state parameter of ArtDAQ_SetDigitalPowerUpStates ***
//const ArtDAQ_Val_High                                     : Longint = 10192;
//const ArtDAQ_Val_Low                                      : Longint = 10214;
const ArtDAQ_Val_Input                                    : Longint = 10310;

//*** Value set AcquisitionType ***
const ArtDAQ_Val_FiniteSamps                              : Longint = 10178;
const ArtDAQ_Val_ContSamps                                : Longint = 10123;

//*** Values for the everyNsamplesEventType parameter of ArtDAQ_RegisterEveryNSamplesEvent ***
const ArtDAQ_Val_Acquired_Into_Buffer                     : Longint = 1;
const ArtDAQ_Val_Transferred_From_Buffer                  : Longint = 2;

//*** Value for the Number of Samples per Channel parameter of ArtDAQ_ReadAnalogF64, ArtDAQ_ReadBinaryI16
const ArtDAQ_Val_Auto                                     : Longint = -1;

//*** Values for the Fill Mode parameter of ArtDAQ_Readxxxx ***
const ArtDAQ_Val_GroupByChannel                           : Longint = 0;
const ArtDAQ_Val_GroupByScanNumber                        : Longint = 1;

//*** Values for ArtDAQ_Read_OverWrite ***
//*** Value set OverwriteMode1 ***
const ArtDAQ_Val_OverwriteUnreadSamps                     : Longint = 10252;
const ArtDAQ_Val_DoNotOverwriteUnreadSamps                : Longint = 10159;

//*** Values for ArtDAQ_Write_RegenMode ***
//*** Value set RegenerationMode1 ***
const ArtDAQ_Val_AllowRegen                               : Longint = 10097;
const ArtDAQ_Val_DoNotAllowRegen                          : Longint = 10158;

//*** Values for the Line Grouping parameter of ArtDAQ_CreateDIChan and ArtDAQ_CreateDOChan ***
const ArtDAQ_Val_ChanPerLine                              : Longint = 0;
const ArtDAQ_Val_ChanForAllLines                          : Longint = 1;

//*** Values for ArtDAQ_SampTimingType ***
//*** Value set SampleTimingType ***
const ArtDAQ_Val_SampOnClk                                : Longint = 10388;
const ArtDAQ_Val_Implicit                                 : Longint = 10451;
const ArtDAQ_Val_OnDemand                                 : Longint = 10390;

//*** Value set Signal ***
const ArtDAQ_Val_AIConvertClock                           : Longint = 12484;
const ArtDAQ_Val_SampleClock                              : Longint = 12487;
const ArtDAQ_Val_RefClock                                 : Longint = 12535;
const ArtDAQ_Val_PauseTrigger                             : Longint = 12489;
const ArtDAQ_Val_ReferenceTrigger                         : Longint = 12490;
const ArtDAQ_Val_StartTrigger                             : Longint = 12491;
const ArtDAQ_Val_CounterOutputEvent                       : Longint = 12494;
const ArtDAQ_Val_SampClkTimebase                          : Longint = 12495;
const ArtDAQ_Val_SyncPulseEvent                           : Longint = 12496;

//*** Values for ArtDAQ_PauseTrig_Type ***
//*** Value set TriggerType6 ***
const ArtDAQ_Val_AnlgLvl                                  : Longint = 10101;
const ArtDAQ_Val_AnlgWin                                  : Longint = 10103;
const ArtDAQ_Val_DigLvl                                   : Longint = 10152;
const ArtDAQ_Val_DigPattern                               : Longint = 10398;
//const ArtDAQ_Val_None                                     : Longint = 10230;

//*** Values for ArtDAQ_RefTrig_Type ***
//*** Value set TriggerType8 ***
const ArtDAQ_Val_AnlgEdge                                 : Longint = 10099;
const ArtDAQ_Val_DigEdge                                  : Longint = 10150;
//const ArtDAQ_Val_DigPattern                               : Longint = 10398;
//const ArtDAQ_Val_AnlgWin                                  : Longint = 10103;
//const ArtDAQ_Val_None                                     : Longint = 10230;

//*** Values for ArtDAQ_StartTrig_Type ***
//*** Value set TriggerType10 ***
//const ArtDAQ_Val_AnlgEdge                                 : Longint = 10099;
//const ArtDAQ_Val_DigEdge                                  : Longint = 10150;
//const ArtDAQ_Val_DigPattern                               : Longint = 10398;
//const ArtDAQ_Val_AnlgWin                                  : Longint = 10103;
//const ArtDAQ_Val_None                                     : Longint = 10230;

//*** Values for ArtDAQ_AnlgWin_StartTrig_When ***
//*** Values for ArtDAQ_AnlgWin_RefTrig_When ***
//*** Value set WindowTriggerCondition1 ***
const ArtDAQ_Val_EnteringWin                              : Longint = 10163;
const ArtDAQ_Val_LeavingWin                               : Longint = 10208;

//*** Values for ArtDAQ_AnlgLvl_PauseTrig_When ***
//*** Value set ActiveLevel ***
const ArtDAQ_Val_AboveLvl                                 : Longint = 10093;
const ArtDAQ_Val_BelowLvl                                 : Longint = 10107;

//*** Values for ArtDAQ_AnlgWin_PauseTrig_When ***
//*** Value set WindowTriggerCondition2 ***
const ArtDAQ_Val_InsideWin                                : Longint = 10199;
const ArtDAQ_Val_OutsideWin                               : Longint = 10251;

//*** Values for ArtDAQ_StartTrig_DelayUnits ***
//*** Value set DigitalWidthUnits1 ***
const ArtDAQ_Val_SampClkPeriods                           : Longint = 10286;
//const ArtDAQ_Val_Seconds                                  : Longint = 10364;
//const ArtDAQ_Val_Ticks                                    : Longint = 10304;

//*** Values for Read/Write Data Format
const ArtDAQ_Val_Binary_U32                               : Longint = 1;
const ArtDAQ_Val_Voltage_F64                              : Longint = 2;
const ArtDAQ_Val_CounterDutyCycleAndFrequency_F64         : Longint = 3;
const ArtDAQ_Val_CounterHighAndLowTimes_F64               : Longint = 4;
const ArtDAQ_Val_CounterHighAndLowTicks_U32               : Longint = 5;

//******************************************************************************
//*** ArtDAQ Function Declarations *********************************************
//******************************************************************************/

//******************************************************/
///***         Task Configuration/Control             ***/
///******************************************************/
function ArtDAQ_LoadTask(taskName:PChar;taskHandle:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_LoadTask';

function ArtDAQ_CreateTask(taskName:PChar;taskHandle:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateTask';

function ArtDAQ_StartTask(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_StartTask';

function ArtDAQ_StopTask(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_StopTask';

function ArtDAQ_ClearTask(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ClearTask';

function ArtDAQ_TestTask(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_TestTask';

function ArtDAQ_WaitUntilTaskDone(taskHandle:Integer; timeToWait:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WaitUntilTaskDone';

function ArtDAQ_IsTaskDone(taskHandle:Integer; isTaskDone:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_IsTaskDone';

function ArtDAQ_GetTaskAttribute(taskHandle:Integer; attributeType:integer; attribute:pointer; size:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetTaskAttribute';

//******************** 定义回调函数  ******************************//
type
ArtDAQ_EveryNSamplesEventCallbackPtr=function(taskHandle:integer; everyNsamplesEventType:integer; nSamples:cardinal; callbackData:pointer):integer;stdcall;

type
ArtDAQ_DoneEventCallbackPtr=function(taskHandle:integer; status:integer; callbackData:pointer):integer;stdcall;

type
ArtDAQ_SignalEventCallbackPtr=function(taskHandle:integer; signalID:integer; callbackData:pointer):integer;stdcall;
//****************************************************************//

function ArtDAQ_RegisterEveryNSamplesEvent(task:Integer; everyNsamplesEventType:integer; nSamples:cardinal; options:cardinal; callbackFunction:ArtDAQ_EveryNSamplesEventCallbackPtr; callbackData:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_RegisterEveryNSamplesEvent';

function ArtDAQ_RegisterDoneEvent(task:Integer; options:cardinal; nSamples:cardinal; allbackFunction:ArtDAQ_DoneEventCallbackPtr; callbackData:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_RegisterDoneEvent';

function ArtDAQ_RegisterSignalEvent(task:Integer; signalID:integer; options:cardinal; allbackFunction:ArtDAQ_SignalEventCallbackPtr; callbackData:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_RegisterSignalEvent';

//******************************************************/
//***        Channel Configuration/Creation          ***/
//******************************************************/
function ArtDAQ_CreateAIVoltageChan(taskHandle:Integer; physicalChannel:pchar; nameToAssignToChannel:pchar; terminalConfig:integer; minVal:double; maxVal:double; units:integer; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateAIVoltageChan';
function ArtDAQ_CreateAIVoltageIEPEChan(taskHandle:Integer; physicalChannel:pchar; nameToAssignToChannel:pchar; terminalConfig:integer; coupling:integer; minVal:double; maxVal:double; currentExcitSource:integer; currentExcitVal:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateAIVoltageIEPEChan';

function ArtDAQ_CreateAOVoltageChan(taskHandle:Integer; physicalChannel:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateAOVoltageChan';
function ArtDAQ_CreateAOCurrentChan(taskHandle:Integer; physicalChannel:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateAOCurrentChan';

function ArtDAQ_CreateDIChan(taskHandle:Integer; lines:pchar; nameToAssignToLines:pchar; lineGrouping:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateDIChan';
function ArtDAQ_CreateDOChan(taskHandle:Integer; lines:pchar; nameToAssignToLines:pchar; lineGrouping:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateDOChan';

function ArtDAQ_CreateCIFreqChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; edge:integer; measMethod:integer; measTime:double; divisor:cardinal; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIFreqChan';
function ArtDAQ_CreateCIPeriodChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; edge:integer; measMethod:integer; measTime:double; divisor:cardinal; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIPeriodChan';
function ArtDAQ_CreateCICountEdgesChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; edge:integer; initialCount:cardinal; countDirection:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCICountEdgesChan';
function ArtDAQ_CreateCIPulseWidthChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; startingEdge:integer; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIPulseWidthChan';
function ArtDAQ_CreateCISemiPeriodChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCISemiPeriodChan';
function ArtDAQ_CreateCITwoEdgeSepChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer; firstEdge:integer; secondEdge:integer; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCITwoEdgeSepChan';
function ArtDAQ_CreateCIPulseChanFreq(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIPulseChanFreq';
function ArtDAQ_CreateCIPulseChanTime(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; minVal:double; maxVal:double; units:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIPulseChanTime';
function ArtDAQ_CreateCIPulseChanTicks(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; sourceTerminal:pchar; minVal:double; maxVal:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIPulseChanTicks';
function ArtDAQ_CreateCILinEncoderChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; decodingType:integer; ZidxEnable:cardinal; ZidxVal:double; ZidxPhase:integer; units:integer; distPerPulse:double; initialPos:double; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCILinEncoderChan';
function ArtDAQ_CreateCIAngEncoderChan(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; decodingType:integer; ZidxEnable:cardinal; ZidxVal:double; ZidxPhase:integer; units:integer; pulsesPerRev:cardinal; initialAngle:double; customScaleName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCIAngEncoderChan';

function ArtDAQ_CreateCOPulseChanFreq(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; units:integer; idleState:integer; initialDelay:double; freq:double; dutyCycle:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCOPulseChanFreq';
function ArtDAQ_CreateCOPulseChanTime(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; units:integer; idleState:integer; initialDelay:double; lowTime:double; highTime:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCOPulseChanTime';
function ArtDAQ_CreateCOPulseChanTicks(taskHandle:Integer; counter:pchar; nameToAssignToChannel:pchar; sourceTerminal:pchar; idleState:integer; initialDelay:integer; lowTicks:integer; highTicks:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CreateCOPulseChanTicks';

//******************************************************/
//***                    Timing                      ***/
//******************************************************/
function ArtDAQ_CfgSampClkTiming(taskHandle:Integer; source:pchar; rate:double; activeEdge:integer; sampleMode:integer; sampsPerChan:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgSampClkTiming';
function ArtDAQ_CfgImplicitTiming(taskHandle:Integer; sampleMode:integer; sampsPerChan:UINT64):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgImplicitTiming';

//******************************************************/
//***                  Triggering                    ***/
//******************************************************/
function ArtDAQ_DisableStartTrig(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_DisableStartTrig';
function ArtDAQ_CfgDigEdgeStartTrig(taskHandle:Integer; triggerSource:pchar; triggerEdge:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgDigEdgeStartTrig';
function ArtDAQ_CfgAnlgEdgeStartTrig(taskHandle:Integer; triggerSource:pchar; triggerSlope:integer; triggerLevel:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgAnlgEdgeStartTrig';
function ArtDAQ_CfgAnlgWindowStartTrig(taskHandle:Integer; triggerSource:pchar; triggerWhen:integer; windowTop:double; windowBottom:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgAnlgWindowStartTrig';

function ArtDAQ_DisableRefTrig(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_DisableRefTrig';
function ArtDAQ_CfgDigEdgeRefTrig(taskHandle:Integer; triggerSource:pchar; triggerEdge:integer; pretriggerSamples:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgDigEdgeRefTrig';
function ArtDAQ_CfgAnlgEdgeRefTrig(taskHandle:Integer; triggerSource:pchar; triggerSlope:integer; triggerLevel:double; pretriggerSamples:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgAnlgEdgeRefTrig';
function ArtDAQ_CfgAnlgWindowRefTrig(taskHandle:Integer; triggerSource:pchar; triggerWhen:integer; windowTop:double; windowBottom:double; pretriggerSamples:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgAnlgWindowRefTrig';

function ArtDAQ_DisablePauseTrig(taskHandle:Integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_DisablePauseTrig';
function ArtDAQ_CfgDigLvlPauseTrig(taskHandle:Integer; triggerSource:pchar; triggerWhen:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgDigLvlPauseTrig';
function ArtDAQ_CfgAnlgLvlPauseTrig(taskHandle:Integer; triggerSource:pchar; triggerWhen:integer; triggerLevel:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgAnlgLvlPauseTrig';
function ArtDAQ_CfgAnlgWindowPauseTrig(taskHandle:Integer; triggerSource:pchar; triggerWhen:integer; windowTop:double; windowBottom:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgAnlgWindowPauseTrig';

function ArtDAQ_SendSoftwareTrigger(taskHandle:Integer; triggerID:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SendSoftwareTrigger';

//******************************************************/
//***                 Read Data                      ***/
//******************************************************/
function ArtDAQ_ReadAnalogF64(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadAnalogF64';
function ArtDAQ_ReadAnalogScalarF64(taskHandle:Integer; timeout:double; value:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadAnalogScalarF64';
function ArtDAQ_ReadBinaryI16(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadBinaryI16';
function ArtDAQ_ReadBinaryU16(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadBinaryU16';
function ArtDAQ_ReadBinaryI32(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadBinaryI32';
function ArtDAQ_ReadBinaryU32(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadBinaryU32';

function ArtDAQ_ReadDigitalU8(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadDigitalU8';
function ArtDAQ_ReadDigitalU16(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadDigitalU16';
function ArtDAQ_ReadDigitalU32(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadDigitalU32';
function ArtDAQ_ReadDigitalScalarU32(taskHandle:Integer; timeout:double; value:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadDigitalScalarU32';
function ArtDAQ_ReadDigitalLines(taskHandle:Integer; numSampsPerChan:integer; timeout:double; fillMode:integer; readArray:pointer; arraySizeInBytes:cardinal; sampsPerChanRead:pointer; numBytesPerSamp:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadDigitalLines';
function ArtDAQ_ReadDigitalPort(taskHandle:Integer; deviceName:pchar; portIndex:integer; portVal:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadDigitalPort';

function ArtDAQ_ReadCounterF64(taskHandle:Integer; numSampsPerChan:integer; timeout:double; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCounterF64';
function ArtDAQ_ReadCounterU32(taskHandle:Integer; numSampsPerChan:integer; timeout:double; readArray:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCounterU32';
function ArtDAQ_ReadCounterScalarF64(taskHandle:Integer; timeout:double; value:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCounterScalarF64';
function ArtDAQ_ReadCounterScalarU32(taskHandle:Integer; timeout:double; value:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCounterScalarU32';
function ArtDAQ_ReadCtrFreq(taskHandle:Integer; numSampsPerChan:integer; timeout:double; readArrayFrequency:pointer; readArrayDutyCycle:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCtrFreq';
function ArtDAQ_ReadCtrTime(taskHandle:Integer; numSampsPerChan:integer; timeout:double; readArrayHighTime:pointer; readArrayLowTime:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCtrTime';
function ArtDAQ_ReadCtrTicks(taskHandle:Integer; numSampsPerChan:integer; timeout:double; readArrayHighTicks:pointer; readArrayLowTicks:pointer; arraySizeInSamps:cardinal; sampsPerChanRead:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCtrTicks';
function ArtDAQ_ReadCtrFreqScalar(taskHandle:Integer; timeout:double; frequency:pointer; dutyCycle:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCtrFreqScalar';
function ArtDAQ_ReadCtrTimeScalar(taskHandle:Integer; timeout:double; highTime:pointer; lowTime:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCtrTimeScalar';
function ArtDAQ_ReadCtrTicksScalar(taskHandle:Integer; timeout:double; highTicks:pointer; lowTicks:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ReadCtrTicksScalar';

//******************************************************/
//***                 Write Data                     ***/
//******************************************************/
function ArtDAQ_WriteAnalogF64(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteAnalogF64';
function ArtDAQ_WriteAnalogScalarF64(taskHandle:Integer; autoStart:integer; timeout:double; value:double; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteAnalogScalarF64';
function ArtDAQ_WriteBinaryI16(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteBinaryI16';
function ArtDAQ_WriteBinaryU16(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteBinaryU16';

function ArtDAQ_WriteDigitalU8(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteDigitalU8';
function ArtDAQ_WriteDigitalU16(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteDigitalU16';
function ArtDAQ_WriteDigitalU32(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteDigitalU32';
function ArtDAQ_WriteDigitalScalarU32(taskHandle:Integer; autoStart:integer; timeout:double; value:cardinal; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteDigitalScalarU32';
function ArtDAQ_WriteDigitalLines(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; dataLayout:integer; writeArray:pointer; sampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteDigitalLines';

function ArtDAQ_WriteCtrFreq(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; frequency:pointer; dutyCycle:pointer; numSampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteCtrFreq';
function ArtDAQ_WriteCtrTime(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; highTime:pointer; lowTime:pointer; numSampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteCtrTime';
function ArtDAQ_WriteCtrTicks(taskHandle:Integer; numSampsPerChan:integer; autoStart:integer; timeout:double; highTicks:pointer; lowTicks:pointer; numSampsPerChanWritten:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteCtrTicks';
function ArtDAQ_WriteCtrFreqScalar(taskHandle:Integer; autoStart:integer; timeout:double; frequency:double; dutyCycle:double; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteCtrFreqScalar';
function ArtDAQ_WriteCtrTimeScalar(taskHandle:Integer; autoStart:integer; timeout:double; highTime:double; lowTime:double; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteCtrTimeScalar';
function ArtDAQ_WriteCtrTicksScalar(taskHandle:Integer; autoStart:integer; timeout:double; highTicks:double; lowTicks:double; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_WriteCtrTicksScalar';

//******************************************************/
//***               Events & Signals                 ***/
//******************************************************/
// Terminology:  For hardware, "signals" comprise "clocks," "triggers," and (output) "events".
// Software signals or events are not presently supported.

// For possible values for parameter signalID see "Value set Signal" in Values section above.
function ArtDAQ_ExportSignal(taskHandle:Integer; signalID:integer; outputTerminal:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ExportSignal';
function ArtDAQ_ExportCtrOutEvent(taskHandle:Integer; outputTerminal:pchar; outputBehavior:integer; pulsePolarity:integer; toggleIdleState:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ExportCtrOutEvent';

//******************************************************/
//***             Buffer Configurations              ***/
//******************************************************/
function ArtDAQ_CfgInputBuffer(taskHandle:Integer; numSampsPerChan:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgInputBuffer';
function ArtDAQ_CfgOutputBuffer(taskHandle:Integer; numSampsPerChan:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgOutputBuffer';

//******************************************************/
//***   Device Configuration                         ***/
//******************************************************/
function ArtDAQ_ResetDevice(deviceName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetDevice';
function ArtDAQ_SelfTestDevice(deviceName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SelfTestDevice';
function ArtDAQ_GetDeviceAttribute(deviceName:pchar; attributeType:integer; attribute:pointer; taskHandle:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetDeviceAttribute';

function ArtDAQ_SetDigitalPowerUpStates(deviceName:pchar; channelNames:pchar; state:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetDigitalPowerUpStates';
function ArtDAQ_GetDigitalPowerUpStates(deviceName:pchar; channelNames:pchar; state:pointer; arraySize:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetDigitalPowerUpStates';
function ArtDAQ_Set5VPowerOutputStates(deviceName:pchar; outputEnable:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_Set5VPowerOutputStates';
function ArtDAQ_Get5VPowerOutputStates(deviceName:pchar; outputEnable:pointer; reserved:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_Get5VPowerOutputStates';
function ArtDAQ_Set5VPowerPowerUpStates(deviceName:pchar; outputEnable:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_Set5VPowerPowerUpStates';
function ArtDAQ_Get5VPowerPowerUpStates(deviceName:pchar; outputEnable:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_Get5VPowerPowerUpStates';

//******************************************************/
//***                 Calibration                    ***/
//******************************************************/
function ArtDAQ_SelfCal(deviceName:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SelfCal';
function ArtDAQ_GetAICalFactor(taskHandle:integer; physicalChannel:pchar; a:pointer; b:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetAICalFactor';
function ArtDAQ_GetAOCalFactor(taskHandle:integer; physicalChannel:pchar; a:pointer; b:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetAOCalFactor';

// obsolete
function ArtDAQ_GetAICalOffsetAndGain(deviceName:pchar; channel:cardinal; minVal:double; maxVal:double; sampClock:double; offset:pointer; codeWidth:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetAICalOffsetAndGain';
function ArtDAQ_GetAOCalOffsetAndGain(deviceName:pchar; channel:cardinal; minVal:double; maxVal:double; sampClock:double; offset:pointer; codeWidth:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetAOCalOffsetAndGain';

//******************************************************/
//***                 Error Handling                 ***/
//******************************************************/
function ArtDAQ_GetErrorString(errorCode:integer; errorString:pchar; bufferSize:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetErrorString';
function ArtDAQ_GetExtendedErrorInfo(errorString:pchar; bufferSize:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetExtendedErrorInfo';

//******************************************************************************
//*** ART-DAQ Specific Attribute Get/Set/Reset Function Declarations ***********
//******************************************************************************/
//*** Set functions for Run Mode ***
// Uses value set RunMode
function ArtDAQ_SetRunMode(mode:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetRunMode';

//********** Timing **********
//*** Set functions for ArtDAQ_AIConv_Src ***
function ArtDAQ_SetAIConvClk(taskHandle:integer; source:pchar; activeEdge:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetAIConvClk';

//*** Set functions for ArtDAQ_SampClk_Timebase_Src ***
function ArtDAQ_SetSampClkTimebaseSrc(taskHandle:integer; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetSampClkTimebaseSrc';

//*** Set functions for ArtDAQ_Exported_SampClkTimebase_OutputTerm ***
function ArtDAQ_SetExportedSampClkTimebaseOutputTerm(taskHandle:integer; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetExportedSampClkTimebaseOutputTerm';

//*** Set functions for ArtDAQ_RefClk_Src ***
function ArtDAQ_SetRefClkSrc(taskHandle:integer; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetRefClkSrc';

//*** Set functions for ArtDAQ_SyncPulse_Src ***
function ArtDAQ_SetSyncPulseSrc(taskHandle:integer; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetSyncPulseSrc';

//*** Set functions for ArtDAQ_Exported_SyncPulseEvent_OutputTerm ***
function ArtDAQ_SetExportedSyncPulseEventOutputTerm(taskHandle:integer; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetExportedSyncPulseEventOutputTerm';

//********** Trigger **********
//*** Set/Get functions for ArtDAQ_StartTrig_Delay ***
function ArtDAQ_GetStartTrigDelay(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetStartTrigDelay';
function ArtDAQ_SetStartTrigDelay(taskHandle:integer; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetStartTrigDelay';

//*** Set/Get functions for ArtDAQ_StartTrig_DelayUnits ***
// Uses value set DigitalWidthUnits1
function ArtDAQ_GetStartTrigDelayUnits(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetStartTrigDelayUnits';
function ArtDAQ_SetStartTrigDelayUnits(taskHandle:integer; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetStartTrigDelayUnits';

//*** Set/Get functions for ArtDAQ_StartTrig_DigFltr_MinPulseWidth ***
function ArtDAQ_GetStartTrigDigFltrMinPulseWidth(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetStartTrigDigFltrMinPulseWidth';
function ArtDAQ_SetStartTrigDigFltrMinPulseWidth(taskHandle:integer; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetStartTrigDigFltrMinPulseWidth';

//*** Set/Get functions for ArtDAQ_StartTrig_Retriggerable ***
function ArtDAQ_GetStartTrigRetriggerable(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetStartTrigRetriggerable';
function ArtDAQ_SetStartTrigRetriggerable(taskHandle:integer; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetStartTrigRetriggerable';

//*** Set/Get functions for ArtDAQ_RefTrig_DigFltr_MinPulseWidth ***
function ArtDAQ_GetRefTrigDigFltrMinPulseWidth(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetRefTrigDigFltrMinPulseWidth';
function ArtDAQ_SetRefTrigDigFltrMinPulseWidth(taskHandle:integer; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetRefTrigDigFltrMinPulseWidth';

//*** Set/Get functions for ArtDAQ_PauseTrig_DigFltr_MinPulseWidth ***
function ArtDAQ_GetPauseTrigDigFltrMinPulseWidth(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetPauseTrigDigFltrMinPulseWidth';
function ArtDAQ_SetPauseTrigDigFltrMinPulseWidth(taskHandle:integer; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetPauseTrigDigFltrMinPulseWidth';

//********** Read **********
//*** Set/Get functions for ArtDAQ_Read_OverWrite ***
// Uses value set OverwriteMode1
function ArtDAQ_GetReadOverWrite(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetReadOverWrite';
function ArtDAQ_SetReadOverWrite(taskHandle:integer; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetReadOverWrite';

//*** Set/Get functions for ArtDAQ_Read_AutoStart ***
function ArtDAQ_GetReadAutoStart(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetReadAutoStart';
function ArtDAQ_SetReadAutoStart(taskHandle:integer; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetReadAutoStart';

//********** Write **********
//*** Set/Get functions for ArtDAQ_Write_RegenMode ***
// Uses value set RegenerationMode1
function ArtDAQ_GetWriteRegenMode(taskHandle:integer; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetWriteRegenMode';
function ArtDAQ_SetWriteRegenMode(taskHandle:integer; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetWriteRegenMode';

// AI
//*** Set/Get functions for ArtDAQ_AI_InputSrc ***
function ArtDAQ_GetAIInputSrc(taskHandle:integer; device:pchar; data:pointer; bufferSize:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetAIInputSrc';
function ArtDAQ_SetAIInputSrc(taskHandle:integer; device:pchar; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetAIInputSrc';

//********** CTR **********
//*** Set/Get functions for CI CountEdges CountReset ***
function ArtDAQ_CfgCICountEdgesCountReset(taskHandle:integer; sourceTerminal:pchar; resetCount:cardinal; activeEdge:integer; digFltrMinPulseWidth:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_CfgCICountEdgesCountReset';
function ArtDAQ_DisableCICountEdgesCountReset(taskHandle:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_DisableCICountEdgesCountReset';

//*** Set/Get functions for ArtDAQ_CI_Source_DigFltr_MinPulseWidth ***
function ArtDAQ_GetCISourceDigFltrMinPulseWidth(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCISourceDigFltrMinPulseWidth';
function ArtDAQ_SetCISourceDigFltrMinPulseWidth(taskHandle:integer; channel:pchar; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCISourceDigFltrMinPulseWidth';
function ArtDAQ_ResetCISourceDigFltrMinPulseWidth(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCISourceDigFltrMinPulseWidth';

//*** Set/Get functions for ArtDAQ_CI_Gate_DigFltr_MinPulseWidth ***
function ArtDAQ_GetCIGateDigFltrMinPulseWidth(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCIGateDigFltrMinPulseWidth';
function ArtDAQ_SetCIGateDigFltrMinPulseWidth(taskHandle:integer; channel:pchar; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCIGateDigFltrMinPulseWidth';
function ArtDAQ_ResetCIGateDigFltrMinPulseWidth(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCIGateDigFltrMinPulseWidth';

//*** Set/Get functions for ArtDAQ_CI_Aux_DigFltr_MinPulseWidth ***
function ArtDAQ_GetCIAuxDigFltrMinPulseWidth(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCIAuxDigFltrMinPulseWidth';
function ArtDAQ_SetCIAuxDigFltrMinPulseWidth(taskHandle:integer; channel:pchar; data:double):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCIAuxDigFltrMinPulseWidth';
function ArtDAQ_ResetCIAuxDigFltrMinPulseWidth(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCIAuxDigFltrMinPulseWidth';

//*** Set/Get functions for ArtDAQ_CI_Encoder_AInputInvert ***
function ArtDAQ_GetCIEncoderAInputInvert(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCIEncoderAInputInvert';
function ArtDAQ_SetCIEncoderAInputInvert(taskHandle:integer; channel:pchar; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCIEncoderAInputInvert';
function ArtDAQ_ResetCIEncoderAInputInvert(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCIEncoderAInputInvert';

//*** Set/Get functions for ArtDAQ_CI_Encoder_BInputInvert ***
function ArtDAQ_GetCIEncoderBInputInvert(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCIEncoderBInputInvert';
function ArtDAQ_SetCIEncoderBInputInvert(taskHandle:integer; channel:pchar; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCIEncoderBInputInvert';
function ArtDAQ_ResetCIEncoderBInputInvert(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCIEncoderBInputInvert';

//*** Set/Get functions for ArtDAQ_CI_Encoder_ZInputInvert ***
function ArtDAQ_GetCIEncoderZInputInvert(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCIEncoderZInputInvert';
function ArtDAQ_SetCIEncoderZInputInvert(taskHandle:integer; channel:pchar; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCIEncoderZInputInvert';
function ArtDAQ_ResetCIEncoderZInputInvert(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCIEncoderZInputInvert';

//*** Set/Get functions for ArtDAQ_CO_Pulse_Term ***
function ArtDAQ_GetCOPulseTerm(taskHandle:integer; channel:pchar; data:pchar; bufferSize:cardinal):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCOPulseTerm';
function ArtDAQ_SetCOPulseTerm(taskHandle:integer; channel:pchar; data:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCOPulseTerm';
function ArtDAQ_ResetCOPulseTerm(taskHandle:integer; channel:pchar):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_ResetCOPulseTerm';

//*** Set/Get functions for ArtDAQ_CO_Count ***
function ArtDAQ_GetCOCount(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCOCount';

//*** Set/Get functions for ArtDAQ_CO_OutputState ***
// Uses value set Level1
function ArtDAQ_GetCOOutputState(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCOOutputState';

//*** Set/Get functions for ArtDAQ_CO_EnableInitialDelayOnRetrigger ***
function ArtDAQ_GetCOEnableInitialDelayOnRetrigger(taskHandle:integer; channel:pchar; data:pointer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_GetCOEnableInitialDelayOnRetrigger';
function ArtDAQ_SetCOEnableInitialDelayOnRetrigger(taskHandle:integer; channel:pchar; data:integer):Integer;stdcall;external 'Art_DAQ.dll' name 'ArtDAQ_SetCOEnableInitialDelayOnRetrigger';

implementation

end.
