cmake_minimum_required(VERSION 3.5)
project(lora_service)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(lora_interfaces REQUIRED)

add_executable(server src/concat_two_strings_server.cpp)
ament_target_dependencies(server
  rclcpp lora_interfaces)

add_executable(client src/concat_two_strings_client.cpp)
ament_target_dependencies(client
  rclcpp lora_interfaces)

install(TARGETS
  server
  client
  DESTINATION lib/${PROJECT_NAME})

ament_package()