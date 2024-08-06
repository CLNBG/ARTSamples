program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  taskHandle:integer;
  data:array[0..999] of double;
  read:integer;
  ret:integer;
  strCardName:string;
  strChannelName:string;
  strExtSource:string;
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
  ret := ArtDAQ_CreateTask('', @taskHandle);
  if(ret < 0) then
  begin
    Writeln('�����豸ʧ��!');
    goto ExitSys;
  end;
  strChannelName := strCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(taskHandle, pchar(strChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('����AIʧ��!');
    goto ExitSys;
  end;

  // ͬ������ʱ��ֻ��Ҫ����ArtDAQ_CfgSampClkTiming,�ο�����ע�͵Ĵ���
  {ret := ArtDAQ_CfgSampClkTiming(taskHandle, '/Dev1/PFI2', 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('���ò���ʱ��ʧ��!');
    getch();
    exit;
  end;}

  // �첽����ʱ����Ҫ����ArtDAQ_CfgSampClkTiming��ArtDAQ_SetAIConvClk
  ret := ArtDAQ_CfgSampClkTiming(taskHandle, '', 1000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_FiniteSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('���ò���ʱ��ʧ��!');
    goto ExitSys;
  end;

  strExtSource := '/' + strCardName + '/PFI2';
  ret := ArtDAQ_SetAIConvClk(taskHandle, pchar(strExtSource), ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('����ת��ʱ��ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  ret := ArtDAQ_StartTask(taskHandle);
  if(ret < 0) then
  begin
    Writeln('��ʼ�ɼ�ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Read Code
	//*********************************************/
  ret := ArtDAQ_ReadAnalogF64(taskHandle, -1, 10.0, ArtDAQ_Val_GroupByChannel, @data, 1000, @read, nil);
  if(ret < 0) then
  begin
    Writeln('����ʧ��!');
    goto ExitSys;
  end
  else
  begin
    Writeln(Format('Acquired %d samples',[read]));
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
