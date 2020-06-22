#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    black={0, 0, 0};
    white={255, 255, 255};
    Color[0]={255, 0, 0}; Color[1]={0, 255, 0}; Color[2]={0,0,255}, Color[3]={255, 255, 0}, Color[4]={255, 0, 255};
    Color[5]={128, 0, 0}; Color[6]={0, 128, 0}; Color[7]={0,0,128}, Color[8]={128, 128, 0}, Color[9]={128, 0, 128};
    Color[10]={64, 0, 0}; Color[11]={0, 64, 0}; Color[12]={0,0,64}, Color[13]={64, 64, 0}, Color[14]={64, 0, 64};
    Color[15]={32, 0, 0}; Color[16]={0, 32, 0}; Color[17]={0,0,32}, Color[18]={128, 32, 0}, Color[19]={128, 0, 32};

    Color[20]={255, 64, 0}; Color[21]={64, 255, 0}; Color[22]={45, 0, 55}, Color[23]={200, 0, 16}, Color[24]={55, 40, 255};
    Color[25]={128, 32, 0}; Color[26]={32, 128, 0}; Color[27]={32, 12, 28}, Color[28]={64, 34, 7}, Color[29]={128, 80, 28};
    Color[30]={64, 32, 0}; Color[31]={32, 64, 255}; Color[32]={18, 128, 53}, Color[33]={4, 46, 0}, Color[34]={64, 67, 64};
    Color[35]={32, 64, 0}; Color[36]={60, 32, 100}; Color[37]={200, 200, 90}, Color[38]={118, 32, 67}, Color[39]={119, 121, 32};

    //Mean Filter for Noise Elimination
    Mean_Kernel.resize(3, std::vector<double> (3));
    Mean_Kernel[0][0] =  0.111111111; Mean_Kernel[0][1] = 0.111111111; Mean_Kernel[0][2] =   0.111111111;
    Mean_Kernel[1][0] =  0.111111111; Mean_Kernel[1][1] = 0.111111111; Mean_Kernel[1][2] =   0.111111111;
    Mean_Kernel[2][0] = 0.111111111; Mean_Kernel[2][1] =0.111111111; Mean_Kernel[2][2] =  0.111111111;

    //Structuring Element
    Structuring_Element.resize(3, std::vector<unsigned char>(3));
    Structuring_Element[0][0] = 0; Structuring_Element[0][1] = 255; Structuring_Element[0][2] = 0;
    Structuring_Element[1][0] = 255; Structuring_Element[1][1] = 255; Structuring_Element[1][2] = 255;
    Structuring_Element[2][0] = 0; Structuring_Element[2][1] = 255; Structuring_Element[2][2] = 0;

    //Gaussian Kernel 3x3
    //Gaussian_Kernel.resize(3, std::vector<double> (3));
    //Gaussian_Kernel[0][0] =  0.0625; Gaussian_Kernel[0][1] = 0.125; Gaussian_Kernel[0][2] =   0.0625;
    //Gaussian_Kernel[1][0] =  0.125; Gaussian_Kernel[1][1] = 0.25; Gaussian_Kernel[1][2] =   0.125;
    //Gaussian_Kernel[2][0] = 0.0625; Gaussian_Kernel[2][1] =0.125; Gaussian_Kernel[2][2] =  0.0625;

    //Gaussian Kernel 5x5
    Gaussian_Kernel.resize(5, std::vector<double> (5));
    Gaussian_Kernel[0][0] =  (double)1/273; Gaussian_Kernel[0][1] = (double)4/273; Gaussian_Kernel[0][2] =   (double)7/273; Gaussian_Kernel[0][3] = (double)4/273; Gaussian_Kernel[0][4] =  (double)1/273;
    Gaussian_Kernel[1][0] =  (double)4/273; Gaussian_Kernel[1][1] = (double)16/273; Gaussian_Kernel[1][2] =  (double)26/273; Gaussian_Kernel[1][3] = (double)16/273; Gaussian_Kernel[1][4] =   (double)4/273;
    Gaussian_Kernel[2][0] =  (double)7/273; Gaussian_Kernel[2][1] = (double)26/273; Gaussian_Kernel[2][2] =  (double)41/273; Gaussian_Kernel[2][3] = (double)26/273; Gaussian_Kernel[2][4] =   (double)7/273;
    Gaussian_Kernel[3][0] =  (double)4/273; Gaussian_Kernel[3][1] = (double)16/273; Gaussian_Kernel[3][2] =  (double)26/273; Gaussian_Kernel[3][3] = (double)16/273; Gaussian_Kernel[3][4] =   (double)4/273;
    Gaussian_Kernel[4][0] =  (double)1/273; Gaussian_Kernel[4][1] =(double)4/273; Gaussian_Kernel[4][2] =  (double)7/273; Gaussian_Kernel[4][3] = (double)4/273; Gaussian_Kernel[4][4] =   (double)1/273;

    //Sobel Horizontal Kernel
    Gx_Kernel.resize(3, std::vector<double> (3));
    Gx_Kernel[0][0] =  -1; Gx_Kernel[0][1] = 0; Gx_Kernel[0][2] =  1;
    Gx_Kernel[1][0] =  -2; Gx_Kernel[1][1] = 0; Gx_Kernel[1][2] =  2;
    Gx_Kernel[2][0] =  -1; Gx_Kernel[2][1] = 0; Gx_Kernel[2][2] =  1;

    //Sobel Vertical Kernel
    Gy_Kernel.resize(3, std::vector<double> (3));
    Gy_Kernel[0][0] =   1; Gy_Kernel[0][1] =  2; Gy_Kernel[0][2] =  1;
    Gy_Kernel[1][0] =   0; Gy_Kernel[1][1] =  0; Gy_Kernel[1][2] =  0;
    Gy_Kernel[2][0] =  -1; Gy_Kernel[2][1] = -2; Gy_Kernel[2][2] = -1;

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Loading Image, filtering and creating intensity image
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, QObject::tr("Open Text File[Image]"));
    if(fileName.isEmpty()){
        this->MsgBox.setText("Select an image");
        this->MsgBox.exec();
        return;
    }
    imagepath=fileName;
    image.addfilename(fileName.toStdString());
    this->width=image.width();
    this->height=image.height();

    //Noise Filtering using Mean Filter
    this->image=convolute(Mean_Kernel, true);
    this->image_intensity=this->image;

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            rgb_t color=image_intensity.get_pixel(i, j);
            unsigned char intensity=(0.3*color.red)+(0.59*color.green)+(0.11*color.blue);
            color.red=color.green=color.blue=intensity;
            image_intensity.set_pixel(i, j, color);
        }
    }

    image_intensity.save_image("intensity.bmp");

    QPixmap m(fileName);
    ui->label1->setPixmap(m.scaled(300, 300,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    QPixmap n("./Processed_images/intensity.bmp");
    ui->label2->setPixmap(n.scaled(300, 300,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));


    this->MsgBox.setText("Image "+fileName+" has been loaded.");
    this->MsgBox.exec();
}


//Saving Image
void MainWindow::on_actionSave_triggered()
{
    QString filename = QInputDialog::getText(0,"Input image name","Note..");
    this->image_intensity.save_image(filename.toStdString());
    this->MsgBox.setText("Image has been saved as "+filename);
    this->MsgBox.exec();
}


//Clearing UI
void MainWindow::on_actionClear_triggered()
{
    //this->image.ClearImage();
    ui->label1->clear();
    ui->label2->clear();
}


//Closing App
void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton question;
    question=QMessageBox::question(this, "Quit", "Are you sure?",QMessageBox::Yes|QMessageBox::No);
    if (question == QMessageBox::Yes){
        QApplication::quit();
    }
    else{
    }
}

//Displaying Gray-Level Image
void MainWindow::on_actionHistogram_triggered()
{
    Calculate_Intensity_Hist();
    QChartView *view = new QChartView;
    QChart *chart = new QChart;
    view->setChart(chart);

    QLineSeries *series = new QLineSeries;
    series->setColor(QColor("Black"));
    series->setName("Black");
    for(int j=0; j <256; ++j){
        *series << QPoint(j, this->histogram[j]);
    }
    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->axisX()->setGridLineVisible(false);
    chart->axisY()->setGridLineVisible(false);

    QLabel *label = new QLabel;
    QPixmap n("intensity.bmp");
    label->setPixmap(n.scaled(300, 300,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    QWidget *window=new QWidget;
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(label);
    lay->addWidget(view);
    window->setLayout(lay);
    window->show();
}

//Displaying Colored-Image Histogram
void MainWindow::on_actionColor_histogram_triggered()
{
    Calculate_Colored_Hist();
    QChartView *view = new QChartView;
    QChart *chart = new QChart;
    view->setChart(chart);

    QString color[3]={"Blue", "Red", "Green"};
    for(int i=0; i<3; i++){
        QLineSeries *series = new QLineSeries;
        series->setColor(QColor(color[i]));
        series->setName(color[i]);
        for(int j=0; j <256; ++j){
            if(i==0)*series << QPoint(j, this->blue_histogram[j]);
            else if(i==1)*series << QPoint(j, this->red_histogram[j]);
            else *series << QPoint(j, this->green_histogram[j]);
        }
        chart->addSeries(series);
    }

    chart->createDefaultAxes();
    chart->axisX()->setGridLineVisible(false);
    chart->axisY()->setGridLineVisible(false);

    QLabel *label = new QLabel;
    QPixmap n(imagepath);
    label->setPixmap(n.scaled(300, 300,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    QWidget *window=new QWidget;
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(label);
    lay->addWidget(view);
    window->setLayout(lay);
    window->show();
}

//Image Displaying Function
void::MainWindow::display_Image(std::string filename, std::string title){
    QLabel *label = new QLabel;
    QPixmap n(QString::fromUtf8(("./Processed_images/"+filename).c_str()));
    label->setPixmap(n.scaled(500, 500,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    QWidget *window=new QWidget;
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(label);
    window->setWindowTitle(QString::fromUtf8(title.c_str()));
    window->setLayout(lay);
    window->show();
}

//Otsu Thresholding Method
void MainWindow::on_actionOtsu_Threshold_triggered()
{
    Otsu_Threshold(true);
}

// Display filter of input Image
void MainWindow::on_actionFilter_triggered()
{
    bitmap_image conv=convolute(Mean_Kernel, true);
    this->image=conv;
    conv.save_image("convolute.bmp");
    display_Image("convolute.bmp", "Convolute");
}


//Displaying each objects after CCA
void MainWindow::on_actionConnected_Component_Analysis_triggered()
{
    bitmap_image CCA=Connected_Component_Analysis();
    CCA.save_image("CCA.bmp");
    display_Image("CCA.bmp", "Connected Component Analysis");
}

//Dilation operation trigger
void MainWindow::on_actionDilation_triggered()
{
    //for (int i=0; i<12; i++)
    Dilate();
    image_binary.save_image("dilation.bmp");
    display_Image("dilation.bmp", "Morphology: Dilation");
}

//Erosion operation trigger
void MainWindow::on_actionErosion_triggered()
{
    //for (int i=0; i<12; i++)
    Erosion();
    image_binary.save_image("erosion.bmp");
    display_Image("erosion.bmp", "Morphology: Erosion");
}

//Opening operation trigger
void MainWindow::on_actionOpen_2_triggered()
{
    Opening();
    image_binary.save_image("opening.bmp");
    display_Image("opening.bmp", "Morphology: Opening");
}

//Closing operation trigger
void MainWindow::on_actionClosing_triggered()
{
    Closing();
    image_binary.save_image("closing.bmp");
    display_Image("closing.bmp", "Morphology: Closing");
}

//Feature Extraction trigger
void MainWindow::on_actionFeature_Extraction_triggered()
{
    Feature_Extraction(true);
    this->MsgBox.setText("Features Extracted");
    this->MsgBox.exec();
}

//Training trigger
void MainWindow::on_actionTrain_triggered()
{
    Training();
    this->MsgBox.setText("Data Trained...\n Now you can predict");
    this->MsgBox.exec();
}

//Testing trigger
void MainWindow::on_actionTest_triggered()
{
    Test();
}

//Smoothening- Gaussian Filter Trigger
void MainWindow::on_actionGaussian_Filter_triggered()
{
    Smoothening_Gaussian_Filter();
    smoothed_image.save_image("smoothening.bmp");
    display_Image("smoothening.bmp", "Smoothening: Gaussian Filter");
}

void MainWindow::on_actionGx_triggered()
{
    bitmap_image gradient_x=Calc_Horizontal_Gradient();
    gradient_x.save_image("Gx.bmp");
    display_Image("Gx.bmp", "Sobel Filter: Horizontal Gradient");
}

void MainWindow::on_actionGy_triggered()
{
    bitmap_image gradient_y=Calc_Vertical_Gradient();
    gradient_y.save_image("Gy.bmp");
    display_Image("Gy.bmp", "Sobel Filter: Vertical Gradient");
}

void MainWindow::on_actionG_triggered()
{
    Calc_Gradient_n_Direction();
    gradient.save_image("Gradient.bmp");
    display_Image("Gradient.bmp", "Sobel Filter: Gradient");
}

void MainWindow::on_actionOrientation_triggered()
{
    Calc_Gradient_n_Direction();
    direction.save_image("Direction.bmp");
    display_Image("Direction.bmp", "Sobel Filter: Direction");
}

void MainWindow::on_actionSobel_Edge_Detector_triggered()
{
    Calc_Gradient_n_Direction();
    image_edge_dect=gradient;

    //Using Otsu Thresholding for thresholding sobel filter result
    int hist[256];
    for(int i=0; i<256; i++) hist[i]=0;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            hist[image_edge_dect.get_pixel(i, j).blue]++;
        }
    }

    int T=0; double max=0;
    for(int i=0; i<256; i++){
        double Weight_b=0, Mean_b=0, Weight_f=0, Mean_f=0;
        for(int j=0; j<i; j++){
            Weight_b+=hist[j];
            Mean_b+=(hist[j]*j);
        }
        for(int j=i; j<256; j++){
            Weight_f+=hist[j];
            Mean_f+=(hist[j]*j);
        }
        Mean_b/=Weight_b;
        Weight_b/=(this->width*this->height);
        Mean_f/=Weight_f;
        Weight_f/=(this->width*this->height);

        double variance=(Weight_b*Weight_f)*pow((Mean_b-Mean_f),2);
        if(variance>max){
            max=variance;
            T=i;
        }
    }

    //thresholding
    unsigned char background_intensity;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            rgb_t color=image_edge_dect.get_pixel(i, j);
            if(i==0 && j==0){
                background_intensity=color.red<T?0: 255;
            }
            unsigned char intensity;
            if(background_intensity==0){
                intensity=color.red<T?255: 0;
            }
            else{
                intensity=color.red<T?0: 255;
            }
            color.red=color.green=color.blue=intensity;
            image_edge_dect.set_pixel(i, j, color);
        }
    }

    //inverting binary image color
    if(this->image_edge_dect.get_pixel(0, 0).red==0){
        //background is black
    }
    else{
        for(int i=0; i<width; i++){
            for(int j=0; j<height; j++){
                if(image_edge_dect.get_pixel(i, j).red==255){
                    image_edge_dect.set_pixel(i,j, black);
                }
                else{
                     image_edge_dect.set_pixel(i,j, white);
                }
            }
        }
    }

    image_edge_dect.save_image("sobel_detector.bmp");
    display_Image("sobel_detector.bmp", "Sobel Edge Detector: Sobel Edge Image");
}

void MainWindow::on_actionCanny_Edge_Detector_triggered()
{
    Canny_EdgeDectector();
    image_edge_dect.save_image("canny.bmp");
    display_Image("canny.bmp", "Canny Edge Detector: Canny Edge Image");
}

void MainWindow::on_actionHough_Transform_triggered()
{
    Line_detect=image_intensity;

    std::pair<int, int> Line=Hough_Transform_Line_Detector();
    /*
    int d=Line.first, theta=Line.second;
    Draw_Line(d, theta);
    */
    Line_detect.save_image("line_detection.bmp");
    display_Image("line_detection.bmp", "Hough transform: Line Detection");

}

void MainWindow::on_actionHough_Transform_Circle_triggered()
{
    Circle_detect=image_intensity;

    std::tuple<int, int, int> Circle=Hough_Transform_Circle_Detector();
    /*
    int a=std::get<0>(Circle), b=std::get<1>(Circle), r=std::get<2>(Circle);
    Draw_Circle(a, b, r);
    */
    Circle_detect.save_image("Circle_detection.bmp");
    display_Image("Circle_detection.bmp", "Hough transform: Circle Detection");

}

