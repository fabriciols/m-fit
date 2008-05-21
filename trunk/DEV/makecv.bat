set PATH=%PATH%;C:\Dev-Cpp\bin
taskkill /IM "MFIT.EXE"
make.exe -f "Makefile.win" clean
make.exe -f "Makefile.win" all