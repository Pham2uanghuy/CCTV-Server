cmake_minimum_required(VERSION 3.10)
project(CCTV_Server)

set(CMAKE_CXX_STANDARD 17)

# Include folder
include_directories(include)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Kafka libs
find_library(RDKAFKA_LIB rdkafka++)
find_library(RDKAFKA_C_LIB rdkafka)

# Source files
set(SOURCES
    src/main.cpp
    src/kafka_producer.cpp
    src/camera_module.cpp
)

# Main app build
add_executable(cctv_app ${SOURCES})
target_link_libraries(cctv_app ${OpenCV_LIBS} ${RDKAFKA_LIB} ${RDKAFKA_C_LIB})

# Test camera
add_executable(test_camera
    tests/test_camera.cpp
    src/camera_module.cpp
)
target_link_libraries(test_camera ${OpenCV_LIBS} ${RDKAFKA_LIB} ${RDKAFKA_C_LIB})

# Test producer
add_executable(test_producer
    tests/test_producer.cpp
    src/kafka_producer.cpp
)
target_link_libraries(test_producer ${OpenCV_LIBS} ${RDKAFKA_LIB} ${RDKAFKA_C_LIB})




