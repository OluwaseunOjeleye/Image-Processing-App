#include "image.h"

Image::Image(){
    loaded=false;
}

Image::Image(std::string filename){
    loaded=false;
    LoadImage(filename);
}

Image::~Image(){
    //delete image;
    //delete image_intensity;
}

void Image::LoadImage(std::string filename){
    if(loaded){
        std::cout<<"Image has been loaded"<<std::endl;
        return;
    }
    image.addfilename("marbles.bmp");
    width=this->image.width();
    height=this->image.height();

    this->image_intensity=this->image;
    loaded=true;
}

void Image::PrintImage(std::string filename){
    if(!loaded){
        std::cout<<"Load Image"<<std::endl;
        return;
    }

    //std::cout<<image_intensity.data.size()<<std::endl;
   /* for(int i=50; i<150; i++){
        std::cout<<static_cast<int>(image_intensity.data[i])<<std::endl;
    } */
    this->image_intensity.save_image(filename);
}

bool Image::isLoaded() const{
    return this->loaded;
}

int Image::getWidth() const{
    return this->width;
}

int Image::getHeight() const{
    return this->height;
}

void Image::ClearImage(){
    this->width=0;
    this->height=0;
    this->loaded=false;
}
