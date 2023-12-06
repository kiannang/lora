#include "rclcpp/rclcpp.hpp"
#include "lora_interfaces/srv/receive_lo_ra.hpp"                                      
#include "lora_interfaces/srv/send_lo_ra.hpp" 

#include <memory>
#include <string>

void send(const std::shared_ptr<lora_interfaces::srv::SendLoRa::Request> request,
          std::shared_ptr<lora_interfaces::srv::SendLoRa::Response> response)
{
    std::string sendString = request->tosend;
    response->success = true; 
    std::string successStatement = "Error Occurred";
    if(response->success){
        successStatement = "Success";
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request to send the following: \n %s", sendString.c_str());
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Output: [%s]", successStatement.c_str());
}

void receive(const std::shared_ptr<lora_interfaces::srv::ReceiveLoRa::Request> request,
            std::shared_ptr<lora_interfaces::srv::ReceiveLoRa::Response> response){
    std::string receivedString = request->enter;
    response->received = receivedString;
    response->error = true; 
    std::string errorStatement = "Error Occurred";
    if (response->error){
        errorStatement = "Success";
  }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Recieved String: [%s]", receivedString.c_str());
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Output: [%s]", errorStatement.c_str());

}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    std::shared_ptr<rclcpp::Node> nodeSend = rclcpp::Node::make_shared("lora_send_server");
    std::shared_ptr<rclcpp::Node> nodeReceive = rclcpp::Node::make_shared("lora_receive_server");

    rclcpp::Service<lora_interfaces::srv::SendLoRa>::SharedPtr serviceSend =
        nodeSend->create_service<lora_interfaces::srv::SendLoRa>("lora_send", &send);
    
    rclcpp::Service<lora_interfaces::srv::ReceiveLoRa>::SharedPtr serviceReceive =
        nodeReceive->create_service<lora_interfaces::srv::ReceiveLoRa>("lora_receive", &receive);
    

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to Recieve or Send Data.");

  rclcpp::spin(nodeSend);
  rclcpp::spin(nodeReceive);
  rclcpp::shutdown();
}