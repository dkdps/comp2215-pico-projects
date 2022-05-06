# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/mnt/c/Users/champ/Documents/Programming/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-fractal C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico-fractal pico-fractal.c fonts.c st7735.c DEV_Config.c)

pico_set_program_name(pico-fractal "pico-fractal")
pico_set_program_version(pico-fractal "0.1")

pico_enable_stdio_uart(pico-fractal 1)
pico_enable_stdio_usb(pico-fractal 0)

# Add the standard library to the build
target_link_libraries(pico-fractal 
hardware_pwm
hardware_pio
hardware_spi
hardware_i2c
pico_stdlib
)

pico_add_extra_outputs(pico-fractal)

