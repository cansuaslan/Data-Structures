#include "EncodeMessage.h"
#include <bitset>
#include <cmath>



// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}


int EncodeMessage::fibonacci(int n) {
    if (n <= 1) {
        return n;
    }

    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }

    return b;
}


char EncodeMessage::transformCharacter(char c, size_t index) {
    // Assuming you have a Fibonacci function that returns the Fibonacci number at a given index
    size_t fibonacciNumber = fibonacci(index);
    bool isPrime = true;
    if (index < 2) isPrime = false;
    for (size_t i = 2; i <= sqrt(index); ++i) {
        if (index % i == 0) {
            isPrime = false;
            break;
        }
    }

    // Add the Fibonacci number corresponding to the index
    if (isPrime) {
        int asciiValue = static_cast<int>(c);
        asciiValue += fibonacciNumber;
        asciiValue = asciiValue < 33 ? asciiValue + 33 : (asciiValue > 126 ? 126 : asciiValue);
        c = static_cast<char>(asciiValue);
    }
    // Ensure that the transformed character is printable within the ASCII range of 33 to 126
    

    return c;
}

std::string EncodeMessage::rightCircularShift(const std::string& str, size_t shift) {
    shift %= str.length();
    return str.substr(str.length() - shift) + str.substr(0, str.length() - shift);
}


ImageMatrix EncodeMessage::embedMessageBits(const ImageMatrix& img, const std::string& messageBits, const std::vector<std::pair<int,int>>& positions){
    size_t index = 0;
    ImageMatrix imgChanged = img;
    for(const auto& pixel : positions){
        if (index >= messageBits.size()) {break;}
        int pixelValue = img.get_data(pixel.first, pixel.second);
        pixelValue &= 0b11111110;
        pixelValue |= (messageBits[index] - '0');
        imgChanged.set_data(pixel.first, pixel.second, pixelValue);
        ++index;
        }
    return imgChanged;
    }


// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    std::string transformedMessage;
    for(size_t i = 0; i < message.length(); ++i){
        char transformedChar = transformCharacter(message[i],i);
        transformedMessage.push_back(transformedChar);
        }
    transformedMessage = rightCircularShift(transformedMessage,message.length()/2);
    std::string messageBits;
    for(char c : transformedMessage){
        int dn = static_cast<int>(c);
        messageBits += (std::bitset<7>(dn)).to_string();
        
        }
    return embedMessageBits(img, messageBits, positions);
    
}
