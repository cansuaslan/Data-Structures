#include "SpaceSectorLLRBT.h"
#include <iomanip>

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    ifstream file(filename);
    if(file.is_open()){
        string line;
        getline(file, line);
        while(getline(file, line)){
            istringstream iss(line);
            string x_str, y_str, z_str;
            if (getline(iss, x_str, ',') && getline(iss, y_str, ',') && getline(iss, z_str, ',')){
                int x = stoi(x_str);
                int y = stoi(y_str);
                int z = stoi(z_str);
                insertSectorByCoordinates(x,y,z);
            }else {
                std::cerr << "Error parsing line: " << line << std::endl;
            }
        }
        file.close();
    }

}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteTree(root);
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map
    // according to the coordinates-based comparison criteria.
    root = insert(root, nullptr, new Sector(x,y,z));
    root->color = false;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal:" << endl;
    inOrderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal:" << endl;
    preOrderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal:" << endl;
    postOrderTraversal(root);
    cout << endl;
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    std::vector<Sector*> earthPath;
    std::vector<Sector*> finalPath;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* destinationSector = findNode(root,sector_code);
    Sector* earth = findNode(root,"0SSS");
    while(earth != nullptr){
        earthPath.push_back(earth);
        earth = earth->parent;
    }
    while(destinationSector != nullptr){
        path.push_back(destinationSector);
        destinationSector = destinationSector->parent;
    }
    if (path.size() != 0){
        Sector* commonParent = nullptr;
        int hold_i = 0;
        int hold_j = 0;
        for (int i = 0; i < earthPath.size(); ++i) {
            for (int j = path.size() - 1 ; j >= 0 ; --j) {
                if(earthPath[i]->sector_code == path[j]->sector_code){
                    hold_i = i;
                    hold_j = j;
                    commonParent = earthPath[i];
                    break;
                }
            }
            if(commonParent != nullptr){
                break;
            }
        }
        for (int i = 0; i <= hold_i; ++i) {
            finalPath.push_back(earthPath[i]);
        }
        if(hold_j != 0){
            for (int i = hold_j -1 ; i >= 0 ;--i){
                finalPath.push_back(path[i]);
            }
        }
        cout << endl;
    }
    return finalPath;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.
    if(path.size() == 0){
        cout  << "A path to Dr. Elara could not be found." << endl;
    }else{
        cout <<  "The stellar path to Dr. Elara: ";
    }
    for (int i = 0; i < path.size() ; ++i) {
        cout << path[i]->sector_code;
        if(i < path.size() -1 ){
            cout << "->";
        }
    }
    cout << endl;
}


Sector* SpaceSectorLLRBT::insert(Sector* node, Sector* parent, Sector* newSector){
    if(node == nullptr){
        newSector->parent = parent;
        return newSector;
    }
    if(newSector->x < node->x || (newSector->x == node->x && newSector->y < node->y) || (newSector->x == node->x && newSector->y == node->y && newSector->z < node->z
    )){
        node->left = insert(node->left, node, newSector);
    }else{
        node->right = insert(node->right, node, newSector);
    }
    fixViolations(node);
    return node;
}

void SpaceSectorLLRBT::fixViolations(Sector*& node) {
    if(node == nullptr){
        return;
    }
    if(isRed(node->right) && !isRed(node->left)){
        node = rotateLeft(node);
    }
    if(isRed(node->left) && isRed(node->left->left)){
        node = rotateRight(node);
    }
    if(isRed(node->left) && isRed(node->right)){
        flipColors(node);
    }
    fixViolations(node->left);
    fixViolations(node->right);
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector*& node){
    Sector* child = node->right;
    child->parent =node->parent;
    node->parent = child;
    if(child->left != nullptr){
        child->left->parent = node;
    }
    node->right = child->left;
    child->left = node;
    child->color = node->color;
    node->color = RED;
    return child;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector*& node){
    Sector* child = node->left;
    child->parent =node->parent;
    node->parent = child;
    if(child->right != nullptr){
        child->right->parent = node;
    }
    node->left = child->right;
    child->right = node;
    child->color = node->color;
    node->color = RED;
    return child;
}


void SpaceSectorLLRBT::flipColors(Sector* node){
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;
}

void SpaceSectorLLRBT::inOrderTraversal(Sector* node) {
    if(node != nullptr){
        inOrderTraversal(node->left);
        printSector(node);
        inOrderTraversal(node->right);
    }
}

void SpaceSectorLLRBT::preOrderTraversal(Sector* node){
    if(node != nullptr) {
        printSector(node);
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
}

void SpaceSectorLLRBT::postOrderTraversal(Sector* node){
    if(node != nullptr){
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        printSector(node);
    }
}

bool SpaceSectorLLRBT::isRed(Sector* node) const {
    return (node != nullptr) && (node->color == RED);
}

void SpaceSectorLLRBT::printSector(Sector* node){
    if (node->color == RED) {
        std::cout << "RED sector: " << node->sector_code << std::endl;
    } else {
        std::cout << "BLACK sector: " << node->sector_code << std::endl;
    }
}

Sector* SpaceSectorLLRBT::findNode(Sector* node, const std::string& sector_code){
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


void SpaceSectorLLRBT::deleteTree(Sector* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

// Add this function definition to SpaceSectorBST.cpp
void SpaceSectorLLRBT::printTreeStructure() {
    printTreeStructure(root, 0);
}

// Helper function for recursive printing of tree structure
void SpaceSectorLLRBT::printTreeStructure(Sector* node, int indent) {
    if (node != nullptr) {
        // Print right subtree
        printTreeStructure(node->right, indent + 4);

        // Print current node
        std::cout << std::setw(indent) << " ";
        std::cout << node->sector_code << std::endl;

        // Print left subtree
        printTreeStructure(node->left, indent + 4);
    }
}