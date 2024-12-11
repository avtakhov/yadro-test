cmake_minimum_required(VERSION 3.22)
project(yadro-parse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS json)

add_executable(
        yadro-parse 
        producers/txtfile.cpp
        producers/binfile.cpp
        producers/jsonfile.cpp
        http_session.cpp
        send_points.cpp
        main.cpp
)

target_include_directories(yadro-parse PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/producers
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(yadro-parse Boost::json)
