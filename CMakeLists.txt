cmake_minimum_required(VERSION 3.10)
project(mdflash VERSION 0.6.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 QUIET
        COMPONENTS Widgets)
if (Qt5_FOUND)
    set(QtVersion 5)
else ()
    find_package(Qt6 REQUIRED
            COMPONENTS Widgets)
    set(QtVersion 6)
endif ()

add_executable(mdflash
        main.cpp)

file(GLOB_RECURSE Sources src/*.cpp)

target_sources(mdflash
        PRIVATE ${Sources})

target_link_libraries(mdflash
        PRIVATE Qt${QtVersion}::Widgets)

target_include_directories(mdflash
        PUBLIC include
        PRIVATE include/mdflash)

install(TARGETS mdflash DESTINATION bin)
