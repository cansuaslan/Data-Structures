#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening Image;
    EdgeDetector detector;
    DecodeMessage decoder;
    ImageMatrix sharpenedImage =Image.sharpen(img,2);
    std::vector<std::pair<int, int>> edges = detector.detectEdges(sharpenedImage);
    std::string password = decoder.decodeFromImage(sharpenedImage, edges);
    return password;
    
    
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening Image_2;
    EdgeDetector detector_2;
    EncodeMessage encoder;
    ImageMatrix sharpenedImage = Image_2.sharpen(img,2);
    std::vector<std::pair<int, int>> edges = detector_2.detectEdges(sharpenedImage);
    ImageMatrix encodedImage = encoder.encodeMessageToImage(img, message ,edges);
    return encodedImage; 

}
