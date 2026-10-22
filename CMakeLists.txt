cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME h7_env)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Set optimization
set(CMAKE_C_FLAGS_DEBUG "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

# Open D2 domain
add_compile_definitions(DATA_IN_D2_SRAM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    Middlewares/ST/ARM/DSP/Include
)

# Add user source files
file(GLOB USER_SOURCES
    "Applications/Task/*.c"
    "Applications/Task/*.cpp"
    "IHiter_Components/Hardwares/*.cpp"
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ${USER_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    Applications/Task
    IHiter_Components/Hardwares
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
    ARM_MATH_CM7
    __FPU_PRESENT=1U
    ARM_MATH_CHECK
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/ARM/DSP/Lib/libCMSISDSP.a
)

# Add compile options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wno-unused-variable
    -Wno-unused-but-set-variable
    -Wno-unused-function
    -Wno-missing-braces
)

# Generate HEX / BIN after build
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files"
)

# J-Link download target
find_program(JFLASH_EXE NAMES JFlash.exe)
set(JFLASH_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/User_Debug/STM32H723VG.jflash")
set(JFLASH_HEX "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.hex")
set(JFLASH_LOG "${CMAKE_CURRENT_BINARY_DIR}/jflash.log")

if(NOT EXISTS "${JFLASH_PROJECT}")
    message(WARNING "JFlash project file not found: ${JFLASH_PROJECT}. Create it with J-Flash GUI and save it as STM32H723VG.jflash.")
endif()

add_custom_target(JLink_FLASH
    COMMAND powershell
        -NoProfile
        -ExecutionPolicy Bypass
        -File "${CMAKE_CURRENT_SOURCE_DIR}/User_Debug/jlink_details.ps1"
        -JFlashExe "${JFLASH_EXE}"
        -Project "${JFLASH_PROJECT}"
        -Hex "${JFLASH_HEX}"
        -LogFile "${JFLASH_LOG}"

    DEPENDS ${CMAKE_PROJECT_NAME}

    COMMENT "Download firmware with J-Flash"
    USES_TERMINAL
)
