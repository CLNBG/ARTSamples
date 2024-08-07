program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  data:array[0..999] of double;
  read:integer;
  totalRead:integer;
  strCardName:string;
  strChannelName:string;
  errbuf:array[0..2047] of char;

Label ExitSys;
Function kbhit() : Integer;stdcall;external 'MSVCRT.DLL' name '_kbhit';
Function getch() : Integer; Stdcall; External'MSVCRT.DLL' name'_getch';
Function getchar() : Integer; Stdcall; External 'MSVCRT.DLL' name '_getchar';

begin
  { TODO -oUser -cConsole Main : Insert code here }
  totalRead := 0;
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

  ret := ArtDAQ_CfgSampClkTiming(taskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('配置采样时钟失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_CfgAnlgEdgeStartTrig(taskHandle, pchar(strChannelName), ArtDAQ_Val_Rising, 1.0);
  if(ret < 0) then
  begin
    Writeln('配置开始触发参数失败!');
    goto ExitSys;
  end;

  ret := ArtDAQ_SetStartTrigDelayUnits(taskHandle, ArtDAQ_Val_SampClkPeriods);
  if(ret < 0) then
  begin
    Writeln('配置延迟单位失败!');
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
	// ArtDAQ read data
	//*********************************************/
  Writeln('开始读取数据，按任意键退出...');
  while kbhit() = 0 do
  begin
    ret := ArtDAQ_ReadAnalogF64(taskHandle, 1000, 10.0, ArtDAQ_Val_GroupByScanNumber, @data, 1000, @read, nil);
    if(read > 0) then
    begin
      totalRead := totalRead + read;
      Writeln(Format('Acquired %d samples. Total %d',[read,totalRead]));
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
  if(taskHandle <> 0) then
  begin
     ArtDAQ_StopTask(taskHandle);
     ArtDAQ_ClearTask(taskHandle);
  end;

  Writeln('按Enter键退出!');
  getch();
  Readln;
  
end.
