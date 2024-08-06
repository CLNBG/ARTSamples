program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  sampsRead:integer;
  strCardName:string;
  strChannelName:string;
  strExtClock:string;
  errbuf:Array[0..2047] of char;
  data:array[0..999] of cardinal;
  
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
  strChannelName := strCardName + '/port0';
  ret := ArtDAQ_CreateDIChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanForAllLines);
  if(ret < 0) then
  begin
    Writeln('创建DI失败!');
    goto ExitSys;
  end;

  strExtClock := '/' + strCardName + '/PFI0';
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, pchar(strExtClock), 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('配置时钟失败!');
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

  ret := ArtDAQ_ReadDigitalU32(taskHandle, -1, 10.0, ArtDAQ_Val_GroupByChannel, @data, 1000, @sampsRead, nil);
  if(ret < 0) then
  begin
    Writeln('读数失败');
    goto ExitSys;
  end
  else
  begin
    Writeln(Format('Acquired %d samples',[sampsRead]));
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
