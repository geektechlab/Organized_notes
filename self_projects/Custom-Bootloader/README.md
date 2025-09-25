# Custom bootloader and Host application using STM32L476RG

Currently developing custom bootloader and host application for STM32L476RG board without using any IDE/development environment.

Host application in PC <-- *UART* --> Custom bootloader application running in STM32L476RG

Commands:
- **Clean project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh clean
- **Build project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh build
- **Flash project**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh flash
- **Erase project from STM32L476RG**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh erase
- **Debug project using GDB**:`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh debug
- **Dissassemble** :`<ARGUMENTS> <PATH OF PROJECT'S ROOT FOLDER>`/build_scripts/build.sh disass `FUNCTION`

_Note:_
- _`FUNCTION` can be any function in the project. e.g. `main`_

Documents related to STM32L476RG:

- https://www.st.com/resource/en/datasheet/stm32l476je.pdf
- https://www.st.com/resource/en/reference_manual/rm0351-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf
- https://wiki.st.com/stm32mcu/wiki/Getting_started_with_UART
- https://wiki.st.com/stm32mcu/wiki/Getting_started_with_EXTI
