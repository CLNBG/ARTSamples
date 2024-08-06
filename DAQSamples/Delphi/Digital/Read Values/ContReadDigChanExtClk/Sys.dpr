program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  sampsRead:integer;
  totalRead:integer;
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
  Writeln('������忨��DMC��ʶ���������:(��:Dev1)');
  Readln(strCardName);
  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('�����豸ʧ��!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/port0/line0';
  ret := ArtDAQ_CreateDIChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanPerLine);
  if(ret < 0) then
  begin
    Writeln('����DIʧ��!');
    goto ExitSys;
  end;

  strExtClock := '/' + strCardName + '/PFI0';
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, pchar(strExtClock), 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('����ʱ��ʧ��!');
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

  totalRead := 0;
  Writeln('��ȡ����...');
  while(kbhit()=0) do
  begin
    ret := ArtDAQ_ReadDigitalU32(taskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @data, 1000, @sampsRead, nil);
    if(sampsRead > 0) then
    begin
      totalRead := totalRead + sampsRead;
      Writeln(Format('Acquired %d samples. Total %d',[sampsRead,totalRead]));
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

  Writeln('��Enter���˳�!');
  getch();
  Readln;
  
end.
