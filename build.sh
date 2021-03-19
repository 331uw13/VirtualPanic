#
#  build and install script for libVirtualPanic
#  
#!/bin/bash

info() {
	echo -e "\033[95m  >> \033[90m\"$1\"\033[0m"
}
info2() {
	echo -e "\033[92m  >> \033[32m\"$1\"\033[0m"
}
info3() {
	echo -e "\033[91m [FAILURE] >> \033[31m\"$1\"\033[0m"
}

_build_type="-DCMAKE_BUILD_TYPE=Release"
_install_lib=0

for arg in "$@" ; do
	case $arg in
		"debug")
			_build_type="-DCMAKE_BUILD_TYPE=Debug";
			;;
		"install")
			_install_lib=1
			;;
		*)
		esac
done

info2 $_build_type
count=$(cat /proc/cpuinfo | grep -c ^processor)

if ! cmake -j$count . $cmake_flags ; then
	exit -1
fi
if ! make ; then
	exit -1
fi

echo -e "\n\033[1m\033[92m -------= Build Completed in ("$SECONDS") seconds! =-------\033[0m"
echo -e " \033[94m($(cat `find ./src/* -type f | grep -v "imgui\|stb_image\|gl3w"` | wc -l))\033[34m lines of code!\n\033[0m"
sleep 1

if [[ $_install_lib == 1 ]] ; then
	info "sudo make install"
	sudo make install > make_install.log
	info2 "Installed libVirtualPanic!"
fi

info2 "Done!"

