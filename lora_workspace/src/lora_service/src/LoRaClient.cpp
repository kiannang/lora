#include "rclcpp/rclcpp.hpp"
#include "lora_interfaces/srv/receive_lo_ra.hpp"                                      
#include "lora_interfaces/srv/send_lo_ra.hpp" 

#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  if (argc != 2) {
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "usage: lora_service X");     
      return 1;
  }

  std::shared_ptr<rclcpp::Node> nodeSend = rclcpp::Node::make_shared("lora_send_client"); 
  std::shared_ptr<rclcpp::Node> nodeReceive = rclcpp::Node::make_shared("lora_receive_client"); 

  rclcpp::Client<lora_interfaces::srv::SendLoRa>::SharedPtr clientSend =               
    nodeSend->create_client<lora_interfaces::srv::SendLoRa>("lora_send"); 

  rclcpp::Client<lora_interfaces::srv::ReceiveLoRa>::SharedPtr clientReceive =               
    nodeReceive->create_client<lora_interfaces::srv::ReceiveLoRa>("lora_receive"); 

  auto request = std::make_shared<lora_interfaces::srv::SendLoRa::Request>();      
  request->tosend = argv[1];                                                       

  while (!clientSend->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  auto result = clientSend->async_send_request(request);
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(nodeSend, result) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    std::string r = "";
    if (result.get()->success){
      r = "Success";
    }
    else
    {
      r = "Error";
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Output: %s", r.c_str());
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service lora_service");   
  }

  rclcpp::shutdown();
  return 0;
}