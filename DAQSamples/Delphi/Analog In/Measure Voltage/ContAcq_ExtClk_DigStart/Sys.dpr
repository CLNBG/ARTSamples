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
  strExtSource:string;
  strTrigSource:string;
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

  // 同步卡的时候只需要配置ArtDAQ_CfgSampClkTiming,参看以下注释的代码
  {ret := ArtDAQ_CfgSampClkTiming(taskHandle, '/Dev1/PFI2', 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('配置采样时钟失败!');
    getch();
    exit;
  end;}

  // 异步卡的时候需要配置ArtDAQ_CfgSampClkTiming和ArtDAQ_SetAIConvClk USB2861是异步卡
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, '', 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('配置采样时钟失败!');
    goto ExitSys;
  end;
  strExtSource := '/' + strCardName + '/PFI2';
  ret := ArtDAQ_SetAIConvClk(taskHandle, pchar(strExtSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('配置转换时钟失败!');
    goto ExitSys;
  end;
  strTrigSource := '/' + strCardName + '/PFI1';
  ret := ArtDAQ_CfgDigEdgeStartTrig(taskHandle, pchar(strTrigSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('配置触发失败!');
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
