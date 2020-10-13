set -e
cc -o field field_main.c
cc -o constrained_field constrained_field_main.c

cancel=2

trap '' INT

while true; do
    # name=$(./field Name 50000000000) || case $? in
    name=$(./field Name 5) || case $? in
        $cancel) continue;;
        *) break;;
    esac
    age=$(./constrained_field Age 3 0123456789) || case $? in
        $cancel) continue;;
        *) break;;
    esac
    echo $name is $age years old
done
