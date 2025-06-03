// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    gHorizontal = new double*[3]{
        new double[3]{-1, 0, 1},
        new double[3]{-2, 0, 2},
        new double[3]{-1, 0, 1}
    };
    gVertical = new double*[3]{
        new double[3]{-1, -2, -1},
        new double[3]{0, 0, 0},
        new double[3]{1, 2, 1}
    };
}

// Destructor
EdgeDetector::~EdgeDetector() {
    for(int i=0; i < 3; ++i){
        delete[] gHorizontal[i];
        delete[] gVertical[i];
    }
    delete[] gHorizontal;
    delete[] gVertical;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    std::vector<std::pair<int, int>> detectedEdges; //hjflajksdfhdsşajklgn
    Convolution convolutionGx(gHorizontal, 3, 3, 1, true); 
    Convolution convolutionGy(gVertical, 3, 3, 1, true); 
    ImageMatrix Ix = convolutionGx.convolve(input_image);
    ImageMatrix Iy = convolutionGy.convolve(input_image);
    int inputHeight = input_image.get_height();
    int inputWidth = input_image.get_width();
    double threshold = 0.0;
    for (int i = 0; i < inputHeight; ++i) {
        for (int j = 0; j < inputWidth; ++j) {
            double magnitude = std::sqrt(std::pow(Ix.get_data(i, j), 2) + std::pow(Iy.get_data(i, j), 2));
            threshold += magnitude;
        }
    }
    threshold /= (inputHeight * inputWidth);
    for (int i = 0; i < inputHeight; ++i) {
        for (int j = 0; j < inputWidth; ++j) {
            double magnitude = std::sqrt(std::pow(Ix.get_data(i, j), 2) + std::pow(Iy.get_data(i, j), 2));
            if (magnitude > threshold) {
                detectedEdges.emplace_back(i, j);
            }
        }
    }
    return detectedEdges;

}

