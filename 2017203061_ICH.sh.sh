#!/bin/bash
clear

declare -i Up_Down_Count=1
declare -i Left_Right=0
declare -i depth=0
declare -i ypos=5
declare -i file_count=0

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

show_num_of_Dir(){
tput cup 26 5
echo "Directory : $1"
}

show_num_of_File(){
tput cup 26 27
echo "File : $1"
}

show_Dir_size(){
tput cup 26 42
echo "Current Directory size : $1"
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
declare -i numOfFile=0
declare -i total_size=0
declare -i for_time=1
declare -i input_num=$1
declare -i total_num
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
    break
    fi
  
  if [ $Left_Right -eq 0 ]; then
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
    total_size+=$size1
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

  if [ -f $var2 ]; then
    if [ -x $var2 ]; then
   
    if [ $Left_Right -eq 0 ]; then 
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
    numOfFile+=1
    total_size+=$size2
    for_time+=1
    fi
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

  if [ -f $var3 ]; then
    if [ -x $var3 ]; then 
    a=1
    else
    
    if [ $Left_Right -eq 0 ]; then
      if [ $for_time -eq $Up_Down_Count ]; then
        echo [47m
      else
        echo [0m
      fi
    fi

    acc3=`stat -c%A $var3`
    size3=`stat -c%s $var3`

    tput cup $i 1
    echo [31m "$var3                "

    tput cup $i 20
    echo "$acc3          "

    tput cup $i 33
    echo "$size3" [0m

    i+=1
    numOfFile+=1
    total_size+=$size3
    for_time+=1
    fi
  fi

  if [ $i -ge 24 ];then
  break
  fi
done

show_num_of_Dir $numOfDir
show_num_of_File $numOfFile
show_Dir_size $total_size
}

declare -i call_time=1

print_sub(){

if [ $call_time -gt 20 ];then
call_time=20
fi

for var in `ls -a | sort -f`; do

if [ $file_count -ge 20 ];then
break
fi

if [ $var = '.' ]; then
continue
fi

if [ $var = ".." ]; then
continue
fi

if [ -d $var ]; then
tput cup $ypos 41
for ((i=0;i<$depth;i++)); do
echo -n "│  "
done

if [ $Left_Right -eq 1 ]; then
  if [ $call_time -eq $Up_Down_Count ]; then
    echo "├"[34m "$var" [32m "<--" [0m
  else
    echo "├"[34m "$var" [0m
  fi
else
  echo "├"[34m "$var" [0m
fi

ypos+=1
file_count+=1
call_time+=1

cd $var
depth=${depth}+1
print_sub

elif [ -f $var ]; then
  if [ -x $var ]; then
    tput cup $ypos 41
    for ((i=0;i<$depth;i++)); do
    echo -n "│  "
    done
    
    if [ $Left_Right -eq 1 ]; then
      if [ $call_time -eq $Up_Down_Count ]; then
        echo "├"[32m "$var <--" [0m
      else
        echo "├"[32m "$var" [0m
      fi
    else
      echo "├"[32m "$var" [0m
    fi
    
    ypos+=1
    file_count+=1
    call_time+=1
  else
    tput cup $ypos 41
    for ((i=0;i<$depth;i++)); do
    echo -n "│  "
    done

    if [ $Left_Right -eq 1 ]; then
      if [ $call_time -eq $Up_Down_Count ]; then
        echo "├"[31m "$var" [32m "<--" [0m
      else
        echo "├"[31m "$var" [0m
      fi
    else
      echo "├"[31m "$var" [0m
    fi

    ypos+=1
    file_count+=1
    call_time+=1
  fi
fi
done
if [ $depth -ne 0 ]; then
cd ..
depth=${depth}-1
fi
}

print_right(){
tput cup 4 40
echo [34m ${PWD##*/} [0m

print_sub
ypos=5
file_count=0
call_time=1
}
##################################################################################

print_left
print_right

while true ; do

declare -i num_of_files=$(get_num_of_files)+2
declare -i nof=${num_of_files}-2
make_field

read -s -n 1 key
if [ "$key" = "" ]; then
  if [ $Left_Right -eq 0 ]; then
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

elif [ "$key" = '' ]; then
  read -s -n 1 key
  read -s -n 1 key
  
  
  if [ "$key" = 'A' ]; then
    if [ $Left_Right -eq 0 ]; then
      if [ $Up_Down_Count -eq 1 ]; then
        Up_Down_Count=1
        clear
        print_left
        print_right
      else
        Up_Down_Count=${Up_Down_Count}-1
        clear
        print_left
        print_right
      fi 
    elif [ $Left_Right -eq 1 ]; then
      if [ $Up_Down_Count -eq 1 ]; then
        Up_Down_Count=1
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
    if [ $Left_Right -eq 0 ]; then  
      if [ $Up_Down_Count -lt $nof ]; then
        if [ $Up_Down_Count -ge 20 ];then
          Up_Down_Count=20
        else
          Up_Down_Count+=1
        fi
      else
        Up_Down_Count=$nof
      fi 
      clear
      print_left
      print_right
      
    elif [ $Left_Right -eq 1 ]; then
	if [ $Up_Down_Count -ge 20 ];then
          Up_Down_Count=20
        else
          Up_Down_Count+=1
        fi

	clear
        print_left
        print_right
  fi

  elif [ "$key" = 'C' ]; then
  Left_Right=1
  Up_Down_Count=1
  clear
  print_left
  print_right


  elif [ "$key" = 'D' ]; then
  Left_Right=0
  Up_Down_Count=1
  clear
  print_left
  print_right
  fi
fi
done
