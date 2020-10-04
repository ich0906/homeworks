#!/bin/bash
clear

declare -i already=0
declare -i Up_Down_Count=1
declare -i Left_Right=1000
declare -i depth=0
declare -i ypos=5
declare -i file_count=0
declare -a choosed
declare -i numOfChoosed=0

declare make
declare -a cfile
declare -a hfile

make_field(){
for ((i=1;i<27;i++))
do
    for j in 0 40 80
    do
        tput cup $i $j
        echo "|"
    done
done

for i in 1 3 25 27
do
    for((j=0;j<80;j++))
    do
        tput cup $i $j
        printf "-"
    done
done

for i in 2 26
do
    tput cup $i 40
    echo " "
done

tput cup 0 0
echo [46m "FILE EXPLORER" [0m

tput cup 2 1
echo "Current Path: $PWD"
}

show_num_of_C(){
tput cup 26 5
echo "C file : $1"
}

show_num_of_H(){
tput cup 26 27
echo "Header : $1"
}

get_num_of_files(){
declare -i i=0
for var in `ls -a | sort -f`; do
i+=1
done

echo $i
}

print_left(){
declare -i i=4
declare -i numOfDir=-2
declare -i numOfC=0
declare -i numOfH=0
declare -i for_time=1
declare -i input_num=$1
declare -i total_num
declare -i already=0
declare -i initial=0

total_num=$(get_num_of_files)

for var1 in `ls -a | sort -f`
do
  if [ $for_time -ge 20 ]; then
    for_time=20
  elif [ $for_time -ge $total_num ]; then
    for_time=$total_num
  fi
  
  if [ -d $var1 ]; then
    if [ $input_num -eq $for_time ]; then
    cd $var1
    Up_Down_Count=1
    initial=1
    break
    fi
  
  if [ `expr $Left_Right % 2` = "0" ]; then
    if [ $for_time -eq $Up_Down_Count ]; then
      echo [47m
    else
      echo [0m
    fi
  fi

  acc1=`stat -c%A $var1`
  size1=`stat -c%s $var1`

  tput cup $i 1
  echo [34m "$var1                 "

  tput cup $i 20
  echo "$acc1          "
 
  if [ $for_time -eq 1 ]; then
    tput cup $i 33
    echo "-"[0m

  elif [ $for_time -eq 2 ]; then
    tput cup $i 33
    echo "-"[0m
  else
    tput cup $i 33
    echo "$size1 " [0m
  fi
  i+=1
  numOfDir+=1
  for_time+=1
  fi

  if [ $i -ge 24 ];then
  break
  fi
done

for var2 in `ls -a | sort -f`
do
  if [ $for_time -ge 20 ]; then
    for_time=20
  elif [ $for_time -ge $total_num ]; then
    for_time=$total_num 
  fi

  if [ `echo $var2 | rev | cut -c 1-2 | rev` = ".c" ]; then
    if [ "$input_num" = "$for_time" ]; then
      if [ $initial -eq 0 ]; then
	for((i=0;i<$numOfChoosed;i++)); do
	  if [ "${choosed[$i]}" = "$var2" ]; then
	  already=1
	  fi
	done

	if [ "$already" = "0" ]; then
	choosed[$numOfChoosed]=$var2
	numOfChoosed+=1
	Left_Right+=1
	Up_Down_Count=1
	else
	already=0
	fi
	fi
    fi
    
    if [ `expr $Left_Right % 2` = "0" ]; then
        if [ $for_time -eq $Up_Down_Count ]; then
          echo [47m
        else
          echo [0m
        fi
    fi

    acc2=`stat -c%A $var2`
    size2=`stat -c%s $var2`

    tput cup $i 1
    echo [32m "$var2                "

    tput cup $i 20
    echo "$acc2           "

    tput cup $i 33
    echo "$size2" [0m

    i+=1
    numOfC+=1
    for_time+=1

  fi

  if [ $i -ge 24 ];then
  break
  fi
done

for var3 in `ls -a | sort -f`
do
  if [ $for_time -ge 20 ]; then
    for_time=20
  elif [ $for_time -ge $total_num ]; then
    for_time=$total_num
  fi

  if [ `echo $var3 | rev | cut -c 1-2 | rev` = ".h" ]; then
    if [ $initial -eq 0 ]; then
      if [ $input_num -eq $for_time ]; then
        for((i=0;i<$numOfChoosed;i++)); do
          if [ "${choosed[$i]}" = "$var3" ]; then
          already=1
          fi
        done

        if [ "$already" = "0" ]; then
        choosed[$numOfChoosed]=$var3
        numOfChoosed+=1
	Left_Right+=1
        Up_Down_Count=1
	else
        already=0
        fi
      fi
    initial=0
    fi

    if [ `expr $Left_Right % 2` = "0" ]; then
      if [ $for_time -eq $Up_Down_Count ]; then
        echo [47m
      else
        echo [0m
      fi
    fi
    acc3=`stat -c%A $var3`
    size3=`stat -c%s $var3`

    tput cup $i 1
    echo [30m "$var3                "

    tput cup $i 20
    echo "$acc3          "

    tput cup $i 33
    echo "$size3" [0m

    i+=1
    numOfH+=1
    for_time+=1
  fi

  if [ $i -ge 24 ];then
  break
  fi
done

show_num_of_H $numOfH
show_num_of_C $numOfC
}

print_right(){
declare -i ypos=4
declare -i for_time=1

for  ((i=0;i<$numOfChoosed;i++)); do #main파일 출력
	string=`find . -name "${choosed[$i]}" -exec grep "int main" {} \;`
	if [ "$string" != "" ]; then
	make=${choosed[$i]}
		if [ "$for_time" = "$Up_Down_Count" ]; then
			if [ `expr $Left_Right % 2` = "1" ]; then
				tput cup $ypos 42
				echo [31m[47m"${choosed[$i]}                          "[0m
			else
				tput cup $ypos 42
				echo [31m"${choosed[$i]}"[0m
			fi
		else
			tput cup $ypos 42
			echo [31m"${choosed[$i]}"[0m
		fi
	ypos+=1
	for_time+=1
	fi
done

for  ((i=0;i<$numOfChoosed;i++)); do #.c파일 출력
	if [ `echo ${choosed[$i]} | rev | cut -c 1-2 | rev` = ".c" ]; then
	string2=`find . -name "${choosed[$i]}" -exec grep "int main" {} \;`
		if [ "$string2" = "" ]; then
			if [ "$for_time" = "$Up_Down_Count" ]; then
				if [ `expr $Left_Right % 2` = "1" ]; then
				tput cup $ypos 42
				echo [32m[47m"${choosed[$i]}			  " [0m
				else
				tput cup $ypos 42
				echo [32m"${choosed[$i]}" [0m
				fi
			else
				tput cup $ypos 42
				echo [32m"${choosed[$i]}" [0m
			fi
		ypos+=1
		for_time+=1
		fi
	fi
done

for((i=0;i<$numOfChoosed;i++)); do #.h파일 출력
	if [ `echo ${choosed[$i]} | rev | cut -c 1-2 | rev` = ".h" ]; then
                if [ "$for_time" = "$Up_Down_Count" ]; then
			if [ `expr $Left_Right % 2` = "1" ]; then
                        tput cup $ypos 42
                        echo [47m[30m"${choosed[$i]}			  " [0m
			else
			tput cup $ypos 42
			echo [30m"${choosed[$i]}" [0m
			fi
                else    
                        tput cup $ypos 42
                        echo [30m"${choosed[$i]}" [0m
                fi
        ypos+=1
        for_time+=1
        fi

done

}

##################################################################################

print_left

while true ; do

declare -i num_of_files=$(get_num_of_files)+2
declare -i nof=${num_of_files}-2
make_field

read -s -n 1 key
if [ "$key" = "" ]; then
  if [ `expr $Left_Right % 2` = "0" ]; then
    print_left $Up_Down_Count
    clear
    print_left
    
    print_right
  else
    clear
    print_left
    print_right
  fi
elif [ "$key" = 'q' ]; then
  break

elif [ "$key" = "e" ]; then
tput cup 25 40
echo "$make"
elif [ "$key" = "m" ]; then
	if [ "$make" != "" ]; then
		gcc -c "${choosed[@]}"
		arr=(`find . -maxdepth 1 -name "*.o" | cut -c 3-`)
	
		echo "2017203061.out : ${arr[@]}" >> makefile
		echo "	gcc ${arr[@]} -o 2017203061.out" >> makefile
		echo "clean :" >> makefile
		echo "	rm -f *.out *.o *.gch" >> makefile
		
		make clean
		exit
	fi

elif [ "$key" = '' ]; then
  read -s -n 1 key
  read -s -n 1 key
  
  
  if [ "$key" = 'A' ]; then
    if [ `expr $Left_Right % 2` = "0" ]; then
      if [ $Up_Down_Count -eq 1 ]; then
        Up_Down_Count=`expr $num_of_files - 2`
        clear
        print_left
	print_right
      else
        Up_Down_Count=${Up_Down_Count}-1
        clear
        print_left
	print_right
      fi 
    elif [ `expr $Left_Right % 2` = "1" ]; then
      if [ $Up_Down_Count -eq 1 ]; then
        Up_Down_Count=$numOfChoosed
        clear
        print_left
	print_right
      else
        Up_Down_Count=${Up_Down_Count}-1
        clear
        print_left
	print_right
      fi       
    fi

  elif [ "$key" = 'B' ]; then
    if [ `expr $Left_Right % 2` = "0" ]; then  
      if [ $Up_Down_Count -lt $nof ]; then
        if [ $Up_Down_Count -ge 20 ];then
          Up_Down_Count=20
        else
          Up_Down_Count+=1
        fi
      else
        Up_Down_Count=1
      fi 
        clear
        print_left
	print_right
      
    elif [ `expr $Left_Right % 2` = "1" ]; then
	if [ $Up_Down_Count -ge $numOfChoosed ];then
          Up_Down_Count=1
        else
          Up_Down_Count+=1
        fi

	clear
        print_left
	print_right
    fi

  elif [ "$key" = 'C' ]; then
  Left_Right=`expr $Left_Right + 1`

  if [ "$numOfChoosed" != "0" ]; then
    if [ $Up_Down_Count -ge $numOfChoosed ]; then
    Up_Down_Count=$numOfChoosed
    fi
  fi

  clear
  print_left
  print_right

  elif [ "$key" = 'D' ]; then
  Left_Right=`expr $Left_Right - 1`
  
  if [ "$numOfChoosed" != "0" ]; then
    if [ $Up_Down_Count -ge $numOfChoosed ]; then
    Up_Down_Count=$numOfChoosed
    fi
  fi

  clear
  print_left
  print_right
  fi
fi
done
