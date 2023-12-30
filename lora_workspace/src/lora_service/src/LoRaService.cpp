//ROS 2 include
#include "rclcpp/rclcpp.hpp"
//LORA SRV include
#include "lora_interfaces/srv/receive_lo_ra.hpp"                                      
#include "lora_interfaces/srv/send_lo_ra.hpp" 
//SERIAL include
#include "third-party/include/serial/serial.h"


//C++ includes
#include <memory>
#include <string>
#include <iostream>
#include "thread"

void send(const std::shared_ptr<lora_interfaces::srv::SendLoRa::Request> request,
          std::shared_ptr<lora_interfaces::srv::SendLoRa::Response> response)
{
    //start of serial setup  
    serial::Serial lora_serial("/dev/pts/1", 9600, serial::Timeout::simpleTimeout(3000)); //creation of serial object: (port "COM6", bautrate (should match in arduino code), timeout "3 seconds")
    
    if (lora_serial.isOpen()) //checking if serial connection is open
    {
        std::cout << "Serial port opened succesfully" << std::endl;
    }
    else
    {
        std::cout << "Serial port failed to open" << std::endl;
        exit(1);
    }
    lora_serial.flushOutput(); //to clear everything from the previous writing? // not sure if needed
    // initial serial write could get lost so we send messages multiple times b/c of arduino timing  

    std::string sendString = request->tosend + "\n"; //arduino uses new line "\n" to indicate end of message
    // write the string to the serial to send to the arduino here 
    for (int i = 0; i < 3; i++) //sent multiple times because of timing issue with arduino to linux
        {
            size_t bytesWritten = lora_serial.write(sendString);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            response->success = false;
            if (bytesWritten > 0){ // if more than 0 bytes were able to be written then send was successful
                response->success = true; 
            } 
        }
    
    // setting the error statement to print out 
    std::string successStatement = "Error Occurred";
    if(response->success){
        successStatement = "Success";
    }
    // printing out to terminal request to send and the output 
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request to send the following: %s", sendString.c_str());
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Output: [%s]", successStatement.c_str());
}

void receive(const std::shared_ptr<lora_interfaces::srv::ReceiveLoRa::Request> request,
            std::shared_ptr<lora_interfaces::srv::ReceiveLoRa::Response> response){
    //response->received = string read from serial;
    response->received = "string read";
    response->error = true; 
    std::string receivedString = response->received;
    std::string errorStatement = "Error Occurred";
    if (response->error){
        errorStatement = "Success";
  }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Received String: [%s]", receivedString.c_str());
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Output: [%s]", errorStatement.c_str());

}

int main(int argc, char **argv)
{
    //start of ROS services 
    rclcpp::init(argc, argv);

    std::shared_ptr<rclcpp::Node> nodeSend = rclcpp::Node::make_shared("lora_send_server");
    std::shared_ptr<rclcpp::Node> nodeReceive = rclcpp::Node::make_shared("lora_receive_server");

    rclcpp::Service<lora_interfaces::srv::SendLoRa>::SharedPtr serviceSend =
        nodeSend->create_service<lora_interfaces::srv::SendLoRa>("lora_send", &send);
    
    rclcpp::Service<lora_interfaces::srv::ReceiveLoRa>::SharedPtr serviceReceive =
        nodeReceive->create_service<lora_interfaces::srv::ReceiveLoRa>("lora_receive", &receive);
    

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to Receive or Send Data.");

  rclcpp::spin(nodeSend);
  rclcpp::spin(nodeReceive);
  rclcpp::shutdown();
}
