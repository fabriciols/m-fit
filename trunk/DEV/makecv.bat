set PATH=%PATH%;C:\Mingw\bin;c:\Qt\4.3.3\bin\
set MAKE=make
taskkill /IM "MFIT.EXE"

qmake
%MAKE%
