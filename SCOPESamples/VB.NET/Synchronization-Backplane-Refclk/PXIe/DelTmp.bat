echo Cleaning unwanted files from the project
echo Make sure this file is placed in the project directory and close VS
echo Begin to clear,please wait a moment......

echo Clean obj files
rd /q/s .\obj
 
echo Clean Debug files
del /q/a/f/s .\*.vshost.exe
del /q/a/f/s .\*.pdb
del /q/a/f/s .\*.xml
del /q/a/f/s .\*.suo

echo Clean bin files
rd /q/s .\bin

pause
 
ECHO File clean up! This program will exit in 3 seconds! Countdown now.........
@echo off 
echo WScript.Sleep 300 > %temp%.\tmp$$$.vbs 
set /a i =3 
:Timeout 
if %i% == 0 goto Next 
setlocal 
set /a i = %i% - 1 
echo Countdown¡­¡­%i% 
cscript //nologo %temp%.\tmp$$$.vbs 
goto Timeout 
goto End 
:Next 
cls & 
echo.