program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  index:integer;
  strCardName:string;
  strChannelName:string;
  strExtClock:string;
  errbuf:Array[0..2047] of char;
  data:array[0..999] of byte;
  
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
  strChannelName := strCardName + '/port0/line0';
  ret := ArtDAQ_CreateDOChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanPerLine);
  if(ret < 0) then
  begin
    Writeln('创建DO失败!');
    goto ExitSys;
  end;

  strExtClock := '/' + strCardName + '/PFI0';
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, pchar(strExtClock), 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('时钟配置失败!');
    goto ExitSys;
  end;

  for index:=0 to 999 do
  begin
    data[index] := index mod 2;
  end;

  //*********************************************/
  // ArtDAQ Write Code
  //*********************************************/
  ret := ArtDAQ_WriteDigitalLines(taskHandle, 1000, 0, 10.0, ArtDAQ_Val_GroupByChannel, @data, nil, nil);
  if(ret < 0) then
  begin
    Writeln('设置DO失败!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('开始DO输出失败!');
    goto ExitSys;
  end;

  //*********************************************/
  // DAQmx Wait Code
  //*********************************************/
  ret := ArtDAQ_WaitUntilTaskDone(taskHandle, 10.0);
  if(ret < 0) then
  begin
    Writeln('等待任务完成失败!');
    goto ExitSys;
  end;

  //*********************************************/
  // ArtDAQ Stop Code
  //*********************************************/
ExitSys:
  if(ret < 0) then
  begin
    ArtDAQ_GetExtendedErrorInfo(@errbuf,2048);
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
