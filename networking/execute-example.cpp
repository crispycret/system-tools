#include <iostream>
#include <cstdio>
#include <memory>
#include <array>

int main() {
    // Command to execute
    const char* cmd = "ls -l";

    // Create a buffer to hold the command output
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to run the command and read its output
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        std::cerr << "popen() failed!" << std::endl;
        return 1;
    }

    // Read the output from the pipe and store it in result
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Print the command output
    std::cout << "Command output:\n" << result << std::endl;

    return 0;
}
