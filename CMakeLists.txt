cmake_minimum_required(VERSION 3.28)
project(WAVAnalyzer)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
find_package(FFTW3 CONFIG REQUIRED)

add_executable(WAVAnalyzer main.cpp)

target_include_directories(WAVAnalyzer PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(WAVAnalyzer PRIVATE FFTW3::fftw3 ${CURSES_LIBRARIES})
