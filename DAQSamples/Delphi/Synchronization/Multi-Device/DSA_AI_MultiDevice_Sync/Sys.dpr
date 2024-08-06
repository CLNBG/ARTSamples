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
  synchronizationType:integer;  // 0-����ʱ�� 1-�ο�ʱ��
  strMasterCardName:string;
  strSlaveCardName:string;
  strMasterChannelName:string;
  strSlaveChannelName:string;
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
  ret := ArtDAQ_CfgSampClkTiming(masterTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('�������ò���ʱ��ʧ��!');
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
  ret := ArtDAQ_CfgSampClkTiming(slaveTaskHandle, '', 10000.0, ArtDAQ_Val_Rising, ArtDAQ_Val_ContSamps, 1000);
  if(ret < 0) then
  begin
    Writeln('�ӿ����ò���ʱ��ʧ��!');
    goto ExitSys;
  end;

  synchronizationType := 0;
  if(synchronizationType = 0) then
  begin
     // ѡ��ο�ʱ��ͬ�������ӿ�����Ҫ�Ӱ忨ָ������(PCI��ָ��RTSI9��PXI��ָ��PXI_10M)
     ret := ArtDAQ_SetRefClkSrc(masterTaskHandle, 'RTSI9');
     if(ret < 0) then
     begin
      Writeln('���������ο�ʱ��ʧ��!');
      goto ExitSys;
     end;
     ret := ArtDAQ_SetRefClkSrc(slaveTaskHandle, 'RTSI9');
     if(ret < 0) then
     begin
      Writeln('���ôӿ��ο�ʱ��ʧ��!');
      goto ExitSys;
     end;
  end
  else
  begin
     // ��������������ʱ��ʱ�����ӿ��������ʱ��ʱ��
     ret := ArtDAQ_SetExportedSampClkTimebaseOutputTerm(masterTaskHandle, 'RTSI8');
     if(ret < 0) then
     begin
      Writeln('������������ʱ��ʧ��!');
      goto ExitSys;
     end;
     ret := ArtDAQ_SetSampClkTimebaseSrc(slaveTaskHandle, 'RTSI8');
     if(ret < 0) then
     begin
      Writeln('�ӿ����ò���ʱ��ʧ��!');
      goto ExitSys;
     end;
  end;

  // �������ͬ�����壬�ӿ�����ͬ�����壬����ͬ��DDC
  ret := ArtDAQ_SetExportedSyncPulseEventOutputTerm(masterTaskHandle, 'RTSI0');
  if(ret < 0) then
  begin
    Writeln('�������ͬ������ʧ��!');
    goto ExitSys;
  end;
  ret := ArtDAQ_SetSyncPulseSrc(slaveTaskHandle, 'RTSI0');
  if(ret < 0) then
  begin
    Writeln('�ӿ�����ͬ������ʧ��!');
    goto ExitSys;
  end;

  // ��������ѡ���ⲿ��������������������Ҳ���Բ����ô����������ڲ��Ĵ����ź���ͬ���ɼ�
  //ArtDAQErrChk (ArtDAQ_CfgDigEdgeStartTrig(gMasterAItaskHandle,"PFI0",ArtDAQ_Val_Rising));
  ret := ArtDAQ_ExportSignal(masterTaskHandle, ArtDAQ_Val_StartTrigger, 'RTSI1');
  if(ret < 0) then
  begin
    Writeln('������������ʧ��!');
    goto ExitSys;
  end;
  ret := ArtDAQ_CfgDigEdgeStartTrig(slaveTaskHandle, 'RTSI1', ArtDAQ_Val_Rising);
  if(ret < 0) then
  begin
    Writeln('�ӿ����ô���ʧ��!');
    goto ExitSys;
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
