set PATH=%PATH%;C:\Dev-Cpp\bin;c:\Qt\4.3.4\bin\
set MAKE=make
taskkill /IM "MFIT.EXE"

qmake
%MAKE%
