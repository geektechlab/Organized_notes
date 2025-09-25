Want to stop compilation at preprocessing stage.

Add this in your makefile
EXTRA_CFLAGS=’-save-temps’

This will generate all the intermediate files in generating .ko

If you need only .i file.

$ make -C /lib/modules/`uname -r`/build M=${PWD} hello.i
