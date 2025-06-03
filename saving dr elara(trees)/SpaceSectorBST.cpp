#include "SpaceSectorBST.h"
#include <sstream>
#include <iostream>   // For cout, cerr
#include <fstream>    // For ifstream
#include <algorithm>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    ifstream inputFile(filename);
    if (inputFile.is_open()){
        string line;
        getline(inputFile,line);
        while (getline(inputFile,line)){
            istringstream iss(line);
            string x_str, y_str, z_str;
            if (getline(iss, x_str, ',') && getline(iss, y_str, ',') && getline(iss, z_str, ',')){
                int x = stoi(x_str);
                int y = stoi(y_str);
                int z = stoi(z_str);
                insertSectorByCoordinates(x,y,z);
                //Sector* sector = new Sector(x, y, z);
            }else {
                std::cerr << "Error parsing line: " << line << std::endl;
            }
        }
        inputFile.close();
    }
    else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
    // according to the given comparison critera based on the sector coordinates.
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the
    // coordinates-based comparison criteria.
    root = insert(root, nullptr, new Sector(x,y,z));
}

Sector* SpaceSectorBST::insert(Sector* node, Sector* parent, Sector* newSector){
    if(node == nullptr){
        newSector->parent = parent;
        return newSector;
    }
    if (newSector->x < node->x || (newSector->x == node->x  && newSector->y < node->y) || (newSector->x == node->x && newSector->y == node->y && newSector->z < node->z)){
        node->left = insert(node->left, node, newSector);
    } else{
        node->right = insert(node->right, node, newSector);
    }
    return node;
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    root = deleteNode(root,sector_code);
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inOrderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preOrderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postOrderTraversal(root);
    cout << endl;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* destinationSector = findNode(root,sector_code);
    while(destinationSector != nullptr){
        path.push_back(destinationSector);
        destinationSector = destinationSector->parent; 
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.
    if(path.size() == 0){
        cout  << "A path to Dr. Elara could not be found." << endl;
    }else{
        cout <<  "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size(); ++i) {
            cout << path[i]->sector_code;
            if(i < path.size() - 1 ){
                cout << "->";
            }
        }
    }

    cout << endl;
}

void SpaceSectorBST::inOrderTraversal(Sector* node) {
    if(node != nullptr){
        inOrderTraversal(node->left);
        cout << node->sector_code << endl;
        inOrderTraversal(node->right);
    }
}

void SpaceSectorBST::preOrderTraversal(Sector* node){
    if(node != nullptr) {
        cout << node->sector_code << endl;
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
}

void SpaceSectorBST::postOrderTraversal(Sector* node){
    if(node != nullptr){
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        cout << node->sector_code << endl;
    }
}

Sector* SpaceSectorBST::deleteNode(Sector *node, const std::string &sector_code){
    Sector* tobedeleted = findNode(root, sector_code);
    if(node == nullptr || tobedeleted == nullptr){
        return nullptr;
    }
    if (node->x < tobedeleted->x || (node->x == tobedeleted->x  && node->y < tobedeleted->y) || (node->x == tobedeleted->x && node->y == tobedeleted->y && node->z < tobedeleted->z)){
        node->right = deleteNode(node->right, sector_code);
    } else if (node->x > tobedeleted->x || (node->x == tobedeleted->x  && node->y > tobedeleted->y) || (node->x == tobedeleted->x && node->y == tobedeleted->y && node->z > tobedeleted->z)){
        node->left = deleteNode(node->left, sector_code);
    }else{
        if (node->left == nullptr && node->right == nullptr){
            delete node;
            return nullptr;
        } else if(node->left == nullptr){
            Sector* temp = node->right;
            temp->parent = node->parent;
            delete node;
            return temp;
        } else if(node->right == nullptr){
            Sector* temp = node->left;
            temp->parent = node->parent;
            delete node;
            return temp;
        }else{
            Sector* successor = FindInorderSuccessor(node->right);
            successor->parent = node->parent;
            if (successor->right) {
                successor->right->parent = successor->parent;
            }
            node->x = successor->x;
            node->y = successor->y;
            node->z = successor->z;
            node->distance_from_earth = successor->distance_from_earth;
            node->sector_code = successor->sector_code;
            node->right = deleteNode(node->right, successor->sector_code);
        }
    }
    return node;
}


Sector* SpaceSectorBST::FindInorderSuccessor(Sector* node){
    while(node->left != nullptr){
        node = node->left;
    }
    return node;
}

Sector* SpaceSectorBST::findNode(Sector* node, const std::string& sector_code) const{
    if(node == nullptr){
        return nullptr;
    }
    if(sector_code == node->sector_code){
        return node;
    }
    Sector* left = findNode(node->left,sector_code);
    if(left != nullptr){
        return left;
    }
    return findNode(node->right,sector_code);
}

void SpaceSectorBST::deleteTree(Sector* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}
