# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/mnt/c/Users/champ/Documents/Programming/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(JukeBoxSD C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# FatFs Module
add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Add executable. Default name is the project name, version 0.1
add_executable(JukeBoxSD JukeBoxSD.c hw_config.c)

pico_set_program_name(JukeBoxSD "JukeBoxSD")
pico_set_program_version(JukeBoxSD "0.1")

pico_enable_stdio_usb(JukeBoxSD 1)
pico_enable_stdio_uart(JukeBoxSD 0)

# Add the standard library to the build
target_link_libraries(JukeBoxSD pico_stdlib)

target_compile_options(JukeBoxSD PUBLIC -Wall -Wextra -Wno-unused-function -Wno-unused-parameter)

# Add any user requested libraries
target_link_libraries(JukeBoxSD
FatFs_SPI
hardware_spi
hardware_i2c
hardware_pio
hardware_pwm
hardware_irq
hardware_sync
        )
        
pico_add_extra_outputs(JukeBoxSD)
        