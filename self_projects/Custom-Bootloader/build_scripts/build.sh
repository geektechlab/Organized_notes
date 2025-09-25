#!/bin/bash
# Compile target application

# Fetch ROOT directory path
ROOT_FOLDER="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

case `uname` in
    *CYGWIN*|*MINGW*|*MSYS*) ROOT_FOLDER=`cygpath -m "$ROOT_FOLDER"`;;
esac

if [[ $1 == clean ]];then
    find . -name '*.o' -exec rm -r {} \;
    find . -name '*.d' -exec rm -r {} \;
    rm -rf "$ROOT_FOLDER/boot_scripts/build"
    echo "Cleaned"
    exit 0;
fi

if [[ ! -v CI ]];then
    export CI=false
fi

export APP=bootloader

# Use this to zip directories
#zip -r $ROOT_FOLDER/stm_files.zip $ROOT_FOLDER/stm_files/
#exit 1;

get_dep()
{
	if [[ ! -d $ROOT_FOLDER/stm_files ]]; then
		unzip $ROOT_FOLDER/stm_files.zip -d $ROOT_FOLDER/stm_files
		if [[ -d $ROOT_FOLDER/stm_files/C: ]]; then
			mv $ROOT_FOLDER/stm_files/"$(dirname $ROOT_FOLDER)"/STM32-Bare-metal-embedded-development/stm_files/* $ROOT_FOLDER/stm_files/
			rm -rf $ROOT_FOLDER/stm_files/C:
		fi
		rm -rf $ROOT_FOLDER/C:
	fi
}

if [[ $1 == build ]];then
	get_dep
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$APP ROOT_FOLDER=$ROOT_FOLDER CI=$CI all
elif [[ $1 == erase ]];then
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$APP ROOT_FOLDER=$ROOT_FOLDER CI=$CI erase
elif [[ $1 == flash ]];then
	get_dep
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$APP ROOT_FOLDER=$ROOT_FOLDER CI=$CI flash
elif [[ $1 == debug ]];then
	get_dep
	if [[ ! -f $ROOT_FOLDER/boot_scripts/build/$build_dir/$APP/$APP.elf ]]; then
		make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$APP ROOT_FOLDER=$ROOT_FOLDER CI=$CI all
	fi
	openocd -s scripts -f $ROOT_FOLDER/build_scripts/stm_programming_support/stm.cfg -c "init; reset init" &
    sleep 1
    arm-none-eabi-gdb -ex "target extended-remote :3333" \
    -ex "monitor reset init" \
    -ex "set confirm off" \
    -ex "file $ROOT_FOLDER/boot_scripts/build/$build_dir/$APP/$APP.elf" \
    -ex "load"
#    -ex "source -v $ROOT_FOLDER/build_scripts/gdb_cmd"
elif [[ $1 == disass ]];then
	get_dep
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$APP ROOT_FOLDER=$ROOT_FOLDER CI=$CI FUNC=$3 disass
fi
