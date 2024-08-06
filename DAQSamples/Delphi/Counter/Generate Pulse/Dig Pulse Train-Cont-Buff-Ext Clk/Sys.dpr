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
  strExtSource:string;
  dataFreq:array[0..999] of double;
  dataDC:array[0..999] of double;
  dutyStep:double;
  index:integer;
  errBuff:array[0..2047] of char;

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

  dutyStep := (0.8-0.5)/1000;
  for index:=0 to 999 do
  begin
    dataFreq[index] := 1000.0;
    dataDC[index] := 0.5 + dutyStep*index;
  end;

  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('�����豸ʧ��!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/ctr0';
  ret := ArtDAQ_CreateCOPulseChanFreq(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_Hz, ArtDAQ_Val_Low, 0.0, 1.0, 0.5);
  if(ret < 0) then
  begin
    Writeln('����CTR0ʧ��!');
    goto ExitSys;
  end;

  strExtSource := '/' + strCardName + '/PFI0';
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, pchar(strExtSource), 200, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('����ʱ�Ӳ���ʧ��!');
    goto ExitSys;
  end;

  ret := ArtDAQ_WriteCtrFreq(taskHandle, 1000, 0, 10.0, @dataFreq, @dataDC, nil, nil);
  if(ret < 0) then
  begin
    Writeln('д������Ƶ��ʧ��!');
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

  while (kbhit() = 0) do
  begin
    Writeln('���������...');
  end;

  //*********************************************/
  // ArtDAQ Stop Code
  //*********************************************/
ExitSys:
  if(ret < 0) then
  begin
     ArtDAQ_GetExtendedErrorInfo(@errBuff, 2048);
     Writeln(errBuff);
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