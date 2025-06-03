#include "Network.h"
#include <algorithm>


Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                               const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    for(const auto &command  : commands ){
        print_dashed_commands(command);
        if(command.substr(0,7) == "MESSAGE"){
            string sender_id, receiver_id, message_content;
            size_t startPos = command.find("#");
            size_t endPos = command.rfind("#");
            sender_id = command.substr(8,1);
            receiver_id = command.substr(10,1);
            message_content = command.substr(startPos+1, endPos - startPos - 1);
            cout << "Message to be sent: " << message_content << endl << endl;
            string receiver_mac;
            int number_of_hops = 0;
            for (Client& client : clients) {
                if (client.client_id == sender_id) {
                    for (auto& client_receiver : clients) {
                        if(client_receiver.client_id == receiver_id){
                            for(Client &mac_client : clients){
                                if(client.routing_table[client_receiver.client_id] == mac_client.client_id){
                                    receiver_mac = mac_client.client_mac;
                                    break;
                                }
                            }
                            int pieces = (message_content.length() + message_limit - 1) / message_limit;
                            for (int i = 0; i < pieces; ++i) {
                                stack<Packet*> packet_stack;
                                int starting_index = i * message_limit;
                                int piece_size = min(message_limit,  static_cast<int>(message_content.length() - starting_index));
                                string message_piece = message_content.substr(starting_index,piece_size);
                                Packet* app_packet = new ApplicationLayerPacket(0,sender_id,receiver_id,message_piece);
                                Packet* transport_packet = new TransportLayerPacket(1, sender_port,receiver_port);
                                Packet* network_packet = new NetworkLayerPacket(2, client.client_ip,client_receiver.client_ip);
                                Packet* physical_packet = new PhysicalLayerPacket(3, client.client_mac, receiver_mac);
                                packet_stack.push(app_packet);
                                packet_stack.push(transport_packet);
                                packet_stack.push(network_packet);
                                packet_stack.push(physical_packet);
                                cout << "Frame #" << i + 1 << endl;
                                cout << "Sender MAC address: " << client.client_mac << ", Receiver MAC address: " << receiver_mac << endl;
                                cout << "Sender IP address: " << client.client_ip << ", Receiver IP address: " << client_receiver.client_ip << endl;
                                cout << "Sender port number: " << sender_port << ", Receiver port number: " << receiver_port << endl;
                                cout << "Sender ID: " << sender_id << ", Receiver ID: " << receiver_id << endl;
                                cout << "Message chunk carried: \"" << message_piece << "\"" << endl;
                                cout << "Number of hops so far: " << number_of_hops << endl;
                                cout << "--------" << endl;
                                client.outgoing_queue.push(packet_stack);

                            }
                            auto now = std::chrono::system_clock::now();
                            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                            std::tm tm_now = *std::localtime(&now_c);
                            std::stringstream ss;
                            ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
                            std::string str_time = ss.str();
                            ActivityType activity = ActivityType::MESSAGE_SENT;
                            Log log(str_time, message_content,pieces, number_of_hops, sender_id, receiver_id,false, activity);
                            client.log_entries.push_back(log);
                        }
                    }
                }
            }
        }
        else if(command == "SEND"){
            for(Client& client : clients){
                if(!client.log_entries.empty()){
                    for (Log& log : client.log_entries ) {

                    }
                }
            }
        }
        else if(command == "RECEIVE"){
            //cout << command << " yazdır" << endl;
        }
        else if(command.substr(0,15) == "SHOW_FRAME_INFO"){
            string client_id, queue_type;
            string frame_str;
            client_id = command.substr(16,1);
            if(command.substr(18,2) == "in"){
                queue_type = "in";
                frame_str = command.substr(21);
            }else if (command.substr(18,3) == "out"){
                queue_type = "out";
                frame_str = command.substr(22);
            }
            for (Client& client : clients){
                if(client.client_id == client_id){
                    queue<stack<Packet*>> temp_queue;
                    stack<Packet*> selected_stack;
                    stack<Packet*> temp_stack;
                    int frame_number = stoi(frame_str);
                    if(queue_type == "in" && !client.incoming_queue.empty()){
                        temp_queue = client.incoming_queue;
                    } else if(queue_type == "out" && !client.outgoing_queue.empty()){
                        temp_queue = client.outgoing_queue;
                    }
                    if(frame_number > 0 && frame_number <= temp_queue.size()){
                        int frame = 1;
                        while(!temp_queue.empty()){
                            if(frame == frame_number){
                                selected_stack = temp_queue.front();
                                break;
                            }
                            temp_queue.pop();
                            frame ++;
                        }
                        while (!selected_stack.empty()) {
                            temp_stack.push(selected_stack.top());
                            selected_stack.pop();
                        }
                        if(temp_stack.top()->layer_ID == 0){
                            cout << "Carried Message: " << "" << dynamic_cast<ApplicationLayerPacket*>(temp_stack.top())->message_data << endl;

                        }
                        cout << "Layer 0 info: " ;
                        temp_stack.top()->print();
                        temp_stack.pop();
                        cout << "Layer 1 info: " ;
                        temp_stack.top()->print();
                        temp_stack.pop();
                        cout << "Layer 2 info: " ;
                        temp_stack.top()->print();
                        temp_stack.pop();
                        cout << "Layer 3 info: " ;
                        temp_stack.top()->print();
                        temp_stack.pop();
                    } else{
                        cout << "No such frame." << endl;
                    }
                }
            }
        }
        else if(command.substr(0,11) == "SHOW_Q_INFO"){
            string client_id, queue_type;
            client_id = command.substr(12,1);
            queue_type = command.substr(14);
            int stack_number = 1;
            int frame_numbers = 0 ;
            for (const Client& client : clients) {
                if (client.client_id == client_id) {
                    queue<stack<Packet*>> selected_queue;
                    if (queue_type == "in") {
                        selected_queue = client.incoming_queue;
                    } else if (queue_type == "out") {
                        selected_queue = client.outgoing_queue;
                    }
                    for(const Log& log : client.log_entries){
                        frame_numbers += log.number_of_frames;
                    }
                    cout << "Client " << client_id << " " << (queue_type == "in" ? "Incoming" : "Outgoing") << " Queue Status" << endl;
                    cout << "Current total number of frames: " << frame_numbers << endl;
                }
            }
        }
        else if(command.substr(0,9) == "PRINT_LOG"){
            string client_name = command.substr(10);
            cout << client_name << endl;
            int log_entry = 1;
            for (Client& client : clients) {
                if(client.client_id == client_name){
                    cout << "Client " << client_name << " Logs:" << endl;
                    for(const Log& log : client.log_entries){
                        cout << "--------------" << endl;
                        cout << "Log Entry #" << log_entry << endl;
                        cout << "Activity: " << static_cast<bool>(log.activity_type) << endl;
                        cout << "Timestamp: " << log.timestamp << endl;
                        cout << "Number of frames: " << log.number_of_frames << endl;
                        cout << "Number of hops: " << log.number_of_hops << endl;
                        cout << "Sender ID: " << log.sender_id << endl;
                        cout << "Receiver ID: " << log.receiver_id << endl;
                        cout << "Success: " << log.success_status << endl;
                        cout << "Message: \"" << log.message_content << "\"" << endl;
                        log_entry ++;
                    }
                }
            }
            //cout << command << " yazdır" << endl;
        }
        else{
            cout << "Invalid command." << endl;
        }

    }
}

vector<Client> Network::read_clients(const string &filename) {
    // TODO: Read clients from the given input file and return a vector of Client instances.
    vector<Client> clients;
    ifstream file(filename);
    if(file.is_open()){
        string line;
        getline(file,line);
        while(getline(file,line)){
            istringstream iss(line);
            string id;
            string ipAddress, data;
            if(iss >> id >> ipAddress >> data){
                clients.emplace_back(id,ipAddress,data);
            }else{
                cerr << "Error reading line: " << line << endl;
            }
        }
        file.close();
    }else{
        cerr << "Unable to open file: " << filename << endl;
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    ifstream file(filename);

    if(file.is_open()){
        string line;
        int i = 0;
        while(getline(file,line)){
            if(line == "-"){
                i++;
                continue;
            }
            istringstream iss(line);
            string receiverID, nextHopID;
            iss >> receiverID >> nextHopID;
            clients[i].routing_table[receiverID] = nextHopID;
        }
        file.close();
    }else{
        cerr << "Unable to open file: " << filename << endl;
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    // TODO: Read commands from the given input file and return them as a vector of strings.
    vector<string> commands;
    ifstream file(filename);
    if(file.is_open()){
        string line;
        getline(file,line);
        while(getline(file,line)){
            commands.push_back(line);
        }
        file.close();
    }else{
        cerr << "Unable to open file: " << filename << endl;
    }
    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}


void Network::print_clients(const std::vector<Client>& clients) const {
    for (const auto& client : clients) {
        std::cout << client;
    }
}

void Network::print_routing_tables(const std::vector<Client>& clients) const {
    for (const auto& client : clients) {
        std::cout << "Client ID: " << client.client_id << " Routing Table:\n";
        for (const auto& entry : client.routing_table) {
            std::cout << "  Receiver ID: " << entry.first << " Next Hop ID: " << entry.second << "\n";
        }
        std::cout << "\n";
    }
}

void Network::print_commands(const std::vector<std::string>& commands) const {
    for (const auto& command : commands) {
        std::cout << command << std::endl;
    }
}
void Network::print_dashed_commands(const std::string& command){
    int length = command.length() + 9;
    cout << string(length, '-') << endl;
    cout << "Command: " << command << endl;
    cout << string(length, '-') << endl;

}
