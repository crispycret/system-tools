#include <iostream>
#include <cstdio>
#include <memory>
#include <array>
#include <string>
#include <algorithm>


// Argument 1: Command to execute
// Optional argument 2: Number of lines to print



/* Summary:
 * This program executes a command and prints its output.
 * It uses popen to run the command and fgets to read the output.
 * The output is stored in a string and printed.
 */


struct Argument {
    std::string name;
    std::string prefix;
    std::string value;
};


struct OptionalArgument {
    std::string name;
    std::string prefix;
    std::string value;
};


// Optional Arguments come efore positional arguments
// Optional Arguments are preceded by a dash
// Optional Arguments can be followed by a value
// Optional Arguments can be followed by a space and then a value



std::array<OptionalArgument, 3> optArgs = {
    OptionalArgument{"-h", "--help", "Print help"},
    OptionalArgument{"-v", "--version", "Print version"},
    OptionalArgument{"-o", "--output", "Print output"}
};

std::array<Argument, 3> args = {
    Argument{"-h", "--help", "Print help"},
    Argument{"-v", "--version", "Print version"},
    Argument{"-o", "--output", "Print output"}
};

std::array<ActionableArgument, 3> actionableArguments = {



OptionalArgument parse_optional_argument(const std::string& arg) {
    OptionalArgument optional_argument;
    optional_argument.name = arg;
    optional_argument.prefix = arg.substr(0, arg.find('='));
    optional_argument.value = arg.substr(arg.find('=') + 1);
    return optional_argument;
}





int main(int argc, char* argv[]) {

    // Check if the command is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    // Command to execute
    const char* cmd = "echo $(iwconfig 2>/dev/null | grep -B 1 \"ESSID\" | grep -v \"no wireless extensions\" | awk 'NR % 2 == 1 {print $1}')";

    // Create a buffer to hold the command output
    std::array<char, 512> buffer;
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

    // Remove the newline character from the result
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    // Print the result
    std::cout << result << std::endl;

    return 0;
}