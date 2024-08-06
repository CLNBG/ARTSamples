program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  numRead:integer;
  strCardName:string;
  strChannelName:string;
  strTrigSource:string;
  errbuf:Array[0..2047] of char;
  data:array[0..7999] of byte;
  
Label ExitSys;
Function kbhit() : Integer;stdcall;external 'MSVCRT.DLL' name '_kbhit';
Function getch() : Integer; Stdcall; External'MSVCRT.DLL' name'_getch';

begin
  { TODO -oUser -cConsole Main : Insert code here }
  //*********************************************/
	// ArtDAQ Configure Code
	//*********************************************/
  Writeln('������忨��DMC��ʶ���������:(��:Dev1)');
  Readln(strCardName);
  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('�����豸ʧ��!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/port0/line0:7';
  ret := ArtDAQ_CreateDIChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanPerLine);
  if(ret < 0) then
  begin
    Writeln('����DIʧ��!');
    goto ExitSys;
  end;

  ret := ArtDAQ_CfgSampClkTiming(taskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('����ʱ�Ӳ���ʧ��!');
    goto ExitSys;
  end;

  strTrigSource := '/' + strCardName + '/PFI0';
  ret := ArtDAQ_CfgDigEdgeStartTrig(taskHandle, pchar(strTrigSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('���ô���ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('��ʼDO���ʧ��!');
    goto ExitSys;
  end;

  ret := ArtDAQ_ReadDigitalLines(taskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @data, 8000, @numRead, nil, nil);
  if(ret < 0) then
  begin
    Writeln('����ʧ��');
    goto ExitSys;
  end
  else
  begin
    Writeln(Format('Acquired %d samples',[numRead]));
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

  Writeln('��Enter���˳�!');
  getch();
  Readln;
  
end.
