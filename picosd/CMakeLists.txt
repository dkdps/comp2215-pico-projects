# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/mnt/c/Users/champ/Documents/Programming/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(SDCard C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(SDCard SDCard.c hw_config.c)

pico_set_program_name(SDCard "SDCard")
pico_set_program_version(SDCard "0.1")

pico_enable_stdio_uart(SDCard 1)
pico_enable_stdio_usb(SDCard 0)

add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)
# Add the standard library to the build
target_link_libraries(SDCard pico_stdlib)

target_compile_options(SDCard PUBLIC -Wall -Wextra -Wno-unused-function -Wno-unused-parameter)

# Add any user requested libraries
target_link_libraries(SDCard
        FatFs_SPI
        hardware_clocks
        hardware_adc
        hardware_spi
        hardware_dma
        hardware_pio
        hardware_clocks
        )

pico_add_extra_outputs(SDCard)
pico_enable_stdio_uart(SDCard 1)
pico_enable_stdio_usb(SDCard 1)
