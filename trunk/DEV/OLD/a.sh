#!/bin/bash

# $1 - linha inicial do comment
# $2 - nome do arquivo
function get_comment()
{
	# se tem comentario em cima
	COMMENT=$(sed -n $(expr $1 - 1)p $2 | grep \* | sed s/\r\n/\n/g)

	if [ ! -z "$COMMENT" ] ; then
		j=2
		TMP=""
		while true ; do
			TMP=$(sed -n $(expr $1 - $j)p $2 | grep \* | sed s/\r\n/\n/g)
			[ -z "$TMP" ] && break
			COMMENT="$TMP $COMMENT"
			((j++))
		done
	fi
	
	echo "$COMMENT" | sed "s/\*\* *//g" | sed "s/[*]/\n/g" | sed "s/  */ /g" | sed "s/^ *//" | sed "s/[\/]//g" | sed "s/\n\n*/\n/g"
}

for i in Transition.java ; do
	
	dos2unix $i > /dev/null 2>&1

	class="$(echo $i | cut -d"." -f 1)"
	export FIRST=0

	echo "\\subsubsection{Classe: $class \\label{classe_$class}}"

	LINE_CLASS=$(grep -n "public class $class" $i | cut -d":" -f1)

	get_comment $LINE_CLASS $i

	NUM_FUNC=$(grep "(.*)" "$i" | grep -v \* | wc -l | awk '{print $1}')

	if [ $NUM_FUNC -gt 0 ] ; then
		echo "\\textbf{Métodos:}"
		echo "\\begin{itemize}"
	fi

	grep -n "(.*)" "$i" | grep -i public | while read oper ; do
		LINE=`echo $oper | cut -d":" -f1`
		FUNC=`echo $oper | cut -d":" -f2`


		FUNC=$(echo $FUNC | sed "s/public //g" | sed "s/[{}\n\r]//g")
		echo "	\\item{$FUNC}"

		get_comment $LINE $i

		FUNC=""
	done 

	if [ $NUM_FUNC -gt 0 ] ; then
		echo "\\end{itemize}"
	fi

done


