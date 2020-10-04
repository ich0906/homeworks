#!bin/bash

clear

touch ${PWD}/ids.txt
touch ${PWD}/pwds.txt
touch ${PWD}/msg.txt
touch ${PWD}/trash.txt
touch ${PWD}/empty.txt
touch ${PWD}/sorted.txt

if [ "`whoami`" = "${PWD##*/}" ]; then 
chmod 777 ids.txt
chmod 777 pwds.txt
chmod 777 msg.txt
chmod 777 trash.txt
chmod 777 empty.txt
chmod 777 sorted.txt
fi

clear

declare -i k=0
declare -i sign_in_or_up=0	#sign_in sign_up 선택
declare -i errors=0		#sign_in error(id,pw오류)
declare -i Yes_or_No=0		
declare -a IDS			#id저장 배열
declare -i logined_num		#id배열에서 현재 로그인된 인덱스번호
declare -a PWDS			#pw저장 배열
declare -i num_of_ms=0		#저장된 메시지 개수
declare -i is_overlap=0		
declare -i num_of_id=0		#저장된 id 개수
declare -i is_login=0		#로그인 여부
declare -i choose_after_login=-1	#로그인 이후 메뉴 선택
declare -i send_mail=0		#send mail에서 id선택
declare -i read_mail=0		#read mail에서 선택
declare -a press_s		#s를 누른 위치 저장
declare -i ps_size=0		#press_s의 size
declare -i al_sel=0		#s로 고른 id 엔터 눌렀을때
declare -a MSGS			#메시지 저장
declare -i sended=0
declare -i trash=0
declare -a check

IDS=(`cat ${PWD}/ids.txt`)
PWDS=(`cat ${PWD}/pwds.txt`)
MSGS=(`cat ${PWD}/msg.txt | sort -r`)
TRA=(`cat ${PWD}/trash.txt | sort -r`)

declare -i check_size="${#IDS[@]}"

for((i=0;i<$check_size;i++)); do
check[$i]=0
done

num_of_id=${#IDS[@]}
num_of_ms=`expr ${#MSGS[@]} / 7`
tra_size=`expr ${#TRA[@]} / 7`

bring_info(){
IDS=(`cat ${PWD}/ids.txt`)
PWDS=(`cat ${PWD}/pwds.txt`)
MSGS=(`cat ${PWD}/msg.txt | sort -r`)
TRA=(`cat ${PWD}/trash.txt | sort -r`)
num_of_id=${#IDS[@]}
num_of_ms=`expr ${#MSGS[@]} / 7`
tra_size=`expr ${#TRA[@]} / 7`
check_size="${#IDS[@]}"
}

show_table(){
  for ((i=0;i<17;i++))
  do
    tput cup $i 0
    echo "|"

    tput cup $i 40
    echo "|"
  done

  for ((i=0;i<41;i++))
  do
    tput cup 0 $i
    printf "-"

    tput cup 2 $i
    printf "-"

    tput cup 3 $i
    printf "-"

    tput cup 16 $i
    printf "-"
  done
}

show_table2(){
  for ((i=0;i<20;i++))
  do
    tput cup $i 0
    echo "|"

    tput cup $i 44
    echo "|"
  done

  for ((i=0;i<45;i++))
  do
    tput cup 0 $i
    printf "-"

    tput cup 2 $i
    printf "-"

    tput cup 4 $i
    printf "-"

    tput cup 19 $i
    printf "-"
  done

}

send_mail_table(){
tput cup 0 0
echo "-----------------------------------"

tput cup 2 0
echo "-----------------------------------"

tput cup 4 0
echo "-----------------------------------"
}

show_main(){
bring_info
show_table
  tput cup 1 16
  echo "MAIL BOX"

  if [ $sign_in_or_up -eq 0 ]; then
  tput cup 6 15
  echo ">Sign in"

  else
  tput cup 6 16
  echo "Sign in"
  fi

  if [ $sign_in_or_up -eq 1 ]; then
  tput cup 8 15
  echo ">Sign up"

  else
  tput cup 8 16
  echo "Sign up"
  fi

}

show_sign_up(){
clear
  while true; do
  show_table

  tput cup 1 16
  echo "MAIL BOX"

  tput cup 5 3
  read -p "Input ID:" id
  
  for ((i=0;i<$num_of_id;i++)); do
  if [ ${IDS[$i]} = $id ]; then
  
  clear
  tput cup 6 10
  echo "[Already defined id!]"
  
  is_overlap=1
  fi
  done

  if [ $is_overlap -eq 1 ]; then
  is_overlap=0
  continue;
  fi  
  
  tput cup 7 3
  read -sp "Input Password:" pw

  tput cup 9 3
  read -sp "Confirm Password:" temp
  
  if [ "$pw" = "$temp" ]; then
  break
  else
  clear
  fi
  
  done

  tput cup 11 15
  echo "*sign up?*"
  
  tput cup 13 14
  echo ">"

  tput cup 13 15
  echo "Yes"

  tput cup 13 22
  echo "No"

  while true; do

  read -s -n 1 key
  if [ "$key" = '' ]; then
    read -s -n 2 key
    if [ "$key" = '[C' ]; then
    Yes_or_No=1

    tput cup 13 14
    echo " "

    tput cup 13 21
    echo ">"

    elif [ "$key" = '[D' ]; then
    Yes_or_No=0

    tput cup 13 14
    echo ">"
  
    tput cup 13 21
    echo " "

    fi
  elif [ "$key" = "" ]; then
    if [ $Yes_or_No -eq 0 ]; then
    IDS[$num_of_id]="$id"
    PWDS[$num_of_id]="$pw"
    echo "$id" >> ${PWD}/ids.txt
    echo "$pw" >> ${PWD}/pwds.txt
    IDS=(`cat ${PWD}/ids.txt`)
    PWDS=(`cat ${PWD}/pwds.txt`)
    check_size=${#IDS[@]}
    check[`expr $check_size - 1`]="0"
    return

    elif [ $Yes_or_No -eq 1 ]; then
    clear
    Yes_or_No=0
    show_sign_up
    return
    fi
  fi
  done
}

show_sign_in(){
clear
bring_info
  show_table
  tput cup 1 16
  echo "MAIL BOX"

  while [ $is_login -eq 0 ]; do

  tput cup 6 3
  read -p "ID: " id
 
  for ((i=0;i<$num_of_id;i++)); do
  if [ "${IDS[$i]}" = "$id" ]; then
    num=$num_of_id
    tput cup 8 3
    read -sp "password: " pw

    if [ "${PWDS[$i]}" = "$pw" ]; then
      is_login=1
      logined_num=$i
    else
    errors=1
    fi
  fi
  done

  if [ $errors -eq 0 ]; then
  clear
  show_table
  tput cup 1 16
  echo "MAIL BOX"

  tput cup 10 14
  echo "Undefined ID!"
  
  elif [ $errors -eq 1 ]; then
  clear
  show_table
  tput cup 1 16
  echo "MAIL BOX"

  tput cup 10 12
  echo "Wrong Password!"
  errors=0
  
  fi
  done

  IDS=(`cat ${PWD}/ids.txt`)
  PWDS=(`cat ${PWD}/pwds.txt`)

  num_of_id=${#IDS[@]}
}

show_logined_menu(){
clear
show_table2

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

if [ $choose_after_login -eq 0 ]; then
tput cup 6 17
echo ">read mail"

else
tput cup 6 18
echo "read mail"
fi

if [ $choose_after_login -eq 1 ]; then
tput cup 8 17
echo ">send mail"

else
tput cup 8 18
echo "send mail"
fi

if [ $choose_after_login -eq 2 ]; then
tput cup 10 17
echo ">sended"

else
tput cup 10 18
echo "sended"
fi

if [ $choose_after_login -eq 3 ]; then
tput cup 12 17
echo ">search"

else
tput cup 12 18
echo "search"
fi

if [ $choose_after_login -eq 4 ]; then
tput cup 14 17
echo ">trash can"

else
tput cup 14 18
echo "trash can"
fi

if [ $choose_after_login -eq 5 ]; then
tput cup 16 17
echo ">exit"

else
tput cup 16 18
echo "exit"
fi
}

make_sorted_msg(){
cp -f /home/lch/empty.txt /home/lch/sorted.txt
echo "`cat ${PWD}/msg.txt | sort -r`" >> ${PWD}/sorted.txt
cp -f ${PWD}/sorted.txt ${PWD}/msg.txt
}

make_sorted_trash(){
cp -f ${PWD}/empty.txt ${PWD}/sorted.txt
echo "`cat ${PWD}/trash.txt | sort -r`" >> ${PWD}/sorted.txt
cp -f ${PWD}/sorted.txt ${PWD}/trash.txt
}

clean_trash(){
declare num=0
while read line1; do
  num=`expr $num + 1`
  while read line2; do
  if [ "$line1" = "$line2" ]; then
    sed -i "${num}s/.*//g" ${PWD}/msg.txt
    make_sorted_msg
  fi
  done < ${PWD}/trash.txt
done < ${PWD}/msg.txt
}

show_read_mail_sub(){
clear
show_table2

make_sorted_msg
make_sorted_trash

bring_info

MSGS=(`cat ${PWD}/msg.txt | sort -r`)
num_of_ms=`expr ${#MSGS[@]} / 7`

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

if [ $read_mail -eq 0 ]; then

  tput cup 5 4
  echo ">"[31m"prev"[0m

else
  tput cup 5 5
  echo [31m"prev"[0m
fi

declare -i coun=0
while read line; do
arr=($line)
if [ `expr $logined_num + 1` = "${arr[1]}" ]; then
  coun+=1
  if [ "${arr[6]}" = "0" ]; then
    if [ "$coun" = "$read_mail" ]; then

    tput cup `expr $coun + 5` 4
    echo [32m">${arr[4]}"

    else
    
    tput cup `expr $coun + 5` 5
    echo [32m"${arr[4]}"

    fi

  tput cup `expr $coun + 5` 15
  echo [32m"-${IDS[${arr[0]}]}"

  tput cup `expr $coun + 5` 20
  echo "${arr[2]}"

  tput cup `expr $coun + 5` 31
  echo "${arr[3]}"[0m
  
  elif [ "${arr[6]}" = "1" ]; then

    if [ "$coun" = "$read_mail" ]; then

    tput cup `expr $coun + 5` 4
    echo [0m">${arr[4]}"

    else

    tput cup `expr $coun + 5` 5
    echo [0m"${arr[4]}"

    fi

  tput cup `expr $coun + 5` 15
  echo "-${IDS[${arr[0]}]}"

  tput cup `expr $coun + 5` 20
  echo "${arr[2]}"

  tput cup `expr $coun + 5` 31
  echo "${arr[3]}"
  fi
fi
done < ${PWD}/msg.txt
}

show_read_mail2(){
clear
send_mail_table

make_sorted_msg
make_sorted_trash

MSGS=(`cat ${PWD}/msg.txt | sort -r`)
num_of_ms=`expr ${#MSGS[@]} / 6`

declare -i count=0
while read line; do
arr=($line)
if [ "${arr[1]}" = `expr $logined_num + 1` ]; then
  count+=1
  if [ "$count" = "$read_mail" ]; then
    tput cup 1 0
    echo -n "from. ${IDS[${arr[0]}]}"

    tput cup 3 0
    echo "title: ${arr[4]}"

    tput cup 5 0
    echo "content:"

    tput cup 6 0
    echo "${arr[5]}"

    if [ "${arr[6]}" = "0" ]; then
    sed -i "${count}s/.*/${arr[0]} ${arr[1]} ${arr[2]} ${arr[3]} ${arr[4]} ${arr[5]} 1/g" ${PWD}/msg.txt
    fi
  fi
fi
done < ${PWD}/msg.txt

while true; do
read -s -n 1 key
if [ "$key" = "" ]; then
  clear
  show_read_mail_sub
  break
fi
done

}

show_read_mail(){
show_read_mail_sub

bring_info
while true; do

read -s -n 1 key

if [ "$key" = '' ]; then
read -s -n 2 key
  if [ "$key" = '[A' ]; then
    if [ $read_mail -eq 0 ]; then
    read_mail=0
    bring_info
    show_read_mail_sub
    else
    bring_info
    read_mail=${read_mail}-1
    show_read_mail_sub
    fi
  elif [ "$key" = '[B' ]; then
    if [ $read_mail -eq $num_of_ms ]; then
    bring_info
    read_mail=$num_of_ms
    show_read_mail_sub
    else
    bring_info
    read_mail+=1
    show_read_mail_sub
    fi
  fi
elif [ "$key" = "" ]; then
  if [ "$read_mail" = "0" ]; then
  bring_info
  show_logined_menu
  break;

  else 
  show_read_mail2
  fi
elif [ "$key" = "d" ]; then
  declare -i count=0
  while read line; do
  arr=($line)
  if [ "${arr[1]}" = `expr $logined_num + 1` ]; then
    count+=1
    if [ "$count" = "$read_mail" ]; then
    echo "$line" >> ${PWD}/trash.txt
    make_sorted_msg
    make_sorted_trash

    fi
  fi
  done < ${PWD}/msg.txt
  clean_trash 				#clean: trash와 msg 겹치는것 삭제
  make_sorted_msg
  make_sorted_trash
  send_mail=0
  show_read_mail_sub
fi

done
}

show_send_mail1_sub(){
clear
show_table2

make_sorted_msg
make_sorted_trash

IDS=(`cat ${PWD}/ids.txt`)
PWDS=(`cat ${PWD}/pwds.txt`)

num_of_id=${#IDS[@]}
check_size=${#IDS[@]}

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

tput cup 5 3
echo "who?"
#########################################
if [ $send_mail -eq 0 ]; then

  tput cup 6 4
  echo ">"[31m"prev"[0m

else
  tput cup 6 5
  echo [31m"prev"[0m
fi

for ((i=0;i<$check_size;i++)); do
  if [ "$send_mail" = `expr $i + 1` ]; then
    if [ "${check[$i]}" = "0" ]; then
      tput cup `expr $i + 7` 4
      echo ">${IDS[$i]}"

    elif [ "${check[$i]}" = "1" ]; then
      tput cup `expr $i + 7` 4
      echo ">${IDS[$i]}"[31m"*"[0m
    fi
  else
    if [ "${check[$i]}" = "0" ]; then
      tput cup `expr $i + 7` 5
      echo "${IDS[$i]}"

    elif [ "${check[$i]}" = "1" ]; then
      tput cup `expr $i + 7` 5
      echo "${IDS[$i]}"[31m"*"[0m
    fi
  fi
done
#########################################
}

show_send_mail1(){
show_send_mail1_sub
while true; do
declare -i already_sel=0
check_size=${#IDS[@]}
read -s -n 1 key

if [ "$key" = '' ]; then
read -s -n 2 key
  if [ "$key" = '[A' ]; then
    if [ $send_mail -eq 0 ]; then
    send_mail=0
    show_send_mail1_sub
    bring_info
    else
    bring_info
    send_mail=${send_mail}-1
    show_send_mail1_sub
    fi
  elif [ "$key" = '[B' ]; then
    if [ $send_mail -eq $num_of_id ]; then
    send_mail=$num_of_id
    show_send_mail1_sub
    bring_info
    else
    bring_info
    send_mail+=1
    show_send_mail1_sub
    fi
  fi
elif [ "$key" = "" ]; then
  if [ $send_mail -eq 0 ]; then
  clear
  bring_info
  show_table2
  show_logined_menu
  return

  else
    bring_info
    check[`expr $send_mail - 1`]="1"
    show_table2
    show_send_mail2
  fi
elif [ "$key" = "s" ]; then
    bring_info
    if [ "${check[`expr $send_mail - 1`]}" = "0" ]; then
    check[`expr $send_mail - 1`]="1"
    
    elif [ "${check[`expr $send_mail - 1`]}" = "1" ]; then
    check[`expr $send_mail - 1`]="0"
    fi
  #######################################
  show_send_mail1_sub
fi
done
}

send_esc_menu(){
echo ""
echo "1.Regular mail"
echo "2.Important mail"
echo "3.With file"
echo "4.reset"
echo "5.exit"
}

show_send_mail2(){
declare member
declare content
declare imp_content="[!]"
declare -i enter_count=0
clear
send_mail_table
bring_info
IDS=(`cat ${PWD}/ids.txt`)
PWDS=(`cat ${PWD}/pwds.txt`)

num_of_id=${#IDS[@]}

tput cup 1 0
printf "to. "

tput cup 1 3
for((j=0;j<$check_size;j++)); do
  if [ "${check[$j]}" = "1" ]; then
    echo -n "${IDS[$j]} "
  fi
done

tput cup 3 0
read -p "title: " title

tput cup 5 0
echo "content:"

while true; do
bring_info
read -n 1 key

if [ "$key" = '' ]; then
send_esc_menu
read -p "send method?[1-5]:" send_method
break

else
content+="$key"

fi
done

if [ $send_method = "1" ]; then
  for ((i=0;i<$check_size;i++)); do
    if [ "${check[$i]}" = "1" ]; then
    echo $logined_num `expr $i + 1` `date +%Y-%m-%d` `date +%H:%M:%S` $title $content "0"  >> ${PWD}/msg.txt
    fi  
  done
  make_sorted_msg
  make_sorted_trash  
  bring_info
  for((i=0;i<$check_size;i++)); do
  check[$i]=0
  done

  send_mail=0
  num_of_ms+=1
  clear
  show_send_mail1_sub
  return
elif [ $send_method = "2" ]; then
  imp_content+=$title
  for ((i=0;i<$check_size;i++)); do
    if [ "${check[$i]}" = "1" ]; then
    echo $logined_num `expr $i + 1` `date +%Y-%m-%d` `date +%H:%M:%S` $imp_content $content "0" >> ${PWD}/msg.txt
    fi
  done
  make_sorted_msg
  make_sorted_trash
  bring_info
  for((i=0;i<$check_size;i++)); do
  check[$i]=0
  done

  send_mail=0
  num_of_ms+=1
  clear
  show_send_mail1_sub
  return
elif [ $send_method = "3" ]; then
  send_mail=0
  return
elif [ $send_method = "4" ]; then
  bring_info
  show_send_mail2
elif [ $send_method = "5" ]; then
  bring_info
  send_mail=0
  clear
  show_send_mail1_sub
  return
fi
}

show_sended2(){
clear
bring_info
make_sorted_msg
make_sorted_trash

send_mail_table
st=`expr $sended - 1`
MSGS=(`cat ${PWD}/msg.txt | sort -r`)
num_of_ms=`expr ${#MSGS[@]} / 7`

temp=${MSGS[`expr 7 '*' $st + 1`]}
tput cup 1 0
printf "to. ${IDS[`expr $temp - 1`]}"

tput cup 3 0
echo "title: ${MSGS[`expr 7 '*' $st + 4`]}"

tput cup 5 0
echo "content:"

tput cup 6 0
echo "${MSGS[`expr 7 '*' $st + 5`]}"

while true; do
read -s -n 1 key
if [ "$key" = "" ]; then
  clear
  show_sended_sub
  return
fi
done

}

show_sended_sub(){
clear
show_table2
bring_info
make_sorted_msg
make_sorted_trash


MSGS=(`cat ${PWD}/msg.txt | sort -r`)
num_of_ms=`expr ${#MSGS[@]} / 7`

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

if [ $sended -eq 0 ]; then

  tput cup 5 4
  echo ">"[31m"prev"[0m

else
  tput cup 5 5
  echo [31m"prev"[0m
fi

for((i=0;i<`expr "$num_of_ms" '*' 7`;i+=7)); do
  if [ "${MSGS[$i]}" = "$logined_num" ]; then
    if [ "$k" = `expr $sended - 1` ]; then
      tput cup `expr $k + 6` 4
      echo ">${MSGS[`expr $i + 4`]}"
    else
      tput cup `expr $k + 6` 5
      echo "${MSGS[`expr $i + 4`]}"
    fi

    temp=${MSGS[`expr $i + 1`]}

    tput cup `expr $k + 6` 12
    echo "-${IDS[`expr $temp - 1`]}"

    tput cup `expr $k + 6` 20
    echo "${MSGS[`expr $i + 2`]}"

    tput cup `expr $k + 6` 31
    echo "${MSGS[`expr $i + 3`]}"

    k+=1
  fi
done
k=0
}

show_sended(){
show_sended_sub

while true; do
bring_info
read -s -n 1 key

if [ "$key" = '' ]; then
read -s -n 2 key
  if [ "$key" = '[A' ]; then
    if [ $sended -eq 0 ]; then
    sended=0
    show_sended_sub
    else
    sended=${sended}-1
    show_sended_sub
    fi
  elif [ "$key" = '[B' ]; then
    if [ $sended -eq $num_of_ms ]; then
    sended=$num_of_ms
    show_sended_sub
    else
    sended+=1
    show_sended_sub
    fi
  fi
elif [ "$key" = "" ]; then
  if [ "$sended" = "0" ]; then
  show_logined_menu
  break;

  else
  show_sended2
  fi
fi

done
}
declare search

show_search_sub(){
clear
show_table2
bring_info
make_sorted_msg
make_sorted_trash

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

tput cup 5 5
echo [31m"'$search' from received mail"[0m

tput cup 13 5
echo [31m"'$search' from sended mail"[0m
}

show_search2(){
declare -i sen=0
declare -i rec=0
bring_info
while read line; do
arr=($line)
if [ "$logined_num" = "${arr[0]}" ]; then
  if [[ "${arr[4]}" =~ "$search" ]] || [[ "${arr[5]}" =~ "$search" ]]; then
  tput cup `expr 14 + $sen` 5
  echo "${arr[4]}"

  temp=`expr ${arr[1]} - 1`
  tput cup `expr 14 + $sen` 15
  echo "-${IDS[$temp]}"

  tput cup `expr 14 + $sen` 21
  echo "${arr[2]}"

  tput cup `expr 14 + $sen` 32
  echo "${arr[3]}"
  sen+=1
  fi
fi
done < ${PWD}/msg.txt

while read line2; do
arr2=($line2)
if [ "$logined_num" = `expr ${arr2[1]} - 1` ]; then
  if [[ "${arr2[4]}" =~ "$search" ]] || [[ "${arr2[5]}" =~ "$search" ]]; then
  tput cup `expr 6 + $rec` 5
  echo "${arr2[4]}"

  tput cup `expr 6 + $rec` 15
  echo "-${IDS[${arr2[0]}]}"

  tput cup `expr 6 + $rec` 21
  echo "${arr2[2]}"

  tput cup `expr 6 + $rec` 32
  echo "${arr2[3]}"
  rec+=1
  fi
fi
done < ${PWD}/msg.txt
bring_info
while true; do
read -s -n 1 key
if [ "$key" = "" ]; then
  show_logined_menu
  break
fi
done
}

show_search(){
clear
show_table2

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

tput cup 5 5
read -p [31m"search: "[0m search

show_search_sub
show_search2

}

show_trash_sub(){
clear
show_table2
bring_info
make_sorted_msg
make_sorted_trash

TRA=(`cat ${PWD}/trash.txt | sort -r`)
tra_size=`expr ${#TRA[@]} / 7`

tput cup 1 18
echo "MAIL BOX"

tput cup 3 3
echo "user: ${IDS[$logined_num]}"

if [ $trash -eq 0 ]; then

  tput cup 5 4
  echo ">"[31m"prev"[0m

else
  tput cup 5 5
  echo [31m"prev"[0m
fi

declare -i coun=0
while read line; do
arr=($line)
if [ `expr $logined_num + 1` = "${arr[1]}" ]; then
  coun+=1
  if [ "${arr[6]}" = "0" ]; then
    if [ "$coun" = "$trash" ]; then

    tput cup `expr $coun + 5` 4
    echo [32m">${arr[4]}"

    else

    tput cup `expr $coun + 5` 5
    echo [32m"${arr[4]}"

    fi

  tput cup `expr $coun + 5` 15
  echo [32m"-${IDS[${arr[0]}]}"

  tput cup `expr $coun + 5` 20
  echo "${arr[2]}"

  tput cup `expr $coun + 5` 31
  echo "${arr[3]}"[0m

elif [ "${arr[6]}" = "1" ]; then

    if [ "$coun" = "$trash" ]; then

    tput cup `expr $coun + 5` 4
    echo [0m">${arr[4]}"

    else

    tput cup `expr $coun + 5` 5
    echo [0m"${arr[4]}"

    fi

  tput cup `expr $coun + 5` 15
  echo "-${IDS[${arr[0]}]}"

  tput cup `expr $coun + 5` 20
  echo "${arr[2]}"

  tput cup `expr $coun + 5` 31
  echo "${arr[3]}"
  fi
fi
done < ${PWD}/trash.txt
bring_info
}

show_trash2(){
clear
###################################
send_mail_table
bring_info
TRA=(`cat ${PWD}/trash.txt | sort -r`)
tra_size=`expr ${#TRA[@]} / 7`

declare -i count=0
while read line; do
arr=($line)
if [ "${arr[1]}" = `expr $logined_num + 1` ]; then
  count+=1
  if [ "$count" = "$trash" ]; then
    tput cup 1 0
    echo -n "from. ${IDS[${arr[0]}]}"

    tput cup 3 0
    echo "title: ${arr[4]}"

    tput cup 5 0
    echo "content:"

    tput cup 6 0
    echo "${arr[5]}"

    if [ "${arr[6]}" = "0" ]; then
    sed -i "${count}s/.*/${arr[0]} ${arr[1]} ${arr[2]} ${arr[3]} ${arr[4]} ${arr[5]} 1/g" ${PWD}/trash.txt
    fi
  fi
fi
done < ${PWD}/trash.txt
bring_info
while true; do
read -s -n 1 key
if [ "$key" = "" ]; then
  clear
  show_trash_sub
  return
fi
done
}

show_trash(){
show_trash_sub
trash=0
while true; do
bring_info
read -s -n 1 key

if [ "$key" = '' ]; then
read -s -n 2 key
  if [ "$key" = '[A' ]; then
    if [ $trash -eq 0 ]; then
    trash=0
    show_trash_sub
    else
    trash=${trash}-1
    show_trash_sub
    fi
  elif [ "$key" = '[B' ]; then
    if [ $trash -eq $tra_size ]; then
    trash=$tra_size
    show_trash_sub
    else
    trash+=1
    show_trash_sub
    fi
  fi
elif [ "$key" = "" ]; then
  if [ "$trash" = "0" ]; then
  show_logined_menu
  break;

  else
  show_trash2
  fi

elif [ "$key" = "b" ]; then
  declare -i count=0
  while read line; do
  arr=($line)
  if [ "${arr[1]}" = `expr $logined_num + 1` ]; then
    count+=1
    if [ "$count" = "$trash" ]; then
    echo "$line" >> ${PWD}/msg.txt
    make_sorted_msg
    make_sorted_trash

    while read line1; do
    if [ "$line" = "$line1" ]; then
    sed -i "${count}s/.*//g" ${PWD}/trash.txt
    make_sorted_trash
    make_sorted_msg
    fi
    done < ${PWD}/msg.txt
    
    fi
  fi
  done < ${PWD}/trash.txt

  make_sorted_msg
  make_sorted_trash
 
  read_mail=0
  trash=0
  show_read_mail
  return
fi

done
}


###	MAIN	###

show_table
show_main

while true; do
bring_info
if [ $is_login -eq 0 ]; then
  read -s -n 1 key
  bring_info
  if [ "$key" = "" ]; then
    if [ $sign_in_or_up -eq 1 ]; then
    bring_info
    show_sign_up
    clear
    show_main

    elif [ $sign_in_or_up -eq 0 ]; then
    bring_info
    errors=0
    show_sign_in

    fi

  elif [ "$key" = 'q' ]; then
    break

  elif [ "$key" = 'w' ]; then
    for ((i=0;i<$num_of_id;i++));do
    echo "${IDS[$i]} ${PWDS[$i]}"
    done

  elif [ "$key" = '' ]; then
    read -s -n 1 key
    read -s -n 1 key
    if [ "$key" = 'A' ]; then
      sign_in_or_up=0
      clear
      show_main
    elif [ "$key" = 'B' ]; then
      sign_in_or_up=1
      clear
      show_main
    fi
  fi
elif [ $is_login -eq 1 ]; then

  if [ $choose_after_login -eq -1 ]; then
  choose_after_login=0
  show_logined_menu
  fi

  read -s -n 1 key
  if [ "$key" = '' ]; then
    read -s -n 2 key
    if [ "$key" = '[A' ]; then
      if [ $choose_after_login -eq 0 ]; then
        choose_after_login=0
	bring_info
	show_logined_menu
      else
	bring_info
        choose_after_login=${choose_after_login}-1
        show_logined_menu
      fi
    elif [ "$key" = '[B' ]; then
      if [ $choose_after_login -eq 5 ]; then
	bring_info
        choose_after_login=5
        show_logined_menu
	bring_info
      else
        choose_after_login+=1
        show_logined_menu
      fi
    fi

  elif [ "$key" = "" ]; then
    if [ $choose_after_login -eq 0 ]; then
      show_read_mail
    elif [ $choose_after_login -eq 1 ]; then
	  show_send_mail1
    elif [ $choose_after_login -eq 2 ]; then
	show_sended
    elif [ $choose_after_login -eq 3 ]; then
	show_search
    elif [ $choose_after_login -eq 4 ]; then
	show_trash
    elif [ $choose_after_login -eq 5 ]; then
      is_login=0
      choose_after_login=0
      clear
      show_table
      show_main
    fi
  fi
fi
done
