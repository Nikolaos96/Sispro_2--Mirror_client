#!/bin/bash


ARR=( {48..57} {65..90} {97..122} )
arrcount=${#ARR[@]}


# return random name for directory or file
get_rand_dir(){
    for ((i=0; i<$((RANDOM%8 + 1)); i++)) {
        printf \\$(printf '%03o' ${ARR[RANDOM%arrcount]});
    }
}


# return random data for file
get_rand_dir_2(){
    k="$(((RANDOM % (32767 + 1 - 256)) + 256))"
    for ((i=0; i<"$k" ; i++)) {
        printf \\$(printf '%03o' ${ARR[RANDOM%arrcount]});
    }
}



if [ ! -d "$1" ]
then
    echo "File doesn't exist. Creating now"
    mkdir -p "$1"
else
    echo "File exists"
fi

if [ "$2" -lt 0 ]
then
    echo "arguments numbers are smaller than 0"
    exit 1
fi

if [ "$3" -lt 0 ]
then
    echo "arguments numbers are smaller than 0"
    exit 1
fi

if [ "$4" -le 0 ]
then
    echo "arguments numbers are smaller than 0"
    exit 1
fi


#files="$2"

dirs="$3"
dirs=$((dirs+1))


files=$(($2/$dirs))
ypo=$(($2%$dirs))


for ((k=0 ; k < files ; k++))
do
    pp2="$1/$(get_rand_dir)"
    touch "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
done
####################################################3
if [ $ypo -gt 0 ]
then
    pp2="$1/$(get_rand_dir)"
    touch "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    echo "$(get_rand_dir_2)" >> "$pp2"
    ypo=$((ypo-1))
fi
#########################################################


rep="$3"
while [ $rep -gt 0 ]
do
    direc="$1/$(get_rand_dir)"
    mkdir -p "$direc"
    rep=$((rep-1))
    for ((k=0 ; k < files ; k++))
    do
        pp2="$direc/$(get_rand_dir)"
	touch "$pp2"
	echo "$(get_rand_dir_2)" >> "$pp2"
	echo "$(get_rand_dir_2)" >> "$pp2"
	echo "$(get_rand_dir_2)" >> "$pp2"
	echo "$(get_rand_dir_2)" >> "$pp2"
    done
#############################################################33
    if [ $ypo -gt 0 ]
    then
        pp2="$direc/$(get_rand_dir)"
	touch "$pp2"
	echo "$(get_rand_dir_2)" >> "$pp2"
        echo "$(get_rand_dir_2)" >> "$pp2"
        echo "$(get_rand_dir_2)" >> "$pp2"
        echo "$(get_rand_dir_2)" >> "$pp2"
        ypo=$((ypo-1))
    fi
###################################################################3
    direc2="$direc"
    for ((i=0 ; i < ("$4"-1) ; i++))
    do
        if [ $rep -gt 0 ]
        then
            direc2="$direc2/$(get_rand_dir)"
            mkdir -p "$direc2"
	    rep=$((rep-1))
	    for ((j=0 ; j < files ; j++))
	    do
	       pp="$direc2/$(get_rand_dir)"
	       touch "$pp"
	       echo "$(get_rand_dir_2)" >> "$pp"
	       echo "$(get_rand_dir_2)" >> "$pp"
	       echo "$(get_rand_dir_2)" >> "$pp"
               echo "$(get_rand_dir_2)" >> "$pp"
	    done
###################################
	    if [ $ypo -gt 0 ]
	    then
		 pp="$direc2/$(get_rand_dir)"
		 touch "$pp"
		 echo "$(get_rand_dir_2)" >> "$pp"
                 echo "$(get_rand_dir_2)" >> "$pp"
                 echo "$(get_rand_dir_2)" >> "$pp"
                 echo "$(get_rand_dir_2)" >> "$pp"
		 ypo=$((ypo-1))
	    fi
########################################
	else
	    break 2
	fi
    done
done
