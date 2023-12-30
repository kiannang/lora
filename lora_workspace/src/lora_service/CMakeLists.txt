cmake_minimum_required(VERSION 3.5)
project(lora_service)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(src/third-party/include)
set(serial_SRCS src/third-party/src/serial.cc src/third-party/include/serial/serial.h 
src/third-party/include/serial/v8stdint.h src/third-party/src/impl/list_ports/list_ports_linux.cc)
list(APPEND serial_SRCS src/third-party/src/impl/unix.cc)

add_library(serial ${serial_SRCS})
target_link_libraries(serial rt)


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(lora_interfaces REQUIRED)

add_executable(server src/concat_two_strings_server.cpp)
ament_target_dependencies(server
  rclcpp lora_interfaces)

add_executable(client src/concat_two_strings_client.cpp)
ament_target_dependencies(client
  rclcpp lora_interfaces)

add_executable(loraserver src/LoRaService.cpp)
ament_target_dependencies(loraserver
  rclcpp lora_interfaces)
target_link_libraries(loraserver serial)

  #add_executable(arduino-communication test.cpp)
  #target_link_libraries(arduino-communication serial)

add_executable(loraclient src/LoRaClient.cpp)
ament_target_dependencies(loraclient
  rclcpp lora_interfaces)
target_link_libraries(loraclient serial)

install(TARGETS
  server
  client
  loraserver
  loraclient
  DESTINATION lib/${PROJECT_NAME})

ament_package()
