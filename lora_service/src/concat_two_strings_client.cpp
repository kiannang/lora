#include "rclcpp/rclcpp.hpp"
#include "lora_interfaces/srv/concat_two_strings.hpp"                                      

#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  if (argc != 3) {
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "usage: concat_two_strings_client X Y");     
      return 1;
  }

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("concat_two_strings_client"); 
  rclcpp::Client<lora_interfaces::srv::ConcatTwoStrings>::SharedPtr client =               
    node->create_client<lora_interfaces::srv::ConcatTwoStrings>("concat_two_strings");         

  auto request = std::make_shared<lora_interfaces::srv::ConcatTwoStrings::Request>();      
  request->str1 = argv[1];
  request->str2 = argv[2];                                                         

  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  auto result = client->async_send_request(request);
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, result) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    std::string r = result.get()->concat;
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Concatenation: %s", r.c_str());
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service concat_two_strings");   
  }

  rclcpp::shutdown();
  return 0;
}