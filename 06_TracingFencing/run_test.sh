#!/bin/bash

expected_exit_code=0
when=1
in_file="in.file"

POSITIONAL=()
while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
        -e|--exit_code)
            expected_exit_code="$2"
            shift
            shift
            ;;
        -w|--when)
            when="$2"
            shift
            shift
            ;;
        -i|--in_file)
            in_file="$2"
            shift
            shift
            ;;
#       --default)
#           DEFAULT=YES
#           shift
#           ;;
        *)
            POSITIONAL+=("$1")
            shift
            ;;
    esac
done

set -- "${POSITIONAL[@]}"


if [ $# -ne 2 ]; then
    echo "There should be 2 positional arguments!"
    exit 1
fi

check_call="$1"
check_error="$2"

in_check_file="check-$in_file"
out_file="out.file"
content="1 + 1 = 2\n2 * 2 = 4\n123123213\n\n\n\n\n\t\t\t\t123 12 3 123 213 2  32 3 2 323\n\n1 3 12 32 32     123 2     \n 1      12 3 \n\n      df    \n     \n\n\n"

printf "$content" > "$in_file"
cp "$in_file" "$in_check_file"

touch "$out_file"
ls

# -P "./$in_file" -P "./$out_file"
# -P "$(pwd)/$in_file" -P "$(pwd)/$out_file"
echo "$in_file $out_file $check_call $when $check_error"
echo "$(pwd)/$in_file, $(pwd)/$out_file"
strace -P "$(pwd)/$in_file" -P "$(pwd)/$out_file" -e fault="$check_call":error="$check_error":when="$when" ./move "$in_file" "$out_file" # > /dev/null 2> /dev/null
exit_code="$?"
echo "$exit_code $expected_exit_code"

# test_name="$0 (error = $check_error)"
test_name="$check_error"
printf "%-13s: " "$test_name"
if [ "$exit_code" -ne "$expected_exit_code" ] || ( [ $exit_code -eq 0 ] && ! $(cmp -s "$in_check_file" "$out_file") ); then
    echo "WA"
else
    echo "OK"
fi

rm -f "$in_file" "$out_file" "$in_check_file"

