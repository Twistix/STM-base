# ========== CMake version ==========
cmake_minimum_required(VERSION 3.15)


# ========== Toolchain ==========
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-gnu-toolchain.cmake)


# ========== Project definition ==========
project(STM32-SD-Card
        VERSION 1.0
        LANGUAGES C ASM)


# ========== CMake options ==========
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# ========== Source files ==========
# Startup source files
set(STARTUP_SRCS
    bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Source/Templates/gcc/startup_stm32l152xe.s
    bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Source/Templates/system_stm32l1xx.c
)

# HAL source files
file(GLOB_RECURSE HAL_SRCS
    bsp/STM32CubeL1/Drivers/STM32L1xx_HAL_Driver/Src/*.c
)

# Project source files
set(PROJECT_SRCS
    src/main.c
    #src/fatfs/diskio_mmc_spi.c
    #src/fatfs/ff.c
    #src/fatfs/ffsystem.c
    #src/fatfs/ffunicode.c
)


# ========== Includes ==========
include_directories(
    src
    bsp/STM32CubeL1/Drivers/CMSIS/Include
    bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Include
    bsp/STM32CubeL1/Drivers/STM32L1xx_HAL_Driver/Inc
)


# ========== Compilation ==========
# Compile startup sources as static library
add_library(startup_lib STATIC 
    ${STARTUP_SRCS}
)

target_compile_options(startup_lib PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
    -Wall
    -Wextra
    -O0
    -g3
)

target_compile_definitions(startup_lib PRIVATE
    STM32L152xE
)

# Compile HAL sources as static library
add_library(HAL_lib STATIC 
    ${HAL_SRCS}
)

target_compile_options(HAL_lib PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
    -Wall
    -Wextra
    -O0
    -g3
)

target_compile_definitions(HAL_lib PRIVATE
    STM32L152xE
)

# Compile project sources as executable
add_executable(app.elf 
    ${PROJECT_SRCS}
)

target_compile_options(app.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
    -Wall
    -Wextra
    -O0
    -g3
)

target_compile_definitions(app.elf PRIVATE
    STM32L152xE
)


# ========== Link ==========
target_link_libraries(app.elf PRIVATE
    startup_lib
    HAL_lib
)

target_link_options(app.elf PRIVATE
    -Wl,-T${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Projects/NUCLEO-L152RE/Templates/SW4STM32/STM32L152RE_NUCLEO/STM32L152RETx_FLASH.ld
    -Wl,--gc-sections
)