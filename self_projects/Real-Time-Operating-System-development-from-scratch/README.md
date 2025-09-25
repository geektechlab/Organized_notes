# Real Time Operating System development from scratch

Developing Real-Time Operating System on STM32L476RG without using any IDE from scratch to understand and get hands-on experience on RTOS kernel internals and OS support features of the Cortex-M architecture.

Commands:
- **Clean project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh clean
- **Build project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh build `APP`
- **Flash project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh flash `APP`
- **Erase project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh erase
- **Debug project using GDB**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh debug `APP`
- **Dissassemble** :`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh disass `APP` `FUNCTION`

_Note:_
- _`APP` can be selected from test_applications directory. e.g. `toggle_led`_
- _`FUNCTION` can be any function in the project. e.g. `main`_

Documents related to STM32L476RG:

- https://www.st.com/resource/en/datasheet/stm32l476je.pdf
- https://www.st.com/resource/en/reference_manual/rm0351-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf

Pin details:
![Nucleo-L476 pin details](./image.png?raw=true "Nucleo-L476 pin details")