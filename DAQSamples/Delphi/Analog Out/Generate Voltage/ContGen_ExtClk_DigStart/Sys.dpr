program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  data:array[0..999] of double;
  ret:integer;
  strCardName:string;
  strChannelName:string;
  strExtSource:string;
  strTrigSource:string;
  index:integer;
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

  for index := 0 to 999 do
  begin
    data[index] := 9.95*sin(index*2.0*3.1415926535/1000.0);
  end;

  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('创建设备失败!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/ao0';
  ret := ArtDAQ_CreateAOVoltageChan(taskHandle, pchar(strChannelName), '', -10.0, 10.0, ArtDAQ_Val_Volts, '');
  if(ret < 0) then
  begin
    Writeln('创建AO失败!');
    goto ExitSys;
  end;

  strExtSource := '/' + strCardName + '/PFI2';
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, pchar(strExtSource), 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('配置采样时钟失败!');
    goto ExitSys;
  end;

  strTrigSource := '/' + strCardName + '/PFI1';
  ret := ArtDAQ_CfgDigEdgeStartTrig(taskHandle, pchar(strTrigSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('配置开始触发参数失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Write Code
	//*********************************************/
  ret := ArtDAQ_WriteAnalogF64(taskHandle, 1000, 0, 10.0, ArtDAQ_Val_GroupByChannel, @data, nil, nil);
  if(ret < 0) then
  begin
    Writeln('写数据失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('开始输出失败!');
    goto ExitSys;
  end;

  while (kbhit() = 0) do
  begin
    Writeln('连续输出中，按任意键退出...');
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
