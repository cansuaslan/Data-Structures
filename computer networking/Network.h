#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                          const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename);
    void print_clients(const std::vector<Client>& clients) const;
    void print_routing_tables(const std::vector<Client>& clients) const;
    void print_commands(const std::vector<std::string>& commands) const;
    void print_dashed_commands(const std::string& command);
};

#endif  // NETWORK_H
