#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <array>
#include <memory>

/* Summary:
 * This program gets the network interface, IP address, and network usage of the system.
 * It uses the /proc/net/dev file to get the network usage.
 * It uses the ip addr show command to get the IP address.
 * It uses the iwconfig command to get the network interface.
 */


// Get the network interface
std::string get_network_interface() {
    // Open the file /proc/net/dev
    std::ifstream interfaces("/proc/net/dev");
    std::string line, iface;

    // Read each line of the file
    while (std::getline(interfaces, line)) {
        
        // Create a string stream from the line
        std::istringstream iss(line);
        std::string name;

        // Check if the line starts with an interface name
        if (iss >> name) {
            name = name.substr(0, name.find(':'));
            if (name != "lo" && name.find("wlp") != std::string::npos || name.find("wlan") != std::string::npos || name.find("wlp3s0") != std::string::npos) {
                iface = name;
                break;
            }
        }
    }
    return iface;
}

// Get the IP address of the interface
std::string get_ip_address(const std::string &interface) {
    // Create a command to get the IP address
    std::string cmd = "ip addr show " + interface + " | grep 'inet ' | awk '{print $2}' | cut -d/ -f1";
    // Create a string to store the IP address
    std::string ip;
    // Create a buffer to store the output of the command
    std::array<char, 128> buffer;
    // Create a shared pointer to a FILE object
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    // Check if the pipe is valid
    if (!pipe) return "N/A";
    // Read the output of the command
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        // Add the output to the IP address
        ip += buffer.data();
    }
    // Return the IP address
    return ip;
}

// Get the network usage of the interface
void get_network_usage(const std::string &interface, long &rx_bytes, long &tx_bytes) {
    // Open the file /proc/net/dev
    std::ifstream dev_file("/proc/net/dev");
    // Create a string to store the line
    std::string line;

    // Read each line of the file
    while (std::getline(dev_file, line)) {
        // Check if the line contains the interface name
        if (line.find(interface) != std::string::npos) {
            // Create a string stream from the line
            std::istringstream iss(line);
            // Create a string to store the interface name
            std::string iface;
            // Check if the interface name is valid
            if (iss >> iface) {
                // Create a long to store the received bytes
                long rx;
                // Create a long to store the transmitted bytes
                long tx;
                // Read the received bytes
                iss >> rx >> std::ws;
                // Skip the next 7 words
                for (int i = 0; i < 7; ++i) iss >> std::ws;
                // Read the transmitted bytes
                iss >> tx;
                // Set the received and transmitted bytes
                rx_bytes = rx;
                tx_bytes = tx;
                break;
            }
        }
    }
}


// Main function
int main() {
    // Get the network interface
    std::string iface = get_network_interface();
    // Check if the interface is valid
    if (iface.empty()) {
        // Print an error message
        std::cerr << "No active network interface found.\n";
        // Return 1
        return 1;
    }

    // Get the IP address of the interface
    std::string ip = get_ip_address(iface);
    // Create a long to store the received bytes
    long rx_bytes;
    // Create a long to store the transmitted bytes
    long tx_bytes;
    // Get the network usage of the interface
    get_network_usage(iface, rx_bytes, tx_bytes);

    // Print the interface, IP address, received bytes, and transmitted bytes
    std::cout << "Interface: " << iface << "\n";
    std::cout << "IP Address: " << ip << "\n";
    std::cout << "Received Bytes: " << rx_bytes << "\n";
    std::cout << "Transmitted Bytes: " << tx_bytes << "\n";

    // Return 0
    return 0;
}
