trap '' INT

while true; do
    name=$(./field Name 5)
    code=$?
    if [ $code -eq 0 ]; then
        echo $name is n years old
    elif [ $code -ne 2 ]; then
        break;
    fi
done
