program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  ret:integer;
  read:integer;
  strCardName:string;
  strChannelName:string;
  errbuf:Array[0..2047] of char;
  data:cardinal;
  
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
  strChannelName := strCardName + '/port0';
  ret := ArtDAQ_CreateDIChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_ChanForAllLines);
  if(ret < 0) then
  begin
    Writeln('����DIʧ��!');
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

  ret := ArtDAQ_ReadDigitalU32(taskHandle, 1, 10.0, ArtDAQ_Val_GroupByChannel, @data, 1, @read, nil);
  if(ret < 0) then
  begin
    Writeln('����ʧ��');
    goto ExitSys;
  end
  else
  begin
    Writeln(Format('Data acquired: 0x%X',[data]));
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
