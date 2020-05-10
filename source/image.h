#ifndef IMAGE_H
#define IMAGE_H

#include <bmp.h>
#include <iostream>
#include <string>
#include <math.h>

struct pixel{
    unsigned Red;
    unsigned Blue;
    unsigned Green;
};
typedef pixel Pixel;

class Image{
    public:
        Image();
        Image(std::string filename);
        ~Image();
        void LoadImage(std::string filename);
        void PrintImage(std::string filename);

        bool isLoaded() const;
        int getWidth() const;
        int getHeight() const;
        void ClearImage();

        //Edge Detection
        void SobelFilter();

    private:
        bitmap_image image;
        bitmap_image image_intensity;

        bool loaded;
        int width, height;
};

#endif // IMAGE_H
