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
