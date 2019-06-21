sum_ids=0
leaves_ids=0

sen_bytes=0
sen_files=0
rec_bytes=0
rec_files=0

max=0
min=0
k=0
while IFS= read -r line; do

      if [ $line -eq -30 ]
      then
	  while IFS= read -r line; do

              sum_ids=$((sum_ids+1))
              printf 'id  %d\n' "$line"
	      if [ $k -eq 0 ]
	      then
	          max=$((max+$line))
		  min=$((min+$line))
		  k=$((k+9))
	      else
		  if [ $line -gt $max ]
		  then
		      max="$line"
		  fi
		  if [ $line -lt $min ]
		  then
		     min="$line"
		  fi
	      fi

	      break
	  done
      elif [  $line -eq -1 ]
      then
          leaves_ids=$((leaves_ids+1))
      elif [ $line -eq -10 ]
      then
          count=0
          while IFS= read -r line; do
              if [ $count -eq 0 ]
	      then
	          sen_bytes=$((sen_bytes+$line))
              else
	          sen_files=$((sen_files+$line))
              fi
              count=$((count+1))
	      if [ $count -eq 2 ]
	      then
	          break
	      fi
           done
      else
          count=0
          while IFS= read -r line; do
	      if [ $count -eq 0 ]
	      then
	          rec_bytes=$((rec_bytes+$line))
 	      else
	          rec_files=$((rec_files+1))
	      fi
	      count=$((count+1))
	      if [ $count -eq 2 ]
	      then
	          break
	      fi
          done
      fi
done


printf '\nCrowd ids  %d\n' "$sum_ids"
printf 'Crowd leave clients  %d\n\n' "$leaves_ids"
printf 'Max id  %d\n' "$max"
printf 'Min id  %d\n\n' "$min"
printf 'Byets send  %d\n' "$sen_bytes"
printf 'Files send  %d\n\n' "$sen_files"
printf 'Byets received  %d\n' "$rec_bytes"
printf 'File received  %d\n' "$rec_files"
