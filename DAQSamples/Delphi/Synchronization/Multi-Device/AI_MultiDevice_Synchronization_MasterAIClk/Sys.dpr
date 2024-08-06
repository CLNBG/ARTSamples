program Sys;

{$APPTYPE CONSOLE}

uses
  SysUtils,
  Art_DAQ in 'Art_DAQ.pas';

Var
  masterTaskHandle:integer;
  slaveTaskHandle:integer;
  masterData:array[0..999] of double;
  slaveData:array[0..999] of double;
  masterRead:integer;
  slaveRead:integer;
  ret:integer;
  strMasterCardName:string;
  strSlaveCardName:string;
  strMasterChannelName:string;
  strSlaveChannelName:string;
  mode:integer;  // 0:�첽���࿨ͬ��ģʽ  1:ͬ����ͬ��ģʽ
  strMasterExtClock:string;
  strSlaveExtClock:string;
  errbuf:array[0..2047] of char;

Label ExitSys;
Function kbhit() : Integer;stdcall;external 'MSVCRT.DLL' name '_kbhit';
Function getch() : Integer; Stdcall; External'MSVCRT.DLL' name'_getch';

begin
  { TODO -oUser -cConsole Main : Insert code here }
  //*********************************************/
	// ArtDAQ Configure Code
	//*********************************************/
  Writeln('������������DMC��ʶ���������:(��:Dev1)');
  Readln(strMasterCardName);
  Writeln('������ӿ���DMC��ʶ���������:(��:Dev1)');
  Readln(strSlaveCardName);

  // �������豸
  ret := ArtDAQ_CreateTask('', @masterTaskHandle);
  if(ret < 0) then
  begin
    Writeln('�������豸ʧ��!');
    goto ExitSys;
  end;
  strMasterChannelName := strMasterCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(masterTaskHandle, pchar(strMasterChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('�������豸AIʧ��!');
    goto ExitSys;
  end;

  // �������豸
  ret := ArtDAQ_CreateTask('', @slaveTaskHandle);
  if(ret < 0) then
  begin
    Writeln('�������豸ʧ��!');
    goto ExitSys;
  end;
  strSlaveChannelName := strSlaveCardName + '/ai0';
  ret := ArtDAQ_CreateAIVoltageChan(slaveTaskHandle, pchar(strSlaveChannelName), '', ArtDAQ_Val_Cfg_Default, -10.0, 10.0, ArtDAQ_Val_Volts, nil);
  if(ret < 0) then
  begin
    Writeln('�������豸AIʧ��!');
    goto ExitSys;
  end;

  // ���ݿ���������ѡ��ͬ��ģʽ
  // 0:�첽���࿨ͬ��ģʽ  1:ͬ����ͬ��ģʽ
  mode := 0;
  strMasterExtClock := '/' + strMasterCardName + '/PFI3';
  strSlaveExtClock := '/' + strSlaveCardName + '/PFI2';
  if(mode = 0) then    // �첽�࿨
  begin
    // �������ڲ�ʱ��
    ret := ArtDAQ_CfgSampClkTiming(masterTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 10000);
    if(ret < 0) then
    begin
      Writeln('�������ò���ʱ��ʧ��!');
      goto ExitSys;
    end;
    // ��������ת��ʱ��
    ret := ArtDAQ_ExportSignal(masterTaskHandle, ArtDAQ_Val_AIConvertClock, pchar(strMasterExtClock));
    if(ret < 0) then
    begin
      Writeln('��������ת��ʱ��ʧ��!');
      goto ExitSys;
    end;

    // �ӿ����ⲿʱ��
    ret := ArtDAQ_CfgSampClkTiming(slaveTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 10000);
    if(ret < 0) then
    begin
      Writeln('�ӿ����ò���ʱ��ʧ��!');
      goto ExitSys;
    end;
    ret := ArtDAQ_SetAIConvClk(slaveTaskHandle, pchar(strSlaveExtClock), ArtDAQ_Val_Rising);
    if(ret < 0) then
    begin
      Writeln('�ӿ�����ת��ʱ��ʧ��!');
      goto ExitSys;
    end;
  end
  else
  begin    // ͬ���࿨
    // �������ڲ�ʱ��
    ret := ArtDAQ_CfgSampClkTiming(masterTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 10000);
    if(ret < 0) then
    begin
      Writeln('�������ò���ʱ��ʧ��!');
      goto ExitSys;
    end;
    // ��������ת��ʱ��
    ret := ArtDAQ_ExportSignal(masterTaskHandle, ArtDAQ_Val_AIConvertClock, pchar(strMasterExtClock));
    if(ret < 0) then
    begin
      Writeln('��������ת��ʱ��ʧ��!');
      goto ExitSys;
    end;
    // �ӿ����ⲿʱ��
    ret := ArtDAQ_CfgSampClkTiming(slaveTaskHandle, pchar(strSlaveExtClock), 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 10000);
    if(ret < 0) then
    begin
      Writeln('�ӿ����ò���ʱ��ʧ��!');
      goto ExitSys;
    end;
  end;

  //*********************************************/
	// ArtDAQ Start Code
	//*********************************************/
  // �ӿ��ȿ�ʼ
  ret := ArtDAQ_StartTask(slaveTaskHandle);
  if(ret < 0) then
  begin
    Writeln('�ӿ���ʼ�ɼ�ʧ��!');
    goto ExitSys;
  end;
  ret := ArtDAQ_StartTask(masterTaskHandle);
  if(ret < 0) then
  begin
    Writeln('������ʼ�ɼ�ʧ��!');
    goto ExitSys;
  end;

  //*********************************************/
	// ArtDAQ Read Code
	//*********************************************/
  while(kbhit()=0) do
  begin
    ret := ArtDAQ_ReadAnalogF64(masterTaskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @masterData, 1000, @masterRead, nil);
    if(ret < 0) then
    begin
      Writeln('����ʧ��!');
      goto ExitSys;
    end
    else
    begin
      Writeln(Format('Master acquired %d samples',[masterRead]));
    end;

    ret := ArtDAQ_ReadAnalogF64(slaveTaskHandle, 1000, 10.0, ArtDAQ_Val_GroupByChannel, @slaveData, 1000, @slaveRead, nil);
    if(ret < 0) then
    begin
      Writeln('����ʧ��!');
      goto ExitSys;
    end
    else
    begin
      Writeln(Format('Slave acquired %d samples',[slaveRead]));
    end;
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
  if(masterTaskHandle <> 0) then
  begin
     ArtDAQ_StopTask(masterTaskHandle);
     ArtDAQ_ClearTask(masterTaskHandle);
  end;
  if(slaveTaskHandle <> 0) then
  begin
     ArtDAQ_StopTask(slaveTaskHandle);
     ArtDAQ_ClearTask(slaveTaskHandle);
  end;

  Writeln('��Enter���˳�!');
  getch();
  Readln;
  
end.
