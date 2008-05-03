set PATH=%PATH%;C:\Dev-Cpp\bin
taskkill /IM "MFIT.EXE"
make.exe -f "C:\Documents and Settings\fabriciols\Desktop\TCC\SRC\CV\Makefile.win" clean
make.exe -f "C:\Documents and Settings\fabriciols\Desktop\TCC\SRC\CV\Makefile.win" all