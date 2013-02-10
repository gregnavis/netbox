function repeat_string() {
    result=""
    i=$1

    while [ $i -gt 0 ]; do
        result="${result}$2"
        i=$((i-1))
    done

	echo "$result"
}
