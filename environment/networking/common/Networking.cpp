#include <sstream>

#include "Command.hpp"
#include "Halite.hpp"
#include "Logging.hpp"
#include "error/BotError.hpp"
#include "error/NetworkingError.hpp"

namespace net {

using namespace hlt;

/** Maximum permissible length for user name in characters, truncated past this. */
constexpr auto NAME_MAX_LENGTH = 30;

/**
 * Launch the bot for a player, send the initial game information to the player, and update its name.
 * Safe to invoke from multiple threads on different players.
 *
 * @param player The player to communicate with.
 */
void Networking::initialize_player(Player &player) {
    Logging::log("Initializing player " + std::to_string(player.player_id) + " with command " + player.command);
    auto connection = connection_factory.new_connection(player.command);
    std::stringstream message_stream;
    Logging::log("Sending init message to player " + std::to_string(player.player_id));
    // Send the number of players and player ID
    message_stream << game.players.size()
                   << " " << player.player_id << std::endl;
    // Send each player's ID and factory location
    for (const auto &[player_id, other_player] : game.players) {
        message_stream << player_id
                       << " " << other_player.factories.front()
                       << std::endl;
    }
    // Send the map
    message_stream << game.game_map;
    connection->send_string(message_stream.str());
    Logging::log("Init message sent to player " + std::to_string(player.player_id));
    // Receive a name from the player.
    player.name = connection->get_string().substr(0, NAME_MAX_LENGTH);
    Logging::log("Init message received from player " + std::to_string(player.player_id) + ", name: " + player.name);
    {
        std::lock_guard<std::mutex> guard(connections_mutex);
        connections[player] = std::move(connection);
    }
}

/**
 * Handle the networking for a single frame, obtaining commands from the player if there are any.
 * Safe to invoke from multiple threads on different players.
 *
 * @param player The player to communicate with.
 * @return The commands from the player.
 */
std::vector<Command> Networking::handle_frame(Player &player) {
    std::vector<Command> commands;
    std::stringstream message_stream;
    std::string received_input;
    bool read_input = false;

    try {
        // Send the turn number, then each player in the game.
        message_stream << game.turn_number << std::endl;
        for (const auto &[_, other_player] : game.players) {
            message_stream << other_player;
        }
        connections[player]->send_string(message_stream.str());
        Logging::log("Turn info sent to player " + std::to_string(player.player_id), Logging::Level::Debug);
        // Get commands from the player.
        received_input = connections[player]->get_string();
        read_input = true;
        std::istringstream command_stream(received_input);
        Command command;
        while (command_stream >> command) {
            commands.push_back(std::move(command));
        }
        command_stream >> command;
        Logging::log("Received " + std::to_string(commands.size()) + " commands from player " + std::to_string(player.player_id), Logging::Level::Debug);
    }
    catch (const BotError& e) {
        player.log_error(e.what());
        if (read_input) {
            player.log_error("Last input received was:");
            player.log_error(received_input);
        }
        throw;
    }
    catch (const NetworkingError& e) {
        player.log_error(e.what());
        if (read_input) {
            player.log_error("Last input received was:");
            player.log_error(received_input);
        }
        throw;
    }

    return commands;
}

}
