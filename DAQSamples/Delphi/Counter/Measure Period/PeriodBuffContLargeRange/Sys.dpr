program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  read:integer;
  samplePerChan:UINT64;
  strCardName:string;
  strChannelName:string;
  errbuf:Array[0..2047] of char;
  data:array[0..999] of double;
  
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
  strChannelName := strCardName + '/ctr0';
  ret := ArtDAQ_CreateCIPeriodChan(taskHandle, pchar(strChannelName), '', 0.000000002, 4.29496, ArtDAQ_Val_Seconds, ArtDAQ_Val_Rising, ArtDAQ_Val_LargeRng2Ctr, 0.001, 10, '');
  if(ret < 0) then
  begin
    Writeln('创建CTR0失败!');
    goto ExitSys;
  end;

  samplePerChan.low32 := 1000;
  ret := ArtDAQ_CfgImplicitTiming(taskHandle, ArtDAQ_Val_ContSamps, samplePerChan);
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
    Writeln('开始CTR失败!');
    goto ExitSys;
  end;

  Writeln('读取周期...');
  while(kbhit()=0) do
  begin
    ret := ArtDAQ_ReadCounterF64(taskHandle, 1000, 10.0, @data, 1000, @read, nil);
    if(ret < 0) then
    begin
     Writeln('读数失败!');
      goto ExitSys;
    end
    else
    begin
      Writeln(Format('Acquired %d samples',[read]));
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
