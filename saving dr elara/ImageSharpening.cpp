#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    kernelHeight = 3;
    kernelWidth = 3;
    blurringKernel = new double*[kernelHeight];
    for(int i = 0; i < kernelHeight; ++i){
        blurringKernel[i] = new double[kernelWidth];  
        }
    for(int i = 0; i < kernelHeight; ++i){
        for(int j = 0; j < kernelWidth;++j){
            blurringKernel[i][j] = (double)1.0/9.0;
            }
    }

}

ImageSharpening::~ImageSharpening(){
    if(blurringKernel != nullptr){
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] blurringKernel[i];
        }
        delete[] blurringKernel;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution convolution(blurringKernel, kernelHeight, kernelWidth, 1, true);
    ImageMatrix blurredImage = convolution.convolve(input_image);
    ImageMatrix sharpenedImage =   (input_image - blurredImage) * k + input_image ;
    for(int i = 0; i < sharpenedImage.get_height(); ++i ){
        for(int j = 0; j < sharpenedImage.get_width(); ++j){
            if(sharpenedImage.get_data(i, j) > 255.0){
                sharpenedImage.set_data(i, j, 255.0);
                }
            if(sharpenedImage.get_data(i, j) < 0.0){
                sharpenedImage.set_data(i, j, 0.0);
                }
            }
        }
    
    return sharpenedImage;

}


