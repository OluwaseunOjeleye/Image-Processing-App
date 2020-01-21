#include "imageProcessing.h"

Image::Image(){
    this->row=0;
    this->column=0;
	 //Sobel Horizontal Mask
    KernelX[0][0] = 1; KernelX[0][1] = 0; KernelX[0][2] = -1;
    KernelX[1][0] = 2; KernelX[1][1] = 0; KernelX[1][2] = -2;
    KernelX[2][0] = 1; KernelX[2][1] = 0; KernelX[2][2] = -1;

    //Sobel Vertical Mask
    KernelY[0][0] =  1; KernelY[0][1] = 2; KernelY[0][2] =   1;
    KernelY[1][0] =  0; KernelY[1][1] = 0; KernelY[1][2] =   0;
    KernelY[2][0] = -1; KernelY[2][1] =-2; KernelY[2][2] =  -1;
}

Image::~Image(){

}

//assignment operator =
Image Image::operator=(const Image &img1){
    if(this!=&img1){
        this->row=img1.row;
        this->column=img1.column;

        this->matrix.resize(img1.row, std::vector<Pixel> (img1.column));

        for(int i=0; i<img1.row; i++){
            for(int j=0; j<img1.column; j++){
                this->matrix[i][j].Blue=img1.matrix[i][j].Blue;
                this->matrix[i][j].Green=img1.matrix[i][j].Green;
                this->matrix[i][j].Red=img1.matrix[i][j].Red;
            }
        }
    }
    return *this;
}

//AND- outputs the intersection between two images
Image Image::operator*(const Image &img2)const{

    Image Output;

    Output.row=this->row;
    Output.column=this->column;
    Output.matrix.resize(this->row, std::vector<Pixel> (this->column));

    for (int i=0; i<this->row; i++){
        for (int j=0; j<this->column; j++){
            Output.matrix[i][j].Blue=sqrt(this->matrix[i][j].Blue*img2.matrix[i][j].Blue);
            Output.matrix[i][j].Green=sqrt(this->matrix[i][j].Green*img2.matrix[i][j].Green);
            Output.matrix[i][j].Red=sqrt(this->matrix[i][j].Red*img2.matrix[i][j].Red);
        }
    }

    return Output;
}

//OR- Merge the two images ontop of one another
Image Image::operator+(const Image &img2){
    Image Output;

    Output.row=this->row;
    Output.column=this->column;
    Output.matrix.resize(this->row, std::vector<Pixel> (this->column));

    for (int i=0; i<this->row; i++){
        for (int j=0; j<this->column; j++){
            Output.matrix[i][j].Blue=int((this->matrix[i][j].Blue+img2.matrix[i][j].Blue)/2);
            Output.matrix[i][j].Green=int((this->matrix[i][j].Green+img2.matrix[i][j].Green)/2);
            Output.matrix[i][j].Red=int((this->matrix[i][j].Red+img2.matrix[i][j].Red)/2);
        }
    }

    return Output;
}

//NOT(!) Turns thresholded white to black and black to white
Image Image::operator!(){
    Image Output;

    Output.row=this->row;
    Output.column=this->column;

    Output.matrix.resize(this->row, std::vector<Pixel> (this->column));

    for (int i=0; i<this->row; i++){
        for (int j=0; j<this->column; j++){
            Output.matrix[i][j].Blue=255-this->matrix[i][j].Blue;
            Output.matrix[i][j].Green=255-this->matrix[i][j].Green;
            Output.matrix[i][j].Red=255-this->matrix[i][j].Red;
        }
    }
    return Output;
}

//Sobel FIlter
void Image::SobelFilter(){
    int convolute_blue, convolute_green, convolute_red;

    std::vector<std::vector<Pixel>> output;
    output.resize(this->row, std::vector<Pixel> (this->column));

    //Gradient X=Image Matrix*KernelX and Gradient Y=Image Matrix*KernelY
    for (int i=1; i<this->row-1; i++){
        for (int j=1; j<this->column-1; j++){

            int ConvoluteY_blue=0, ConvoluteX_blue=0;
            int ConvoluteY_green=0, ConvoluteX_green=0;
            int ConvoluteY_red=0, ConvoluteX_red=0;

            for (int k=-1; k<=1; k++){
                for (int l=-1; l<=1; l++){
                    ConvoluteX_blue+= this->KernelX[k+1][l+1] * this->matrix[i+k][j+l].Blue;  //Gradient X
                    ConvoluteY_blue+= this->KernelY[k+1][l+1] * this->matrix[i+k][j+l].Blue;  //Gradient Y

                    ConvoluteX_green+= this->KernelX[k+1][l+1] * this->matrix[i+k][j+l].Green;  //Gradient X
                    ConvoluteY_green+= this->KernelY[k+1][l+1] * this->matrix[i+k][j+l].Green;  //Gradient Y

                    ConvoluteX_red+= this->KernelX[k+1][l+1] * this->matrix[i+k][j+l].Red;  //Gradient X
                    ConvoluteY_red+= this->KernelY[k+1][l+1] * this->matrix[i+k][j+l].Red;  //Gradient Y
                }
            }

            //The magnitude Of Gradient X and Gradient Y
            convolute_blue=sqrt(pow(ConvoluteX_blue,2)+pow(ConvoluteY_blue,2));
            //Normalizing values higher than 255 to 255 and values lesser than 0 to 0
            convolute_blue = convolute_blue > 255 ? 255:convolute_blue;
            output[i][j].Blue=convolute_blue;

            convolute_green=sqrt(pow(ConvoluteX_green,2)+pow(ConvoluteY_green,2));
            convolute_green = convolute_green > 255 ? 255:convolute_green;
            output[i][j].Green=convolute_green;

            convolute_red=sqrt(pow(ConvoluteX_red,2)+pow(ConvoluteY_red,2));
            convolute_red = convolute_red > 255 ? 255:convolute_red;
            output[i][j].Red=convolute_red;
        }
    }
    //Storing the sobel output in the image matrix
    for (int s=0; s<this->row; s++){
        for (int t=0; t<this->column; t++){
            this->matrix[s][t].Blue=output[s][t].Blue;
            this->matrix[s][t].Green=output[s][t].Green;
            this->matrix[s][t].Red=output[s][t].Red;
        }
    }

    //Thresholding the sobel output
    //Threshold(150);
}

//Thresholding Method
void Image::Threshold(int value){
	for (int i=1; i<this->row-1; i++){
		for (int j=1; j<this->column-1; j++){
            this->matrix[i][j].Blue= this->matrix[i][j].Blue> value? 255:0;
            this->matrix[i][j].Green= this->matrix[i][j].Green> value? 255:0;
            this->matrix[i][j].Red= this->matrix[i][j].Red> value? 255:0;

		}
	}
}


//Reading Image
void operator>>(std::string filename, Image &ImgData){
    cv::Mat image;
    image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);

    if(! image.data ) {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return;
    }


    ImgData.row=image.rows;
    ImgData.column=image.cols;

    ImgData.matrix.resize(ImgData.row, std::vector<Pixel>(ImgData.column));

    std::vector<cv::Mat> BGR;
    cv::split(image, BGR);

    for(int i=0; i<ImgData.row; i++){
        for(int j=0; j<ImgData.column; j++){
            ImgData.matrix[i][j].Blue=(int)BGR[0].at<uchar>(i,j);
            ImgData.matrix[i][j].Green=(int)BGR[1].at<uchar>(i,j);
            ImgData.matrix[i][j].Red=(int)BGR[2].at<uchar>(i,j);

            //std::cout<<"i="<<i<<" j-"<<j<<" Blue is: "<<this->Matrix[i][j].Blue<<"\t";

        }
    }
}

//Writing Image
void operator<<(std::string filename, Image &ImgData){
    cv::Mat Printimage=cv::Mat(ImgData.row, ImgData.column, CV_8UC3);

    for(int i=0; i<ImgData.row; i++){
        for(int j=0; j<ImgData.column; j++){
            cv::Vec3b color;
            color[0]=(unsigned char)ImgData.matrix[i][j].Blue;
            color[1]=(unsigned char)ImgData.matrix[i][j].Green;
            color[2]=(unsigned char)ImgData.matrix[i][j].Red;
            Printimage.at<cv::Vec3b>(i,j)=color;
        }
    }

    cv::imwrite(filename+".jpg", Printimage);
    std::cout<<filename<<" Printed"<<std::endl;
}


