#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace
{
void usage(std::string_view app_name) { std::cout << "Usage:\n  " << app_name << " file\n"; }

enum class Direction
{
    forward,
    up,
    down,
    invalid
};

using String_to_Direction_Map = std::unordered_map<std::string, Direction>;

String_to_Direction_Map const& getMap()
{
    static String_to_Direction_Map const the_map = {
        {"forward", Direction::forward},
        {"up", Direction::up},
        {"down", Direction::down},
    };
    return the_map;
}

Direction from_string(std::string const& string)
{
    auto const& the_map{getMap()};
    if (the_map.contains(string)) { return the_map.at(string); }
    return Direction::invalid;
}

std::istream& operator>>(std::istream& in, Direction& direction)
{
    std::string direction_as_string;
    in >> direction_as_string;

    direction = from_string(direction_as_string);
    return in;
}

struct Command
{
    Direction direction{Direction::invalid};
    long units{};
};

std::istream& operator>>(std::istream& in, Command& command)
{
    in >> command.direction >> command.units;
    return in;
}

using Commands = std::vector<Command>;
using MaybeCommands = std::optional<Commands>;

MaybeCommands read_commands_from_file(std::string_view file_path)
{
    std::ifstream infile(file_path.data());
    if (!infile.is_open())
    {
        std::cerr << "Can't open file: \"" << file_path << "\"\n";
        return std::nullopt;
    }

    std::istream_iterator<Command> start(infile), end;
    Commands commands(start, end);
    return commands;
}

struct Position
{
    long horizontal{};
    long depth{};
};

std::ostream& operator<<(std::ostream& out, Position const& position)
{
    out << "horizontal = " << position.horizontal << "; depth = " << position.depth;
    return out;
}

Position steer(Position const& position, Command command)
{
    switch (command.direction)
    {
    case Direction::forward:
        return {position.horizontal + command.units, position.depth};
    case Direction::up:
        return {position.horizontal, position.depth - command.units};
    case Direction::down:
        return {position.horizontal, position.depth + command.units};
    case Direction::invalid:
        return position;
    }
    return position;
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

    auto const commands = read_commands_from_file(args[1]);
    if (!commands.has_value()) return 1;

    std::cout << commands.value().size() << " commands read from file.\n";

    Position const start_position;
    std::cout << "Start position: " << start_position << '\n';
    auto const final_position =
        std::accumulate(commands->cbegin(), commands->cend(), start_position, steer);

    std::cout << "Final position: " << final_position << '\n';
    std::cout << "=> Antwort = " << final_position.horizontal * final_position.depth << '\n';
}
