set PATH=%PATH%;C:\Dev-Cpp\bin
set MAKE=mingw32-make.exe
taskkill /IM "MFIT.EXE"
%MAKE% -f "Makefile.win" clean
%MAKE% -f "Makefile.win" all
