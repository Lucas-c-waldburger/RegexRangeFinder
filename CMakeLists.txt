cmake_minimum_required(VERSION 3.24)
project(RegexRangeFinder)

set(CMAKE_CXX_STANDARD 17)

add_executable(RegexRangeFinder main.cpp RegAssist.cpp RegAssist.h FileReader.cpp FileReader.h RangeTools.h)
