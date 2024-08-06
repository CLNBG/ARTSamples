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
  data:array[0..999] of double;
  
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
  strChannelName := strCardName + '/ctr0';
  ret := ArtDAQ_CreateCIPeriodChan(taskHandle, pchar(strChannelName), '', 0.00000002, 42.9496, ArtDAQ_Val_Seconds, ArtDAQ_Val_Rising, ArtDAQ_Val_LowFreq1Ctr, 0.001, 4, '');
  if(ret < 0) then
  begin
    Writeln('����CTR0ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('��ʼCTRʧ��!');
    goto ExitSys;
  end;

  ret := ArtDAQ_ReadCounterScalarF64(taskHandle, 10.0, @data[0], nil);
  if(ret < 0) then
  begin
    Writeln('����ʧ��!');
    goto ExitSys;
  end
  else
  begin
    Writeln(Format('Frequency: %.9f s',[data[0]]));
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
