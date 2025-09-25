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
    rm -rf "$ROOT_FOLDER/test_applications/build"
    echo "Cleaned"
    exit 0;
fi

if [[ ! -v CI ]];then
    export CI=false
fi

if [[ $1 == build ]];then
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$2 ROOT_FOLDER=$ROOT_FOLDER CI=$CI all
elif [[ $1 == erase ]];then
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$2 ROOT_FOLDER=$ROOT_FOLDER CI=$CI erase
elif [[ $1 == flash ]];then
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$2 ROOT_FOLDER=$ROOT_FOLDER CI=$CI flash
elif [[ $1 == debug ]];then
	if [[ ! -f $ROOT_FOLDER/test_applications/build/$build_dir/$2/$2.elf ]]; then
		make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$2 ROOT_FOLDER=$ROOT_FOLDER CI=$CI all
	fi
	openocd -s scripts -f $ROOT_FOLDER/build_scripts/stm_programming_support/stm.cfg -c "init; reset init" &
    sleep 1
    arm-none-eabi-gdb -ex "target extended-remote :3333" \
    -ex "monitor reset init" \
    -ex "set confirm off" \
    -ex "file $ROOT_FOLDER/test_applications/build/$build_dir/$2/$2.elf" \
    -ex "load"
#    -ex "source -v $ROOT_FOLDER/build_scripts/gdb_cmd"
elif [[ $1 == disass ]];then
    make --no-print-directory -C $ROOT_FOLDER/build_scripts APP=$2 ROOT_FOLDER=$ROOT_FOLDER CI=$CI FUNC=$3 disass
fi
