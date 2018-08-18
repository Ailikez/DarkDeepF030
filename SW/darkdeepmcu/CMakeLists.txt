# In Clion add Cmake option (Settings -> Build, Execution, Deployment -> CMake)
# -DCMAKE_TOOLCHAIN_FILE=f0.cmake
INCLUDE(CMakeForceCompiler)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

# specify the cross compiler
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(AS arm-none-eabi-as)
set(AR arm-none-eabi-ar)
set(OBJCOPY arm-none-eabi-objcopy)
set(OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)


#set(LD_PREFIX libmcu/F0/ld)
# optimizations (-O0 -O1 -O2 -O3 -Os -Ofast -Og -flto)
#set(CMAKE_C_FLAGS_DEBUG "-Og -g -DDEBUG")
#set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
#set(CMAKE_C_FLAGS_RELEASE "-Os -flto")
#set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto")

# ld script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/f030f4p6.ld)

#Uncomment for hardware floating point
#SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1 )

#Uncomment for software floating point
#SET(FPU_FLAGS "-mfloat-abi=soft")
SET(COMMON_FLAGS "-mcpu=cortex-m0 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=nano.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu11")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections -T ${LINKER_SCRIPT}")




project(f030_cpp_demo C CXX ASM)
set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

# provide the toolchain-File to CMAKE
#set(CMAKE_TOOLCHAIN_FILE CMAKE_TOOLCHAIN_FILE)

#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1 )
add_definitions(-D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\) -DUSE_HAL_DRIVER -DSTM32F030x6)

# project settings
file(GLOB_RECURSE SOURCES
        libmcu/F0/STM32F0xx_HAL_Driver/*.*
        libmcu/F0/hal_cpp/*.*
        projects/f030_cpp_demo/*.cpp
        projects/f030_cpp_demo/*.c
        projects/f030_cpp_demo/*.h
        projects/f030_cpp_demo/startup_stm32f030x6.s
)
#include_directories(.)
#include_directories(${F0_HAL_PREFIX}/STM32F0xx_HAL_Driver/Inc)
#include_directories(${F0_HAL_PREFIX}/STM32F0xx_HAL_Driver/Inc/Legacy)
#include_directories(${HAL_CPP_PREFIX})
#include_directories(${PROJECT_PREFIX})
include_directories(projects/f030_cpp_demo)
include_directories(libmcu/F0/hal_cpp)
include_directories(libmcu/F0/STM32F0xx_HAL_Driver/Inc)
include_directories(libmcu/F0/STM32F0xx_HAL_Driver/Inc/Legacy)
include_directories(libmcu/F0/cmsis/math)


set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#        COMMAND ${size} ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

add_custom_target(UPLOAD
        arm-none-eabi-gdb -iex "target remote tcp:127.0.0.1:3333"
        -iex "monitor program $<TARGET_FILE:${PROJECT_NAME}.elf>"
        -iex "monitor reset init"
        -iex "disconnect" -iex "quit")
