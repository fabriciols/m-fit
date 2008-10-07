set PATH=%PATH%;C:\Mingw\bin;c:\Qt\4.4.2\bin\;C:\Program Files\Vim\vim71
set MAKE=make
taskkill /IM "MFIT.EXE"

qmake
%MAKE%
