cmake_minimum_required(VERSION 3.7)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)



# specify the cross compiler
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(AS arm-none-eabi-as)
set(AR arm-none-eabi-ar)
set(OBJCOPY arm-none-eabi-objcopy)
set(OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

# modify the 2 lines below accordingly
project(f030_cpp_demo C CXX ASM)
set(PROJECT_PREFIX ./projects/f030_cpp_demo)
set(LIBMCU_PREFIX ./libmcu)
set(F0_HAL_PREFIX ./libmcu/F0/STM32F0xx_HAL_Driver)
set(STARTUP_PREFIX ./libmcu/F0/cmsis/startup)
set(RTOS_PREFIX ./libmcu/F0/cmsis/rtos)
set(LD_PREFIX ./libmcu/F0/ld)
set(HAL_CPP_PREFIX ./libmcu/hal_cpp)

# optimizations (-O0 -O1 -O2 -O3 -Os -Ofast -Og -flto)
#set(CMAKE_C_FLAGS_DEBUG "-Og -g -DDEBUG")
#set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
#set(CMAKE_C_FLAGS_RELEASE "-Os -flto")
#set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto")


set(LINKER_SCRIPT ${LD_PREFIX}/STM32F030F4Px_FLASH.ld)

#Uncomment for hardware floating point
#SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1 )

#Uncomment for software floating point
#SET(FPU_FLAGS "-mfloat-abi=soft")
SET(COMMON_FLAGS "-mcpu=cortex-m0 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=nano.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections -T ${LINKER_SCRIPT}")

#add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1 )
add_definitions(
    -D__weak=__attribute__\(\(weak\)\) 
    -D__packed=__attribute__\(\(__packed__\)\) 
    -DUSE_HAL_DRIVER 
    -DSTM32F030x6 
)

# project settings
file(GLOB_RECURSE SOURCES
        ${F0_HAL_PREFIX}/*.*
        ${HAL_CPP_PREFIX}/*.*
        ${PROJECT_PREFIX}/*.*
)
include_directories(
    ${F0_HAL_PREFIX}/STM32F0xx_HAL_Driver/Inc
    ${F0_HAL_PREFIX}/STM32F0xx_HAL_Driver/Inc/Legacy 
    ${HAL_CPP_PREFIX}
    ${PROJECT_PREFIX}
)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${size} ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

add_custom_target(UPLOAD
        arm-none-eabi-gdb -iex "target remote tcp:127.0.0.1:3333"
        -iex "monitor program $<TARGET_FILE:${PROJECT_NAME}.elf>"
        -iex "monitor reset init"
        -iex "disconnect" -iex "quit")