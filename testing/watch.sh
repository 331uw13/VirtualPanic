#!/bin/bash

while ((1)); do
	pid=$(pidof vpanic_test)
	clear
	if [[ $pid > 0 ]]; then
		output=$(ps -p $pid -o %cpu,%mem | awk NR==2)
		echo -e "\033[32m(( Test Usage ))\033[0m\n"
		data=($(echo $output | tr " " "\n"))
		echo -e " CPU: \033[95m${data[0]}\033[0m\n MEM: \033[95m${data[1]}\033[0m"
	else
		echo -e "\033[90m Not running ...\033[0m"
	fi
	sleep 1.0
done

