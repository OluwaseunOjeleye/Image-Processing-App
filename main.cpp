#include <imageProcessing.h>

int main(){
	Image Img1, Img2, Img3;

    "monalisa.jpg">>Img1;
    "image.jpeg">>Img2;

	//Img1.Threshold(150);
    //Img2.Threshold(150);

    Img1.SobelFilter();
    Img2.SobelFilter();


    //before the AND(*), OR(+), NOT(!) operators are used, the images must be thresholded first
    //Img3=Img1*Img2;
    //Img3=Img1+Img2;
    //Img3=Img1;
    //Img3=!Img3;

    "output1"<<Img1;
    "output2"<<Img2;
}