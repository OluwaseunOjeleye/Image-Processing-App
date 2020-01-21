#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>


#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

struct pixel{
    int Blue;
    int Green;
    int Red;
};
typedef pixel Pixel;

/************************************Image******************************************************/
class Image{
	friend void operator>>(std::string filename, Image &ImgData);
	friend void operator<<(std::string filename, Image &ImgData);
    public:
        Image();
        ~Image();

        //Filters
		void gaussFilter();
		void smoothingFilter();
		void sharpen();

		//Edge Detection
		void SobelFilter();
		void edgeDetect();
		void robertFilter();
		void prewittFilter();
        
        void Threshold(int value);

        Image operator=(const Image &img1);
        Image operator*(const Image &img2)const; //AND
        Image operator+(const Image &img2); //OR
        Image operator!();  //NOT


    private:
        std::vector<std::vector<Pixel>> matrix;
        int row;
        int column;
    	int KernelX[3][3];
    	int KernelY[3][3];

};