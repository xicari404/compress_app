cmake_minimum_required(VERSION 3.16)
project(compress_app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 COMPONENTS 
    Core 
    Gui 
    Widgets 
    Concurrent
    Sql
    REQUIRED
)

# Find compression libraries
find_package(zstd REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/diskanalyzer.cpp
    src/diskanalyzer.h
    src/compressionengine.cpp
    src/compressionengine.h
    src/navigationbar.cpp
    src/navigationbar.h
    src/step1_analysis.cpp
    src/step1_analysis.h
    src/step2_results.cpp
    src/step2_results.h
    src/step3_selection.cpp
    src/step3_selection.h
    src/step4_compression.cpp
    src/step4_compression.h
    src/step5_results.cpp
    src/step5_results.h
    src/database.cpp
    src/database.h
)

# Create executable
add_executable(compress_app ${SOURCES})

# Link libraries
target_link_libraries(compress_app 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::Concurrent
    Qt6::Sql
    zstd::zstd
)
