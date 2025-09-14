cmake_minimum_required(VERSION 3.22)
project(Ncurses-stuff LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------
# Output directories
# -------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(CONFIG IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# -------------------------------
# Homebrew prefix (macOS) / Linux
# -------------------------------
if(APPLE)
    set(HOMEBREW_PREFIX $<$<STREQUAL:${CMAKE_OSX_ARCHITECTURES},arm64>:/opt/homebrew:/usr/local>)
    list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
else()
    set(HOMEBREW_PREFIX "/usr")
endif()

# -------------------------------
# Function to build each subdir
# -------------------------------
function(add_ncurses_program SUBDIR)
    file(GLOB_RECURSE PROG_SRC CONFIGURE_DEPENDS ${SUBDIR}/*.cpp ${SUBDIR}/*.h)
    if(NOT PROG_SRC)
        return()
    endif()

    get_filename_component(PROG_NAME ${SUBDIR} NAME)
    add_executable(${PROG_NAME} ${PROG_SRC})
    target_compile_features(${PROG_NAME} PRIVATE cxx_std_20)

    # --- Wide-char / UTF-8 support ---
    target_compile_definitions(${PROG_NAME} PRIVATE _XOPEN_SOURCE_EXTENDED)
    target_compile_options(${PROG_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)

    # --- Ncurses linkage ---
    if(APPLE)
        set(NCURSES_PREFIX "")
        foreach(prefix /opt/homebrew /usr/local)
            if(EXISTS "${prefix}/opt/ncurses/include/ncursesw/ncurses.h")
                set(NCURSES_PREFIX "${prefix}/opt/ncurses")
                break()
            endif()
        endforeach()
        if(NCURSES_PREFIX STREQUAL "")
            message(FATAL_ERROR "Homebrew ncurses not found. Install with: brew install ncurses")
        endif()

        target_include_directories(${PROG_NAME} PRIVATE ${NCURSES_PREFIX}/include)
        target_link_directories(${PROG_NAME} PRIVATE ${NCURSES_PREFIX}/lib)
        target_link_libraries(${PROG_NAME} PRIVATE ncursesw panelw)
        target_compile_definitions(${PROG_NAME} PRIVATE USE_NCURSESW)
        message(STATUS "Using Homebrew ncurses from ${NCURSES_PREFIX}")
    else()
        find_package(Curses REQUIRED)
        target_include_directories(${PROG_NAME} PRIVATE ${CURSES_INCLUDE_DIR})
        target_link_libraries(${PROG_NAME} PRIVATE ${CURSES_LIBRARIES} panel)
        target_compile_definitions(${PROG_NAME} PRIVATE USE_NCURSESW)
    endif()

    # --- Post-build: show linked libraries ---
    if(APPLE)
        add_custom_command(TARGET ${PROG_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo '=== Linked libraries (macOS) ==='
            COMMAND otool -L $<TARGET_FILE:${PROG_NAME}>
        )
    elseif(UNIX)
        add_custom_command(TARGET ${PROG_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo '=== Linked libraries (Linux) ==='
            COMMAND ldd $<TARGET_FILE:${PROG_NAME}>
        )
    endif()
endfunction()

# -------------------------------
# Build all subdirectories in src/
# -------------------------------
file(GLOB CHILDREN RELATIVE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/*)
foreach(CHILD ${CHILDREN})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/src/${CHILD})
        add_ncurses_program(${CMAKE_SOURCE_DIR}/src/${CHILD})
    endif()
endforeach()

