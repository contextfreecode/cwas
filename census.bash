trap '' INT

cancel=2
success=0

while true; do
    # Name
    # name=$(./field Name 50000000000)
    name=$(./field Name 5)
    code=$?
    if [ $code -eq $cancel ]; then
        continue;
    elif [ $code -ne $success ]; then
        break;
    fi
    # Age
    age=$(./constrained_field Age 3 0123456789)
    code=$?
    if [ $code -eq $cancel ]; then
        continue;
    elif [ $code -ne $success ]; then
        break;
    fi
    # Message
    echo $name is $age years old
done
