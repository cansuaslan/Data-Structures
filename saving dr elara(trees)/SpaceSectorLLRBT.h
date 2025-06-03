#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* insert(Sector* node, Sector* parent, Sector* newSector);
    void fixViolations(Sector*& node);
    bool isRed(Sector* node) const;
    Sector* rotateLeft(Sector*& node);
    Sector* rotateRight(Sector*& node);
    void flipColors(Sector* node);
    void inOrderTraversal(Sector* node);
    void postOrderTraversal(Sector* node);
    void preOrderTraversal(Sector* node);
    void printSector(Sector* node);
    Sector* findNode(Sector* node, const std::string& sector_code);
    void printTreeStructure(Sector* node, int indent);
    void printTreeStructure();
    void deleteTree(Sector* node);


};

#endif // SPACESECTORLLRBT_H
