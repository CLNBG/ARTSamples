rd /q/s debug
rd /q/s release

echo 清理Debug文件
del /q/a/f/s .\*.stash
del /q/a/f/s .\*.Debug
del /q/a/f/s .\*.Release
del /q/a/f/s .\Makefile
