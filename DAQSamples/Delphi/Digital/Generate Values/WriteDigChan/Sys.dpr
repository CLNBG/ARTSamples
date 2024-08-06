program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  strCardName:string;
  strChannelName:string;
  errbuf:Array[0..2047] of char;
  data:array[0..7] of byte;
  
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
  strChannelName := strCardName + '/port0/line0:7';
  ret := ArtDAQ_CreateDOChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanForAllLines);
  if(ret < 0) then
  begin
    Writeln('创建DO失败!');
    goto ExitSys;
  end;

  data[0] := 1;
  data[1] := 0;
  data[2] := 0;
  data[3] := 1;
  data[4] := 1;
  data[5] := 1;
  data[6] := 1;
  data[7] := 1;

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
  // ArtDAQ Write Code
  //*********************************************/
  ret := ArtDAQ_WriteDigitalLines(taskHandle, 1, 1, 10.0, ArtDAQ_Val_GroupByChannel, @data, nil, nil);
  if(ret < 0) then
  begin
    Writeln('设置DO失败!');
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
