#include "BlockFall.h"
#include <sstream>
#include <iostream>
#include <fstream>

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}


vector<bool> BlockFall::parseLine(const string& line) {
    vector<bool> row;
    for (char c : line) {
        if (c == '0') {
            row.push_back(false);
        } else if (c == '1') {
            row.push_back(true);
        }
    }
    return row;
}


vector<vector<bool>> BlockFall::rotate_clockwise(const vector<vector<bool>>&original){
    size_t rows = original.size();
    size_t cols = original[0].size();

    vector<vector<bool>> rotated(cols, vector<bool>(rows, false));
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            rotated[j][i] = original[i][cols - j -1];
        }
    }
    return rotated;
}


vector<vector<bool>> BlockFall::rotate_counter_clockwise(const vector<vector<bool>>&original){
    size_t rows = original.size();
    size_t cols = original[0].size();

    vector<vector<bool>> rotated(cols,vector<bool>(rows,false));

    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j < cols; ++j){
            rotated[j][rows - i -1] = original[i][j];
        }
    }
    return rotated;
}


void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    ifstream file(input_file);
    if(!file.is_open()){
        cerr << "File cannot be opened!";
        exit(-1);
    }

    vector<vector<vector<bool>>> all_blocks;

    //string line;
    //Block* current_block = new Block();


    string line;
    while (getline(file, line)) {
        if (line[0] == '[') {
            // Start of a new block
            vector<vector<bool>> block;
            vector<bool> row = parseLine(line);
            // Add the row to the current block
            block.emplace_back(row);

            while(getline(file, line)){
                if (line.empty()){
                    break;
                }else{
                    // Add the row to the current block
                    block.emplace_back(parseLine(line));

                }

            }
            all_blocks.emplace_back(block);
        }
    }
    Block *current_Block = new Block();
    current_Block->shape = all_blocks[0];
    initial_block = current_Block;
    active_rotation = current_Block;
    /*
    initial_block = new Block();
    initial_block->shape = all_blocks[0];
    active_rotation = initial_block;*/

    Block* current_block = initial_block;
    if(!all_blocks.empty()){
        power_up = all_blocks[all_blocks.size()-1];
        all_blocks.pop_back();
    }
    for(int i = 0; i < all_blocks.size(); i++){
        // create circular doubly linked list for all blocks
        Block* left_block = new Block();
        left_block->shape = rotate_counter_clockwise(current_block->shape);
        Block* right_block = new Block();
        right_block->shape = rotate_clockwise(current_block->shape);

        // if the circular list has 1 elemnet
        if(*right_block == *current_block && *left_block == *current_block){
            current_block->left_rotation = current_block;
            current_block->right_rotation = current_block;
            delete left_block;
            delete right_block;

            // Link the rest of the blocks
            Block* new_block = new Block();
            if(i == all_blocks.size()-1){
                current_block->next_block = nullptr;
                delete new_block;
                break;
            }
            new_block->shape = all_blocks[i+1];
            current_block->next_block = new_block;
            current_block = new_block;
        }

            // if the circular list has 2 elemnets
        else if(*left_block == *right_block){
            current_block->left_rotation = right_block;
            current_block->right_rotation = right_block;
            right_block->left_rotation = current_block;
            right_block->right_rotation = current_block;
            delete left_block;

            // Link the rest of the blocks
            Block* new_block = new Block();
            if(i == all_blocks.size()-1){
                current_block->next_block = nullptr;
                delete new_block;
                break;
            }
            new_block->shape = all_blocks[i+1];
            current_block->next_block = new_block;
            right_block->next_block = new_block;
            current_block = new_block;
        }

            // if the circular list has 4 elemnets
        else{
            current_block->left_rotation = left_block;
            current_block->right_rotation = right_block;
            Block* new_right = new Block();
            new_right->shape = rotate_clockwise(right_block->shape);
            right_block->left_rotation = current_block;
            right_block->right_rotation = new_right;
            new_right->left_rotation = right_block;
            new_right->right_rotation = left_block;
            left_block->right_rotation = current_block;
            left_block->left_rotation = new_right;

            // Link the rest of the blocks
            Block* new_block = new Block();
            if(i == all_blocks.size()-1){
                current_block->next_block = nullptr;
                delete new_block;
                break;
            }
            new_block->shape = all_blocks[i+1];
            current_block->next_block = new_block;
            right_block->next_block = new_block;
            left_block->next_block = new_block;
            new_right->next_block = new_block;
            current_block = new_block;
        }
    }
    file.close();
}




void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    ifstream file(input_file);
    if(!file.is_open()){
        cerr << "File cannot be opened!";
        exit(-1);
    }
    string line;
    rows = 0;
    while(getline(file, line)){
        vector<int> row;
        stringstream str_stream(line);
        int value;
        while(str_stream >> value){
            row.emplace_back(value);
        }
        grid.emplace_back(row);
        cols = row.size();
        rows ++;
    }
    file.close();
}



BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block* current_block = initial_block;
    while (current_block != nullptr) {
        Block* block_to_delete = current_block;
        current_block = current_block->next_block;
        if(block_to_delete == block_to_delete->right_rotation){
            delete block_to_delete;
        }
        else if(block_to_delete->left_rotation == block_to_delete->right_rotation){
            delete block_to_delete->left_rotation;
            delete block_to_delete;

        }
        else{
            delete block_to_delete->right_rotation->right_rotation;
            delete block_to_delete->right_rotation;
            delete block_to_delete->left_rotation;
            delete block_to_delete;
        }
    }
}