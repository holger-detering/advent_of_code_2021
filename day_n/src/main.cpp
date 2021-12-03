#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string_view>

namespace
{
void usage(std::string_view app_name) { std::cout << "Usage:\n  " << app_name << " file\n"; }
} // namespace

int main(int argc, char* argv[])
{
    auto args = std::span<char*>(argv, argc);
    auto const app_name = std::filesystem::path(args[0]).filename().string();

    if (argc == 1)
    {
        std::cerr << app_name << ": file name missing" << std::endl;
        usage(app_name);
        return 1;
    }

    if (argc > 2)
    {
        std::cerr << app_name << ": too many arguments" << std::endl;
        usage(app_name);
        return 1;
    }

    // TODO:
    // 1. read input data
    std::ifstream infile(args[1]);
    if (!infile.is_open())
    {
        std::cerr << "Can't open file: \"" << args[1] << "\"" << std::endl;
        return 1;
    }

    // 2. solve puzzle
    // 3. print solution
}
