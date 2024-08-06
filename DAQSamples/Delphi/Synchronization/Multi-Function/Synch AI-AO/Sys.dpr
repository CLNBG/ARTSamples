program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  AItaskHandle:integer;
  AOtaskHandle:integer;
  AIdata:array[0..999] of double;
  AOdata:array[0..999] of double;
  sineWave:array[0..999] of double;
  readAI:integer;
  ret:integer;
  strCardName:string;
  strAIChannelName:string;
  strAOChannelName:string;
  strTrigSource:string;
  phase:double;
  errbuf:array[0..2047] of char;

Label ExitSys;
Function kbhit() : Integer;stdcall;external 'MSVCRT.DLL' name '_kbhit';
Function getch() : Integer; Stdcall; External'MSVCRT.DLL' name'_getch';

Function GenSineWave(amplitude:double;frequency:double):integer;
var
  i:integer;
  x:double;
  y:double;
begin
  for i:=0 to 999 do
  begin
    sineWave[i] := amplitude*sin(3.1415929/180.0*(phase+360.0*frequency*i));
  end;
  x:=phase+frequency*360.0*1000;
  y:=360.0;
  phase := x-(x/y)*y;
end;

begin
  { TODO -oUser -cConsole Main : Insert code here }
  //*********************************************/
	// ArtDAQ Configure Code
	//*********************************************/
  Writeln('请输入板卡在DMC内识别出的名字:(如:Dev1)');
  Readln(strCardName);

  // 创建设备AI任务
  ret := ArtDAQ_CreateTask('', @AItaskHandle);
  if(ret < 0) then
  begin
    Writeln('创建设备AI任务失败!');
    goto ExitSys;
  end;
  strAIChannelName := strCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(AItaskHandle, pchar(strAIChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('创建设备AI任务失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgSampClkTiming(AItaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('AI配置采样时钟失败!');
    goto ExitSys;
  end;
  strTrigSource := '/' + strCardName + '/PFI1';
  ret := ArtDAQ_CfgDigEdgeStartTrig(AItaskHandle, pchar(strTrigSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('配置AI触发失败!');
    goto ExitSys;
  end;

  // 创建设备AO任务
  ret := ArtDAQ_CreateTask('', @AOtaskHandle);
  if(ret < 0) then
  begin
    Writeln('创建设备AO任务失败!');
    goto ExitSys;
  end;
  strAOChannelName := strCardName + '/ao0';
  ret := ArtDAQ_CreateAOVoltageChan(AOtaskHandle, pchar(strAOChannelName), '', -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('创建设备A0失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgSampClkTiming(AOtaskHandle, '', 5000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('AO配置采样时钟失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgDigEdgeStartTrig(AOtaskHandle, pchar(strTrigSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('AO配置触发失败!');
    goto ExitSys;
  end;

  // 产生正弦波
  phase:=0;
  GenSineWave(1.0,1.0/1000);

  ret := ArtDAQ_WriteAnalogF64(AOtaskHandle, 1000, 0, 10.0, ArtDAQ_Val_GroupByChannel, @AOdata, nil, nil);
  if(ret < 0) then
  begin
    Writeln('写入AO失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  // 开始AO
  ret := ArtDAQ_StartTask(AOtaskHandle);
  if(ret < 0) then
  begin
    Writeln('AO开始失败!');
    goto ExitSys;
  end;
  ret := ArtDAQ_StartTask(AItaskHandle);
  if(ret < 0) then
  begin
    Writeln('AI开始失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Read Code
	//*********************************************/
  while(kbhit()=0) do
  begin
    ret := ArtDAQ_ReadAnalogF64(AItaskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @AIdata, 1000, @readAI, nil);
    if(ret < 0) then
    begin
      Writeln('AI读数失败!');
      goto ExitSys;
    end
    else
    begin
      Writeln(Format('AI acquired %d samples',[readAI]));
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
  if(AItaskHandle <> 0) then
  begin
     ArtDAQ_StopTask(AItaskHandle);
     ArtDAQ_ClearTask(AItaskHandle);
  end;
  if(AOtaskHandle <> 0) then
  begin
     ArtDAQ_StopTask(AOtaskHandle);
     ArtDAQ_ClearTask(AOtaskHandle);
  end;

  Writeln('按Enter键退出!');
  getch();
  Readln;
  
end.
