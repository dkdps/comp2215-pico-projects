# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "~/coursework/comp2215/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico-midi-ultra C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico-midi-ultra pico-midi-ultra.c usb_descriptors.c ICM20948.c)

pico_set_program_name(pico-midi-ultra "pico-midi-ultra")
pico_set_program_version(pico-midi-ultra "0.1")

pico_enable_stdio_usb(pico-midi-ultra 0)
pico_enable_stdio_uart(pico-midi-ultra 1)

# Add the standard library to the build
target_include_directories(pico-midi-ultra PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pico-midi-ultra PRIVATE pico_stdlib tinyusb_device tinyusb_board hardware_i2c)

pico_add_extra_outputs(pico-midi-ultra)
