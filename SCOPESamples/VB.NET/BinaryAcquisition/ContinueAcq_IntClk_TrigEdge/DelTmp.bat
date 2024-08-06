echo 正在清理工程中不需要的文件
echo 请确保本文件放置在工程目录之中并关闭VS
echo 开始清理请稍等......

echo 清理obj文件
rd /q/s .\obj
 
echo 清理Debug文件
del /q/a/f/s .\*.vshost.exe
del /q/a/f/s .\*.pdb
del /q/a/f/s .\*.xml
del /q/a/f/s .\*.suo

echo 清理bin文件
rd /q/s .\bin

pause
 
ECHO 文件清理完毕！本程序将在3秒后退出！现在进入倒计时......... 
@echo off 
echo WScript.Sleep 300 > %temp%.\tmp$$$.vbs 
set /a i =3 
:Timeout 
if %i% == 0 goto Next 
setlocal 
set /a i = %i% - 1 
echo 倒计时……%i% 
cscript //nologo %temp%.\tmp$$$.vbs 
goto Timeout 
goto End 
:Next 
cls & 
echo.