#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {

public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* insert(Sector* node, Sector* parent, Sector* newSector);
    void inOrderTraversal(Sector* node);
    void preOrderTraversal(Sector* node);
    void postOrderTraversal(Sector* node);
    Sector* deleteNode(Sector* node, const std::string& sector_code);
    Sector* FindInorderSuccessor(Sector* node);
    Sector* findNode(Sector* node, const std::string& sector_code) const;
    void deleteTree(Sector* node);
};

#endif // SPACESECTORBST_H
