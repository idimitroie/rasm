cmake_minimum_required(VERSION 3.10)
project(rasm LANGUAGES C CXX)

# Set the compiler to clang with options set(CMAKE_C_COMPILER clang)
add_compile_options(-Wall -gdwarf-4)

# Define compile options for different build types
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_UNITTEST "-O0 -g -Wall -DUNITTEST=1")
# Set the build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Define additional options based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring Debug build")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build")
elseif(CMAKE_BUILD_TYPE STREQUAL "Unittest")
    message(STATUS "Configuring Unittest build")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_UNITTEST}" )
    enable_testing()
endif()


message(STATUS "CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
# Find Bison and Flex
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# Specify your source files
file(GLOB SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.l"
    "${CMAKE_SOURCE_DIR}/src/*.y"
    )

# Define the bison parser
BISON_TARGET(Parser ${CMAKE_SOURCE_DIR}/src/parser.y ${CMAKE_BINARY_DIR}/parser.tab.c
    DEFINES_FILE ${CMAKE_BINARY_DIR}/parser.tab.h
    )

# Define the flex scanner
FLEX_TARGET(Scanner ${CMAKE_SOURCE_DIR}/src/scanner.l ${CMAKE_BINARY_DIR}/scanner.yy.c
    COMPILE_FLAGS "--header-file=${CMAKE_BINARY_DIR}/scanner.h"
    DEFINES_FILE ${CMAKE_BINARY_DIR}/scanner.h
    )

message( STATUS "FLEX_Scanner_INPUT ${FLEX_Scanner_INPUT} - the flex source file, an alias for ${FlexInput}")
message( STATUS "FLEX_Scanner_OUTPUT_HEADER ${FLEX_Scanner_OUTPUT_HEADER} - the header flex output, if any.")

# Add a custom target that links Flex and Bison generated files
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR})

# Define a function to create an OBJECT library and append object files to a common variable
function(create_object_library lib_name output_var)
    set(src_files ${ARGN})

    # Create the OBJECT library
    add_library(${lib_name} OBJECT ${src_files})

    # Append the object file to the list
    set(${output_var} ${${output_var}} $<TARGET_OBJECTS:${lib_name}> PARENT_SCOPE)

    # Print a debug message
    message(STATUS "Created OBJECT library: ${lib_name}")
    message(STATUS "Appending object files to ${output_var}: $<TARGET_OBJECTS:${lib_name}>")
endfunction()

create_object_library(main   modules src/main.c ${CMAKE_BINARY_DIR}/scanner.h ${CMAKE_BINARY_DIR}/parser.tab.h)
create_object_library(decode modules src/decode.c src/decode.h)
create_object_library(instr  modules src/instr.c ${CMAKE_BINARY_DIR}/parser.tab.h src/instr.h)
create_object_library(label  modules src/label.c src/label.h)
create_object_library(prgcnt modules src/progc.c src/progc.h)
create_object_library(loclab modules src/loclab.c src/loclab.h)
create_object_library(storage modules src/storage.c src/storage.h)

# Clean target 
add_custom_target(force_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    )

# If using unit tests, add your test files here
if(CMAKE_BUILD_TYPE STREQUAL "Unittest")
    add_subdirectory(test/unittests)
else()
    # Create the executable
    add_executable(rasm 
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Scanner_OUTPUTS}
        )
    target_link_libraries(rasm ${modules})
endif()

# Check if Doxygen is found
if(CMAKE_BUILD_TYPE STREQUAL "Doc" )
    # Find Doxygen package
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        # Set the output directory for documentation
        set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/doc")

        # Create a Doxygen configuration file if it doesn't exist
        set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/Doxyfile")
        set(DOXYGEN_OUT "${DOXYGEN_OUTPUT_DIR}/Doxygen")

        # Add the Doxygen configuration options
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_LATEX YES)
        set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")

        # Create a custom command to run Doxygen
        add_custom_command(
            OUTPUT ${DOXYGEN_OUT}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS ${DOXYGEN_IN}  # Ensure Doxyfile is a dependency
            COMMENT "Generating API documentation with Doxygen"
            )

        # Create a custom target for building documentation
        add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUT})

        # Optional: Create a target to clean up generated documentation
        add_custom_target(clean_doc
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${DOXYGEN_OUTPUT_DIR}
            COMMENT "Cleaning up Doxygen documentation"
            )
    else()
        message(WARNING "Doxygen not found, documentation will not be generated.")
    endif()
endif()
