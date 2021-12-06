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

#include <range/v3/all.hpp>

namespace rg3 = ranges;
namespace rv3 = ranges::views;

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

auto sum_up_three_sliding_window(Measurements const& measurements)
{
    return measurements | rv3::sliding(3) |
           rv3::transform([](auto&& window) { return rg3::accumulate(window, 0); }) |
           rg3::to<std::vector>();
}

auto count_measurements(Measurements const& measurements)
{
    return rg3::count_if(rv3::zip(measurements, measurements | rv3::drop(1)),
                         [](auto&& pair) { return pair.first < pair.second; });
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
              << "Day 1 (Sonar Sweep) - Part 2\n\n";

    auto const measurements = read_measurements_from_file(args[1]);
    if (!measurements.has_value()) return 1;

    std::cout << measurements.value().size() << " measurements read from file.\n";

    auto const sliding_window = sum_up_three_sliding_window(measurements.value());
    std::cout << "size of sliding window = " << sliding_window.size() << '\n';

    auto const solution = count_measurements(sliding_window);
    std::cout << "The answer is: " << solution << '\n';
}
