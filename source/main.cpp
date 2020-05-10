#include "mainwindow.h"
#include <QApplication>
#include <image.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*
    BMP b("marbles.bmp");
    BMP z;
    z=b;
    std::cout<<z.data.size()<<std::endl;
    z.write("ff.bmp");
    */
    //w.show();

    //bitmap_image b("marbles.bmp");
    //b.save_image("jsjsj.bmp");

    /*
    Image img;
    img.LoadImage("marbles.bmp");
    img.PrintImage("hdhdh.bmp");
    */
    return a.exec();
}
