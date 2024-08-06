program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  data:array[0..999] of double;
  read:integer;
  ret:integer;
  strCardName:string;
  strChannelName:string;
  errbuf:array[0..2047] of char;

Label ExitSys;
Function kbhit() : Integer;stdcall;external 'MSVCRT.DLL' name '_kbhit';
Function getch() : Integer; Stdcall; External'MSVCRT.DLL' name'_getch';

begin
  { TODO -oUser -cConsole Main : Insert code here }
  //*********************************************/
	// ArtDAQ Configure Code
	//*********************************************/
  Writeln('请输入板卡在DMC内识别出的名字:(如:Dev1)');
  Readln(strCardName);
  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('创建设备失败!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('创建AI失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_CfgSampClkTiming(taskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('配置采样时钟失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_CfgAnlgEdgeStartTrig(taskHandle, pchar(strChannelName), ArtDAQ_Val_Rising, 1.0);
  if(ret < 0) then
  begin
    Writeln('配置触发失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_SetStartTrigDelayUnits(taskHandle, ArtDAQ_Val_SampClkPeriods);
  if(ret < 0) then
  begin
    Writeln('配置开始触发延时参数失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_SetStartTrigDelay(taskHandle, 100);
  if(ret < 0) then
  begin
    Writeln('配置延迟点数失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_SetStartTrigDigFltrMinPulseWidth(taskHandle, 0.0001);
  if(ret < 0) then
  begin
    Writeln('配置最小脉冲宽度失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('开始采集失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Read Code
	//*********************************************/
  ret := ArtDAQ_ReadAnalogF64(taskHandle, -1, 10.0, ArtDAQ_Val_GroupByChannel, @data, 1000, @read, nil);
  if(ret < 0) then
  begin
    Writeln('读数失败!');
    goto ExitSys;
  end
  else
  begin
    Writeln(Format('Acquired %d samples',[read]));
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
  if(taskHandle <> 0) then
  begin
     ArtDAQ_StopTask(taskHandle);
     ArtDAQ_ClearTask(taskHandle);
  end;

  Writeln('按Enter键退出!');
  getch();
  Readln;
  
end.
