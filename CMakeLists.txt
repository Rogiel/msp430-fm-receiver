cmake_minimum_required(VERSION 2.8)
project(MSP430I2C)

include(cmake/msp430.cmake)

set(BOARD_NAME "msp430g2553")

# Set the C flags. Only the compiler is set by the platform file. The -mmcu flag is REQUIRED 
# by mspgcc to find the correct include files and enable preprocessor defs. this definition should
# be equal to the target board.
set(MSP_CPU_FLAG "-mmcu=${BOARD_NAME}")
set(CMAKE_C_FLAGS "${MSP_CPU_FLAG}")
set(CMAKE_CXX_FLAGS "${MSP_CPU_FLAG} --std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${MSP_CPU_FLAG} -lm -Wl,--gc-sections -Os ")

# Create the executable
add_executable(i2cmaster Source/main.cpp Source/I2C/I2CBus.cpp Source/I2C/I2CBus.h Source/IO/Pin.h Source/FM/FMReceiver.cpp Source/FM/FMReceiver.h)

# Create a flash target if the toolchain file was selected correctly
if (MSP_TOOLCHAIN)
    setup_flash_target(i2cmaster rf2500)
endif () 