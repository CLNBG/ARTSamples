program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  masterTaskHandle:integer;
  slaveTaskHandle:integer;
  masterData:array[0..999] of double;
  slaveData:array[0..999] of double;
  masterRead:integer;
  slaveRead:integer;
  ret:integer;
  synchronizationType:integer;  // 0-采样时钟 1-参考时钟
  strMasterCardName:string;
  strSlaveCardName:string;
  strMasterChannelName:string;
  strSlaveChannelName:string;
  errbuf:array[0..2047] of char;

Label ExitSys;
Function kbhit() : Integer;stdcall;external 'MSVCRT.DLL' name '_kbhit';
Function getch() : Integer; Stdcall; External'MSVCRT.DLL' name'_getch';

begin
  { TODO -oUser -cConsole Main : Insert code here }
  //*********************************************/
	// ArtDAQ Configure Code
	//*********************************************/
  Writeln('请输入主卡在DMC内识别出的名字:(如:Dev1)');
  Readln(strMasterCardName);
  Writeln('请输入从卡在DMC内识别出的名字:(如:Dev1)');
  Readln(strSlaveCardName);

  // 创建主设备
  ret := ArtDAQ_CreateTask('', @masterTaskHandle);
  if(ret < 0) then
  begin
    Writeln('创建主设备失败!');
    goto ExitSys;
  end;
  strMasterChannelName := strMasterCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(masterTaskHandle, pchar(strMasterChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('创建主设备AI失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgSampClkTiming(masterTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('主卡配置采样时钟失败!');
    goto ExitSys;
  end;

  // 创建从设备
  ret := ArtDAQ_CreateTask('', @slaveTaskHandle);
  if(ret < 0) then
  begin
    Writeln('创建从设备失败!');
    goto ExitSys;
  end;
  strSlaveChannelName := strSlaveCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(slaveTaskHandle, pchar(strSlaveChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('创建主设备AI失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgSampClkTiming(slaveTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('从卡配置采样时钟失败!');
    goto ExitSys;
  end;

  synchronizationType := 0;
  if(synchronizationType = 0) then
  begin
     // 选择参考时钟同步，主从卡都需要从板卡指定引脚(PCI卡指定RTSI9，PXI卡指定PXI_10M)
     ret := ArtDAQ_SetRefClkSrc(masterTaskHandle, 'RTSI9');
     if(ret < 0) then
     begin
      Writeln('配置主卡参考时钟失败!');
      goto ExitSys;
     end;
     ret := ArtDAQ_SetRefClkSrc(slaveTaskHandle, 'RTSI9');
     if(ret < 0) then
     begin
      Writeln('配置从卡参考时钟失败!');
      goto ExitSys;
     end;
  end
  else
  begin
     // 主卡导出主采样时钟时基，从卡输入采样时钟时基
     ret := ArtDAQ_SetExportedSampClkTimebaseOutputTerm(masterTaskHandle, 'RTSI8');
     if(ret < 0) then
     begin
      Writeln('主卡导出采样时钟失败!');
      goto ExitSys;
     end;
     ret := ArtDAQ_SetSampClkTimebaseSrc(slaveTaskHandle, 'RTSI8');
     if(ret < 0) then
     begin
      Writeln('从卡配置采样时基失败!');
      goto ExitSys;
     end;
  end;

  // 主卡输出同步脉冲，从卡输入同步脉冲，用来同步DDC
  ret := ArtDAQ_SetExportedSyncPulseEventOutputTerm(masterTaskHandle, 'RTSI0');
  if(ret < 0) then
  begin
    Writeln('主卡输出同步脉冲失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_SetSyncPulseSrc(slaveTaskHandle, 'RTSI0');
  if(ret < 0) then
  begin
    Writeln('从卡输入同步脉冲失败!');
    goto ExitSys;
  end;

  // 主卡可以选择外部触发，并将触发导出；也可以不配置触发，导出内部的触发信号来同步采集
  //ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(gMasterAItaskHandle,"PFI0",ArtDAQ_Val_Rising));
  ret := ArtDAQ_ExportSignal(masterTaskHandle, ArtDAQ_Val_StartTrigger, 'RTSI1');
  if(ret < 0) then
  begin
    Writeln('主卡导出触发失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgDigEdgeStartTrig(slaveTaskHandle, 'RTSI1', ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('从卡配置触发失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  // 从卡先开始
  ret := ArtDAQ_StartTask(slaveTaskHandle);
  if(ret < 0) then
  begin
    Writeln('从卡开始采集失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_StartTask(masterTaskHandle);
  if(ret < 0) then
  begin
    Writeln('主卡开始采集失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Read Code
	//*********************************************/
  while(kbhit()=0) do
  begin
    ret := ArtDAQ_ReadAnalogF64(masterTaskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @masterData, 1000, @masterRead, nil);
    if(ret < 0) then
    begin
      Writeln('读数失败!');
      goto ExitSys;
    end
    else
    begin
      Writeln(Format('Master acquired %d samples',[masterRead]));
    end;

    ret := ArtDAQ_ReadAnalogF64(slaveTaskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @slaveData, 1000, @slaveRead, nil);
    if(ret < 0) then
    begin
      Writeln('读数失败!');
      goto ExitSys;
    end
    else
    begin
      Writeln(Format('Slave acquired %d samples',[slaveRead]));
    end;
  end;

  //*********************************************/
  // ArtDAQ Stop Code
  //*********************************************/
ExitSys:
  if(ret < 0) then
  begin
    ArtDAQ_GetExtendedErrorInfo(@errbuf, 2048);
    Writeln(errbuf);
  end;
  if(masterTaskHandle <> 0) then
  begin
     ArtDAQ_StopTask(masterTaskHandle);
     ArtDAQ_ClearTask(masterTaskHandle);
  end;
  if(slaveTaskHandle <> 0) then
  begin
     ArtDAQ_StopTask(slaveTaskHandle);
     ArtDAQ_ClearTask(slaveTaskHandle);
  end;

  Writeln('按Enter键退出!');
  getch();
  Readln;
  
end.
