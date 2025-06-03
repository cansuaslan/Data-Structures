// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string decodedMessage;
    for(const auto& pixel : edgePixels){
        int pixelValue = image.get_data(pixel.first, pixel.second);
        int lsb = pixelValue & 1;        
        decodedMessage.push_back('0' + lsb);
        }
    while (decodedMessage.length() % 7 != 0){
        decodedMessage = "0" + decodedMessage;
        }
    std::string asciiString;
    for(size_t i = 0; i < decodedMessage.length(); i += 7 ){
        std::string segment = decodedMessage.substr(i,7);
        int decimalValue = std::stoi(segment, nullptr, 2);
        if(decimalValue <= 32){
            decimalValue += 33;
            }
        else if(decimalValue == 127){
            decimalValue = 126;
            }
        asciiString.push_back(static_cast<char>(decimalValue));
        }
    decodedMessage = asciiString;
    return decodedMessage;

}

