cmake_minimum_required(VERSION 2.8)
project(MSP430I2C)

include(CMake/MSP430.cmake)

set(BOARD_NAME "msp430g2553")

# Set the C flags. Only the compiler is set by the platform file. The -mmcu flag is REQUIRED 
# by mspgcc to find the correct include files and enable preprocessor defs. this definition should
# be equal to the target board.
set(MSP_CPU_FLAG "-mmcu=${BOARD_NAME}")
set(CMAKE_C_FLAGS "${MSP_CPU_FLAG} -Os")
set(CMAKE_CXX_FLAGS "${MSP_CPU_FLAG} -O3 --std=c++0x -g -ffunction-sections -fno-rtti -fdata-sections --gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${MSP_CPU_FLAG} -lm -Wl -Os -L${CMAKE_CURRENT_SOURCE_DIR}/MSP430 -L${CMAKE_CURRENT_SOURCE_DIR}/MSP430/${BOARD_NAME} -Tmsp430.x")

# Create the executable
add_executable(i2cmaster
    Source/main.cpp
    Source/Application.cpp
    Source/Application.h
    Source/I2C/I2CBus.cpp
    Source/I2C/I2CBus.h
    Source/IO/Button.h
    Source/IO/FlashVariable.h
    Source/IO/Pin.h
    Source/FM/FMReceiver.cpp
    Source/FM/FMReceiver.h
    Source/FM/FMMemory.cpp
    Source/FM/FMMemory.h
)

# Create a flash target if the toolchain file was selected correctly
if (MSP_TOOLCHAIN)
    setup_flash_target(i2cmaster rf2500)
endif () 