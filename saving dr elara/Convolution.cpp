#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : kernel(nullptr), kernelHeight(0), kernelWidth(0), stride(1), padding(false) {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) : kernel(customKernel), kernelHeight(kh), kernelWidth(kw), stride(stride_val), padding(pad) {
    kernel = new double*[kw];
    for(int i = 0; i < kernelHeight; ++i){
        kernel[i] = new double[kernelWidth];
    }
    for(int i = 0; i < kernelHeight; ++i ){
        for(int j = 0; j < kernelWidth; ++j){
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if(kernel != nullptr){
    for (int i = 0; i < kernelHeight; ++i) {
        delete[] kernel[i];
    }
    delete[] kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other) : kernel(other.kernel), kernelHeight(other.kernelHeight), kernelWidth(other.kernelWidth),stride(other.stride), padding(other.padding){
    kernel = new double*[kernelHeight];
    for(int i = 0; i < kernelHeight; ++i){
        kernel[i] = new double[kernelWidth];
    }
    for(int i = 0; i < kernelHeight; ++i ){
        for(int j = 0; j < kernelWidth; ++j){
            kernel[i][j] = other.kernel[i][j];
        }
    }
    //kernelHeight = other.kernelHeight;
    //kernelWidth = other.kernelWidth;
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if(this != &other){
        if(kernel != nullptr){
            for(int i = 0; i < kernelHeight; ++i){
                delete[] kernel[i];
            }
            delete[] kernel;
        }
        if(other.kernel != nullptr){
            kernelHeight = other.kernelHeight;
            kernelWidth = other.kernelWidth;
            stride = other.stride;
            padding = other.padding;
            kernel = new double*[kernelHeight];
            for(int i = 0; i < kernelHeight; ++i){
                kernel[i] = new double[kernelWidth];
            }
            for(int i = 0; i < kernelHeight; ++i){
                for(int j = 0; j < kernelWidth; ++j){
                    kernel[i][j] = other.kernel[i][j];
                }
            }
        }
        
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int inputHeight = input_image.get_height();
    int inputWidth = input_image.get_width();
    int outputHeight = (inputHeight - kernelHeight + 2 * (padding ? 1 : 0)) / stride + 1;
    int outputWidth = (inputWidth - kernelWidth + 2 * (padding ? 1 : 0)) / stride + 1;
    ImageMatrix padded(inputHeight = inputHeight + 2, inputWidth = inputWidth + 2);
    if(padding){
        for(int i = 1; i < inputHeight + 2; ++i){
            for(int j = 1; j < inputWidth + 2; ++j){
                padded.set_data(i,j, input_image.get_data(i-1,j-1));
                }
            }
        }
    ImageMatrix result(outputHeight,outputWidth);
    for(int i = 0; i < outputHeight; ++i){
        for(int j = 0; j < outputWidth; ++j){
            double sum = 0.0;
            for(int m = 0; m < kernelHeight; ++m){
                for(int k = 0; k < kernelWidth; ++k){
                    int inputRow = i * stride + m;
                    int inputCol = j * stride + k;
                    if(padding){
                        if(inputRow >= 0 && inputRow < inputHeight && inputCol >= 0 && inputCol < inputWidth){
                            sum += kernel[m][k] * padded.get_data(inputRow, inputCol);
                        }
                    }else{
                        if(inputRow >= 0 && inputRow < inputHeight && inputCol >= 0 && inputCol < inputWidth){
                        sum += kernel[m][k] * input_image.get_data(inputRow, inputCol);
                        }
                    }
                    
                }
            }
            result.set_data(i, j, sum);            
            }
        }
    return result;
}
