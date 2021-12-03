#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
void usage(std::string_view app_name) { std::cout << "Usage:\n  " << app_name << " file\n"; }

using Measurements = std::vector<int>;
using MaybeMeasurements = std::optional<Measurements>;

MaybeMeasurements read_measurements_from_file(std::string_view file_path)
{
    std::ifstream infile(file_path.data());
    if (!infile.is_open())
    {
        std::cerr << "Can't open file: \"" << file_path << "\"\n";
        return std::nullopt;
    }

    std::istream_iterator<int> start(infile), end;
    Measurements measurements(start, end);
    return measurements;
}

auto count_measurements(Measurements const& measurements)
{
    std::vector<std::pair<int, int>> pairs;
    pairs.reserve(measurements.size() - 1);

    std::transform(measurements.cbegin(), measurements.cend() - 1, measurements.cbegin() + 1,
                   std::back_inserter(pairs), [](int i, int j) { return std::make_pair(i, j); });
    return std::count_if(pairs.cbegin(), pairs.cend(),
                         [](std::pair<int, int> elem) { return elem.first < elem.second; });
}
} // namespace

int main(int argc, char* argv[])
{
    auto args = std::span<char*>(argv, argc);
    auto const app_name = std::filesystem::path(args[0]).filename().string();

    if (argc == 1)
    {
        std::cerr << app_name << ": file name missing\n";
        usage(app_name);
        return 1;
    }

    if (argc > 2)
    {
        std::cerr << app_name << ": too many arguments\n";
        usage(app_name);
        return 1;
    }

    std::cout << "**************\n"
              << "Advent of code\n"
              << "**************\n"
              << "Day 1 (Sonar Sweep) - Part 1\n\n";

    auto const measurements = read_measurements_from_file(args[1]);
    if (!measurements.has_value()) return 1;

    std::cout << measurements.value().size() << " measurements read from file.\n";

    auto const solution = count_measurements(measurements.value());
    std::cout << "The answer is: " << solution << '\n';
}
