set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
find_program(ARM_GCC_PATH arm-none-eabi-gcc
    PATHS /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin
    NO_DEFAULT_PATH
)
if(NOT ARM_GCC_PATH)
    find_program(ARM_GCC_PATH arm-none-eabi-gcc)
endif()
get_filename_component(ARM_TOOLCHAIN_BIN_DIR ${ARM_GCC_PATH} DIRECTORY)

set(CMAKE_C_COMPILER   ${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-g++)
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
find_program(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# MCU specific flags
set(TARGET_FLAGS "-mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections -fstack-usage")

#cắt bỏ phần prefix path chung, để __FILE__ chỉ còn path tương đối từ project root
get_filename_component(FIRMWARE_ROOT "${CMAKE_SOURCE_DIR}/../../.." ABSOLUTE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmacro-prefix-map=${FIRMWARE_ROOT}/=")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmacro-prefix-map=${FIRMWARE_ROOT}/=")

# The cyclomatic-complexity parameter must be defined for the Cyclomatic complexity feature in STM32CubeIDE to work.
# However, most GCC toolchains do not support this option, which causes a compilation error; for this reason, the feature is disabled by default.
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcyclomatic-complexity")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_EXE_LINKER_FLAGS "${TARGET_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32U535CEUXQ_FLASH.ld\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
set(TOOLCHAIN_LINK_LIBRARIES "m")

# Tự sinh wrapper lọc output: mặc định chỉ hiện lỗi + 3 dòng trước / 10 dòng sau,
# đặt VERBOSE_BUILD=1 để xem full log
set(_QUIET_COMPILE_SCRIPT "${CMAKE_BINARY_DIR}/quiet_compile.sh")
file(WRITE ${_QUIET_COMPILE_SCRIPT}
"#!/bin/bash
OUTPUT=\$(\"\$@\" 2>&1)
STATUS=\$?
if [ -n \"\$VERBOSE_BUILD\" ]; then
    echo \"\$OUTPUT\"
elif [ \$STATUS -ne 0 ]; then
    echo \"\$OUTPUT\" | grep -B3 -A10 \"error:\"
fi
exit \$STATUS
")
file(CHMOD ${_QUIET_COMPILE_SCRIPT} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

set(CMAKE_C_COMPILER_LAUNCHER   ${_QUIET_COMPILE_SCRIPT})
set(CMAKE_CXX_COMPILER_LAUNCHER ${_QUIET_COMPILE_SCRIPT})