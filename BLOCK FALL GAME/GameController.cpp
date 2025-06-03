#include "GameController.h"
#include <iostream>
#include <fstream>

bool GameController::play(BlockFall& game, const string& commands_file){
    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    ifstream file(commands_file);
    if (!file.is_open()) {
        cerr << "Commands file cannot be opened!";
        exit(-1);
    }

    string command;
    int i = 0;
    bool is_it_switch = false;
    while (getline(file, command)) {
        if(!is_it_fit(game)){
            cout << "GAME OVER";
            break;
        }
        if (command == "PRINT_GRID") {
            print_grid(game);
        } else if (command == "ROTATE_RIGHT") {
            rotate_active_block(game, true);
        } else if (command == "ROTATE_LEFT") {
            rotate_active_block(game ,false);
        } else if (command == "MOVE_RIGHT") {
            move_active_block(game,true);
        } else if (command == "MOVE_LEFT") {
            move_active_block(game, false);
        } else if (command == "DROP") {
            drop_active_block(game, is_it_switch);
        } else if (command == "GRAVITY_SWITCH") {
            is_it_switch = true;
            toggle_gravity_mode(game, is_it_switch);
            is_it_switch = false;
        } else {
            cout << "Unknown command: " << command << endl;
        }
    }
    file.close();
    // Gameplay successful
    return true;
}


bool GameController::print_grid(const BlockFall& game) const{
    cout << "Score: " << game.current_score << endl;
    for(int i = 0; i < game.rows; ++i){
        for(int j = 0; j < game.cols; ++j){
            int start = game.active_rotation->starting_position;
            int block_row = i;
            int block_col = j-start;
            if(block_row >= 0 && block_row < game.active_rotation->shape.size() && block_col >= 0 && block_col < game.active_rotation->shape[0].size() && game.active_rotation->shape[block_row][block_col]){
                cout << occupiedCellChar;
            }else{
                int value = game.grid[i][j];
                if(value == 1){
                    cout << occupiedCellChar;
                }else{
                   cout << unoccupiedCellChar;
                }
            }
        } cout << endl;
    }
    return true;
    // Implement logic to print the grid
}

void GameController::rotate_active_block(BlockFall& game, bool clockwise) {
    // Implement logic to rotate the active block
    if(clockwise){
        //Rotate right
        if(can_rotate(game,clockwise)){
            game.active_rotation = game.active_rotation->right_rotation;
        }
    }else{
        if(can_rotate(game,clockwise)){
            game.active_rotation = game.active_rotation->left_rotation;
        }
        //Rotate left
    }
}

bool GameController::move_active_block(BlockFall& game,bool right) {
    if(can_move(game,right)){
        if(right){
            game.active_rotation->starting_position += 1;
        }else{
            game.active_rotation->starting_position -= 1;
        }
    }return true;
    // Implement logic to move the active block
}

void GameController::drop_active_block(BlockFall& game, bool is_it_switch) {

    // Implement logic to drop the active block
    int distance = 0;
    int temp_distance = 0;
    int row = game.active_rotation->shape.size();
    int col = game.active_rotation->shape[0].size();
    int true_values = 0;
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j) {
            if(game.active_rotation->shape[i][j] == 1 ){
                true_values += 1;
            }
        }
    }
    for(int i = 0; i < col; ++i){
        int shapeRow = 0;
        for (int j = row - 1 ; j >= 0 ; --j) {
            if (game.active_rotation->shape[j][i] == 1){
                shapeRow  = j;
                break;
            }
        }
        for(int k = shapeRow + 1 ; k < game.grid.size(); ++k){
            if(game.grid[k][i + game.active_rotation->starting_position] == 0 ){
                temp_distance += 1;
            }else if(game.grid[k][i + game.active_rotation->starting_position] == 1 ){
                break;
            }

        }
        if(distance == 0 || distance >= temp_distance){
            distance = temp_distance;
        }
        temp_distance = 0;
    }
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if(game.active_rotation->shape[i][j]==1){
                game.grid[i +  distance][j + game.active_rotation->starting_position] = game.active_rotation->shape[i][j];
            }

        }
    }
    if(game.gravity_mode_on){
        toggle_gravity_mode(game,is_it_switch);
        game.gravity_mode_on = true;
    }
    bool power_check = check_power_up(game);
    int grid_row = game.grid.size() ;
    int grid_col = game.grid[0].size();
    int power_point = 0;
    if(power_check){
        for (int i = 0; i < grid_row ; ++i) {
            for (int j = 0; j < grid_col; ++j) {
                if(game.grid[i][j] == 1){
                    power_point ++;
                    game.grid[i][j] = 0;
                }
            }
        }
        cout << "1000 + power_point: gr" << 1000 + power_point << endl;
        game.current_score += 1000 + power_point;
    }
    game.active_rotation = game.active_rotation->next_block;
    game.current_score += distance * true_values;
}

void GameController::toggle_gravity_mode(BlockFall& game, bool is_it_switch) {
    int row = game.grid.size();
    int col = game.grid[0].size();

    for (int i = 0; i < col; ++i) {
        int true_value = 0;
        for (int j = row - 1 ; j >= 0; --j) {
            if(game.grid[j][i] == 1){
                true_value += 1 ;
                game.grid[j][i] = 0;
            }
        }
        for (int j = row -1 ; j >= col - true_value ; --j) {
            game.grid[j][i] = 1;
        }
    }
    delete_rows(game,is_it_switch);
    game.gravity_mode_on = !game.gravity_mode_on;

}
bool GameController::is_it_fit(BlockFall& game){

    int row = game.active_rotation->shape.size();
    int col = game.active_rotation->shape[0].size();
    int revise = game.active_rotation->starting_position;
    if(revise + col < game.grid[0].size()){
        for(int i = 0; i < row; ++i){
            for(int j = 0; j < col; ++j){
                if(game.grid[i][revise + j] + game.active_rotation->shape[i][j] >= 2 ){
                    return false;
                }
            }
        }
        return true;
    }
    return true;
}

bool GameController::can_rotate(BlockFall& game, bool clockwise){
    if(clockwise){
        int rotate_row = game.active_rotation->right_rotation->shape.size();
        int rotate_col = game.active_rotation->right_rotation->shape[0].size();
        game.active_rotation->right_rotation->starting_position = game.active_rotation->starting_position;
        int revise = game.active_rotation->right_rotation->starting_position;
        if((rotate_col + game.active_rotation->right_rotation->starting_position) < game.grid[0].size()){
            for(int i = 0; i < rotate_row; ++i ){
                for(int j = 0; j < rotate_col; ++j){
                    if(game.grid[i][revise + j] == 1){
                        return false;
                    }
                }
            }return true;
        }
    }else{
        int rotate_row = game.active_rotation->left_rotation->shape.size();
        int rotate_col = game.active_rotation->left_rotation->shape[0].size();
        game.active_rotation->left_rotation->starting_position = game.active_rotation->starting_position;
        int revise = game.active_rotation->left_rotation->starting_position;
        if((rotate_col + game.active_rotation->left_rotation->starting_position) < game.grid[0].size()){
            for(int i = 0; i < rotate_row; ++i ){
                for(int j = 0; j < rotate_col; ++j){
                    if(game.grid[i][revise + j] == 1){
                        return false;
                    }
                }
            }return true;

        }
    }
}
bool GameController::can_move(BlockFall& game, bool right){
    int row = game.active_rotation->shape.size();
    int col = game.active_rotation->shape[0].size();
    int revise = game.active_rotation->starting_position;
    if(right && (revise + col ) < game.grid[0].size()){
        for(int i = 0; i < row; ++i){
            for(int j = 0; j < col; ++j){
                if(game.grid[i][revise + j + 1] == 1 && game.active_rotation->shape[i][revise +j + 1]){
                    return false;
                }
            }
        }return true;
    }else if( (revise -  1) > 0 && !right ){
        for(int i = 0; i < row; ++i){
            for(int j = 0; j < col; ++j){
                if(game.grid[i][revise + j -  1] == 1 && game.active_rotation->shape[i][revise +j - 1]){
                    return false;
                }
            }
        }return true;
    }return false;
}


void GameController::delete_rows(BlockFall& game, bool is_it_switch){

    vector<int> rows_to_delete;
    int row = game.grid.size();
    int col = game.grid[0].size();
    bool is_completed = true;
    for (int i = row - 1; i >= 0 ; --i) {
        is_completed = true;
        for (int j = 0; j < col; ++j) {
            cout << game.grid[i][j];
            if(game.grid[i][j] == 0){
                is_completed = false;
                break;
            }
        }
        if(is_completed){
            rows_to_delete.push_back(i);
        }
    }
    if(!rows_to_delete.empty()){
    for(int i = rows_to_delete.size() ; i > 0; --i){
        int row_index = rows_to_delete[i-1];
        game.grid.erase(game.grid.begin() + row_index);
    }
    for (int i = 0; i < rows_to_delete.size(); ++i) {
        game.grid.insert(game.grid.begin(), vector<int>(col, 0));
    }}
    if(!is_it_switch){
        game.current_score +=  col * rows_to_delete.size();
    }
}

bool GameController::check_power_up(BlockFall& game){
    int row = game.grid.size();
    int col = game.grid[0].size();
    int power_row = game.power_up.size();
    int power_col = game.power_up[0].size();
    for(int i = 0; i <= row - power_row; ++i){
        for (int j = 0; j < col - power_col; ++j) {
            bool match = true;
            for (int k = 0; k < power_row ; ++k) {
                for (int l = 0; l < power_col ; ++l) {
                    if(game.power_up[k][l] != (game.grid[i + k][j + l] != 0)){
                     match = false;
                     break;
                    }
                }
                if(!match) break;
            }
            if(match){
                return true;
            }
        }
    }
    return false;
}


