@echo off
del *.exp
del *.bsc
del *.obj
del *.res
del *.sbr
del *.idb
del *.pch
del *.aps
del *.opt
del *.ilk
del *.ncb
del *.log
del *err.txt
del *.pdb
del *.plg

del *.exe
del *.sdf
del *.sln
del /a:H *.suo
del *.user
del *.filters

del debug\*.* /q

del release\*.* /q

rd debug

rd release
