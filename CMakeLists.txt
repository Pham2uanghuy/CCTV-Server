# cmake_minimum_required(VERSION 3.10)
# project(rtsp_to_kafka)

# set(CMAKE_CXX_STANDARD 17)

# find_package(OpenCV REQUIRED)

# include_directories(${OpenCV_INCLUDE_DIRS})

# add_executable(rtsp_to_kafka main.cpp)

# target_link_libraries(rtsp_to_kafka
#     ${OpenCV_LIBS}
#     rdkafka
#     rdkafka++
# )


cmake_minimum_required(VERSION 3.10)
project(camera_viewer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(camera_viewer camera_viewer.cpp)
target_link_libraries(camera_viewer ${OpenCV_LIBS})

