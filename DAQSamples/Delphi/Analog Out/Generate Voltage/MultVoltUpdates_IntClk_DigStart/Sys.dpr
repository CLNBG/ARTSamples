program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  data:array[0..3999] of double;
  ret:integer;
  strCardName:string;
  strChannelName:string;
  strTrigSource:string;
  index:integer;
  written:integer;
  errbuf:array[0..2047] of char;

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

  for index := 0 to 3999 do
  begin
    data[index] := 9.95*sin(index*2.0*3.1415926535/1000.0);
  end;

  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('�����豸ʧ��!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/ao0';
  ret := ArtDAQ_CreateAOVoltageChan(taskHandle, pchar(strChannelName), '', -10.0, 10.0, ArtDAQ_Val_Volts, '');
  if(ret < 0) then
  begin
    Writeln('����AOʧ��!');
    goto ExitSys;
  end;

  ret := ArtDAQ_CfgSampClkTiming(taskHandle, '', 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 4000);
  if(ret < 0) then
  begin
    Writeln('���ò���ʱ��ʧ��!');
    goto ExitSys;
  end;

  strTrigSource := '/' + strCardName + '/PFI1';
  ret := ArtDAQ_CfgDigEdgeStartTrig(taskHandle, pchar(strTrigSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('�����ⲿ���ִ���ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Write Code
	//*********************************************/
  ret := ArtDAQ_WriteAnalogF64(taskHandle, 4000, 0, 10.0, ArtDAQ_Val_GroupByChannel, @data, @written, nil);
  if(ret < 0) then
  begin
    Writeln('д����ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('��ʼ���ʧ��!');
    goto ExitSys;
  end;

  ret := ArtDAQ_WaitUntilTaskDone(taskHandle, 10.0);
  if(ret < 0) then
  begin
    Writeln('�ȴ�AO������ʧ��!');
    goto ExitSys;
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

  Writeln('��Enter���˳�!');
  getch();
  Readln;
  
end.
