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
# HAL source files
file(GLOB_RECURSE HAL_SRCS
    bsp/STM32CubeL1/Drivers/STM32L1xx_HAL_Driver/Src/*.c
)

# Project source files
set(PROJECT_SRCS
    src/startup/startup_stm32l152xe.s
    src/system/syscalls.c
    src/system/sysmem.c
    src/system/system_stm32l1xx.c
    src/stm32l1xx_hal_msp.c
    src/main.c
    src/fatfs/diskio_mmc_spi.c
    src/fatfs/ff.c
    src/fatfs/ffsystem.c
    src/fatfs/ffunicode.c
)


# ========== Includes ==========
include_directories(
    src
    bsp/STM32CubeL1/Drivers/CMSIS/Include
    bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Include
    bsp/STM32CubeL1/Drivers/STM32L1xx_HAL_Driver/Inc
)


# ========== Compilation ==========
add_executable(${PROJECT_NAME}.elf
    ${HAL_SRCS}
    ${PROJECT_SRCS}
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
    -Wall
    -Wextra
    -O0
    -g3
)

target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32L152xE
)


# ========== Link ==========
target_link_libraries(${PROJECT_NAME}.elf PRIVATE
    -lc
    -lm
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
    -Wl,-T${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Projects/NUCLEO-L152RE/Templates/SW4STM32/STM32L152RE_NUCLEO/STM32L152RETx_FLASH.ld
    -Wl,--gc-sections
    -Wl,--allow-multiple-definition
)