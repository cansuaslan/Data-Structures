#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    void handle_commands(const string &commands_file);
    bool print_grid(const BlockFall& game) const;
    void rotate_active_block(BlockFall& game, bool clockwise);
    bool move_active_block(BlockFall& game,bool right);
    void drop_active_block(BlockFall& game, bool is_it_switch);
    void toggle_gravity_mode(BlockFall& game, bool is_it_switch);
    bool is_it_fit(BlockFall& game);
    bool can_move(BlockFall& game, bool right);
    bool can_rotate(BlockFall& game, bool clockwise);
    void delete_rows(BlockFall& game, bool is_it_switch);
    bool check_power_up(BlockFall& game);
};


#endif //PA2_GAMECONTROLLER_H
