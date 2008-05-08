cd "C:\Documents and Settings\fabriciols\Desktop\TCC\SRC\LATEX"
set FOLDER=C:\Program Files\MiKTex 2.7\miktex\bin\
"%FOLDER%\pdflatex.exe" "Master.tex"
"%FOLDER%\bibtex.exe" "Master"
"%FOLDER%\pdflatex.exe" "Master.tex"
"%FOLDER%\pdflatex.exe" "Master.tex"
taskkill /IM "FOXITR~1.EXE"
Master.pdf
