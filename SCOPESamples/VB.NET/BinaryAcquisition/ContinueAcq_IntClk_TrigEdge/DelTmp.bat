echo �����������в���Ҫ���ļ�
echo ��ȷ�����ļ������ڹ���Ŀ¼֮�в��ر�VS
echo ��ʼ�������Ե�......

echo ����obj�ļ�
rd /q/s .\obj
 
echo ����Debug�ļ�
del /q/a/f/s .\*.vshost.exe
del /q/a/f/s .\*.pdb
del /q/a/f/s .\*.xml
del /q/a/f/s .\*.suo

echo ����bin�ļ�
rd /q/s .\bin

pause
 
ECHO �ļ�������ϣ���������3����˳������ڽ��뵹��ʱ......... 
@echo off 
echo WScript.Sleep 300 > %temp%.\tmp$$$.vbs 
set /a i =3 
:Timeout 
if %i% == 0 goto Next 
setlocal 
set /a i = %i% - 1 
echo ����ʱ����%i% 
cscript //nologo %temp%.\tmp$$$.vbs 
goto Timeout 
goto End 
:Next 
cls & 
echo.