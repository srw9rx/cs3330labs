for n in {1..6}; do
    cat bombinput.txt
    for i in {1..6}; do
        x=$(($RANDOM%10))
        cat "$x"
done

done
