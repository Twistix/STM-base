# Specify the CMake minimum version
cmake_minimum_required(VERSION 3.15)

# Define the project
project(STM32_project 
        VERSION 1.0.0
        LANGUAGES C ASM)

# Compiler and linker flags
set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft")
set(CMAKE_C_FLAGS "${CPU_FLAGS} -Wall -Wextra -O0 -g3")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS}")
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Projects/NUCLEO-L152RE/Templates/SW4STM32/STM32L152RE_NUCLEO/STM32L152RETx_FLASH.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-T${LINKER_SCRIPT} -Wl,--gc-sections")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Include
    ${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Drivers/STM32L1xx_HAL_Driver/Inc
)

# Source files
set(STARTUP_FILES
    ${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Source/Templates/gcc/startup_stm32l152xe.s
)

set(CMSIS_SOURCES
    ${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Drivers/CMSIS/Device/ST/STM32L1xx/Source/Templates/system_stm32l1xx.c
)

file(GLOB_RECURSE HAL_SOURCES
    ${CMAKE_SOURCE_DIR}/bsp/STM32CubeL1/Drivers/STM32L1xx_HAL_Driver/Src/*.c
)

set(SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/main.c
)

# Combine all sources
set(SOURCES ${STARTUP_FILES} ${CMSIS_SOURCES} ${HAL_SOURCES} ${SRC_FILES})

# Add the executable target
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# Add target definitions
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32L152xE
    USE_HAL_DRIVER
    DEBUG
)

# Add linker flags after object files are listed
target_link_libraries(${PROJECT_NAME}.elf
    -lc
    -lm
)

# Post-build commands
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJDUMP} -D ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lst
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMENT "Building binary, list file, and size information"
)
