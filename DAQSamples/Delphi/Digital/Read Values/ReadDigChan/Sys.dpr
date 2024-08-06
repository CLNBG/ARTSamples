program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  read:integer;
  index:integer;
  bytesPerSamp:integer;
  strCardName:string;
  strChannelName:string;
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
  strChannelName := strCardName + '/port0/line0:7';
  ret := ArtDAQ_CreateDIChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanPerLine);
  if(ret < 0) then
  begin
    Writeln('创建DI失败!');
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

  ret := ArtDAQ_ReadDigitalLines(taskHandle, 1, 10.0, ArtDAQ_Val_GroupByChannel, @data, 100, @read, @bytesPerSamp, nil);
  if(ret < 0) then
  begin
    Writeln('读数失败');
    goto ExitSys;
  end
  else
  begin
    for index:=0 to 7 do
    begin
      Writeln(Format('Data acquired, channel %d: 0x%X',[index,data[index]]));
    end;
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
