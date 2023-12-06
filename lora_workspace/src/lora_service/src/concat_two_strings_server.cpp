#include "rclcpp/rclcpp.hpp"
#include "lora_interfaces/srv/concat_two_strings.hpp"

#include <memory>
#include <string>

void add(const std::shared_ptr<lora_interfaces::srv::ConcatTwoStrings::Request> request,
          std::shared_ptr<lora_interfaces::srv::ConcatTwoStrings::Response> response)
{
  response->concat = request->str1 + request->str2;
 
  std::string firstStr = request->str1; 
  std::string secondStr = request->str2;   
  std::string concatStr = response->concat;

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request\nstr1: %s" " str2: %s",
                firstStr.c_str(), secondStr.c_str());
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response: [%s]", concatStr.c_str());
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("concat_two_strings_server");

  rclcpp::Service<lora_interfaces::srv::ConcatTwoStrings>::SharedPtr service =
    node->create_service<lora_interfaces::srv::ConcatTwoStrings>("concat_two_strings", &add);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to combine two strings.");

  rclcpp::spin(node);
  rclcpp::shutdown();
}