#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include "ImageMatrix.h"

class EncodeMessage {
public:
    EncodeMessage();
    ~EncodeMessage();
    char transformCharacter(char c, size_t index);
    std::string rightCircularShift(const std::string& str, size_t shift);
    ImageMatrix embedMessageBits(const ImageMatrix& img, const std::string& messageBits, const std::vector<std::pair<int,int>>& positions);
    ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions);
    int fibonacci(int n);

private:
    // Any private helper functions or variables if necessary

    
};

#endif // ENCODE_MESSAGE_H
