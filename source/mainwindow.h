#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <bmp.h>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QImage>
#include <QHBoxLayout>
#include <QtCharts>
#include <random>
#include <QDebug>
#include <fstream>
#include <utility>
#include <float.h>

struct Point{
    int x, y;
};

struct Neighbor{
  std::vector<Point> neighbor;
};

struct Object{
    int label;
    int color_index;
    int min_width=INT_MAX;
    int max_width=INT_MIN;
    int min_height=INT_MAX;
    int max_height=INT_MIN;
    int width;
    int height;
    std::vector<double> Feature_vector;
    std::string name;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionClear_triggered();

    void on_actionExit_triggered();

    void on_actionHistogram_triggered();

    void on_actionColor_histogram_triggered();

    void on_actionK_Means_triggered();

    void on_actionOtsu_Threshold_triggered();

    void on_actionK_Means_Binary_triggered();

    void on_actionFilter_triggered();

    void on_actionConnected_Component_Analysis_triggered();

    void on_actionDilation_triggered();

    void on_actionErosion_triggered();

    void on_actionOpen_2_triggered();

    void on_actionClosing_triggered();

    void on_actionBoundary_Extraction_triggered();

    void on_actionDraw_Bound_Box_triggered();

    void on_actionDraw_Object_Edge_triggered();

    void on_actionFeature_Extraction_triggered();

    void on_actionTrain_triggered();

    void on_actionTest_triggered();

    void on_actionGaussian_Filter_triggered();

    void on_actionGx_triggered();

    void on_actionGy_triggered();

    void on_actionG_triggered();

    void on_actionOrientation_triggered();

    void on_actionCanny_Edge_Detector_triggered();

    void on_actionHough_Transform_triggered();

    void on_actionHough_Transform_Circle_triggered();

    void on_actionSobel_Edge_Detector_triggered();

private:
    void display_Image(std::string filename, std::string title);
    void Invert_Binary_Image_Background();
    void Threshold(int T, bool display);
    void Otsu_Threshold(bool display);
    void Calculate_Intensity_Hist();
    void Calculate_Colored_Hist();
    int generate_random(int max);
    bool is_centroids_same(rgb_t *C1, rgb_t *C2, int N);

    //Convolution Function for filter
    bitmap_image convolute(std::vector<std::vector<double>> Kernel, bool normal);

    //Connected Component Analysis
    int get_minimum_Label(std::vector<int> array);
    bitmap_image Connected_Component_Analysis();

    //Morphology Functions
    void Dilate();
    void Erosion();
    void Opening();
    void Closing();

    //RST-Invariant Feature Extraction
    void Feature_Extraction(bool save_RSI_Feature_vector);
    //η-Function
    double eta(double p, double q, int object_label);
    //μ-Function
    double mu(double p, double q, int object_label);
    //γ-Function
    double gamma(double p, double q);
    //moment-Function
    double moment(double p, double q, int object_label);


    //Training
    void Training();

    //Testing
    void Test();
    double Euclidean(std::vector<double> v1, std::vector<double>v2);

    //Smoothening
    void Smoothening_Gaussian_Filter();

    //Sobel Filter
    bitmap_image Calc_Horizontal_Gradient();
    bitmap_image Calc_Vertical_Gradient();
    void Calc_Gradient_n_Direction();

    //Canny Edge Detector
    void Canny_EdgeDectector();
    void Non_maxima_Suppression();
    void Hysteresis_Thresholding(int T_low, int T_high);

    //Hough Transform
    std::pair<int, int> Hough_Transform_Line_Detector();
    std::tuple<int, int, int> Hough_Transform_Circle_Detector();
    void Draw_Line(int d, int theta);
    void Draw_Circle(int a, int b, int r);

    //Data
    Ui::MainWindow *ui;
    QMessageBox MsgBox;

    QString imagepath;
    bitmap_image image;
    bitmap_image image_intensity;
    bitmap_image image_binary;
    bitmap_image bounded_image_binary;

    bitmap_image smoothed_image;
    bitmap_image gradient;
    bitmap_image direction;
    bitmap_image image_edge_dect;
    bitmap_image Line_detect;
    bitmap_image Circle_detect;

    int width;
    int height;

    rgb_t Color[40], black, white;

    int histogram[256];
    int red_histogram[256], blue_histogram[256], green_histogram[256];

    // Noise elimination filter
    std::vector<std::vector<double>> Mean_Kernel;

    //Structuring element for dilation and erosion
    std::vector<std::vector<unsigned char>> Structuring_Element;

    //For Connected Component Analysis
    std::vector<Object> objects;
    std::vector<std::vector<int>> label_matrix;
    int no_of_objects;

    //For Training
    std::vector<Object> training_objects;

    //Smoothening
    std::vector<std::vector<double>> Gaussian_Kernel;

    //Sobel Filter
    std::vector<std::vector<double>> Gx_Kernel;
    std::vector<std::vector<double>> Gy_Kernel;

};

#endif // MAINWINDOW_H
