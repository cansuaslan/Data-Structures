#include <fstream>
#include <iostream>
#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry *new_entry) {
    // TODO: Implement the insert_new_entry function to insert a new leaderboard entry
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        while (current->next_leaderboard_entry && new_entry->score <= current->next_leaderboard_entry->score) {
            current = current->next_leaderboard_entry;
        }
        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
    }
    LeaderboardEntry *prev = nullptr;
    LeaderboardEntry *current = head_leaderboard_entry;
    int count = 0;
    while (count < MAX_LEADERBOARD_SIZE && current) {
        prev = current;
        current = current->next_leaderboard_entry;
        count++;
    }

    if (prev) {
        prev->next_leaderboard_entry = nullptr;
        delete current;
    }
}

void Leaderboard::write_to_file(const string &filename) {
    // TODO: Implement the write_to_file function to write the leaderboard to a file
    ofstream outputFile(filename);
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        outputFile << current->score << " " << current->last_played << " " << current->player_name << endl;
        current = current->next_leaderboard_entry;
    }
    outputFile.close();
}

void Leaderboard::read_from_file(const string &filename) {
    // TODO: Implement the read_from_file function to read the leaderboard from a file
    ifstream inputFile(filename);
    if (inputFile.is_open()) {
        LeaderboardEntry *prev = nullptr;
        while (!inputFile.eof()) {
            unsigned long score;
            time_t lastPlayed;
            string playerName;
            inputFile >> score >> lastPlayed >> playerName;
            LeaderboardEntry *newEntry = new LeaderboardEntry(score, lastPlayed, playerName);
            if (prev) {
                prev->next_leaderboard_entry = newEntry;
            } else {
                head_leaderboard_entry = newEntry;
            }
            prev = newEntry;
        }
        inputFile.close();
    }
}

void Leaderboard::print_leaderboard() {
    // TODO: Implement the print_leaderboard function to print the current leaderboard
    LeaderboardEntry *current = head_leaderboard_entry;
    int rank = 1;
    while (current) {
        std::cout << "Rank: " << rank << " Score: " << current->score << " Last Played: " << current->last_played << " Player: "
                  << current->player_name << std::endl;
        current = current->next_leaderboard_entry;
        rank++;
    }
}

Leaderboard::~Leaderboard() {
    // TODO: Implement the destructor for freeing dynamically allocated memory
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current) {
        LeaderboardEntry *next = current->next_leaderboard_entry;
        delete current;
        current = next;
    }
}