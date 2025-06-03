#include "Sector.h"
#include <cmath>
#include <string>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    // TODO: Calculate the distance to the Earth, and generate the sector code
    distance_from_earth = sqrt(x*x + y*y + z*z);
    sector_code = std::to_string(int(distance_from_earth));
    if(x > 0){sector_code += "R";
    } else if(x == 0){sector_code += "S";
    }else{sector_code += "L";}
    if(y > 0){sector_code += "U";
    } else if(y == 0){sector_code += "S";
    }else{sector_code += "D";}
    if(z > 0){sector_code += "F";
    } else if(z == 0){sector_code += "S";
    }else{sector_code += "B";
    }
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}