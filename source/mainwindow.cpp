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

    //Gaussian Kernel
    Gaussian_Kernel.resize(3, std::vector<double> (3));
    Gaussian_Kernel[0][0] =  0.0625; Gaussian_Kernel[0][1] = 0.125; Gaussian_Kernel[0][2] =   0.0625;
    Gaussian_Kernel[1][0] =  0.125; Gaussian_Kernel[1][1] = 0.25; Gaussian_Kernel[1][2] =   0.125;
    Gaussian_Kernel[2][0] = 0.0625; Gaussian_Kernel[2][1] =0.125; Gaussian_Kernel[2][2] =  0.0625;

    //Sobel Horizontal Kernel
    Gx_Kernel.resize(3, std::vector<double> (3));
    Gx_Kernel[0][0] =  -1; Gx_Kernel[0][1] = 0; Gx_Kernel[0][2] =  1;
    Gx_Kernel[1][0] =  -2; Gx_Kernel[1][1] = 0; Gx_Kernel[1][2] =  2;
    Gx_Kernel[2][0] =  -1; Gx_Kernel[2][1] = 0; Gx_Kernel[2][2] =  1;

    //Sobel Vertical Kernel
    Gy_Kernel.resize(3, std::vector<double> (3));
    Gy_Kernel[0][0] =  -1; Gy_Kernel[0][1] = -2; Gy_Kernel[0][2] = -1;
    Gy_Kernel[1][0] =   0; Gy_Kernel[1][1] =  0; Gy_Kernel[1][2] =  0;
    Gy_Kernel[2][0] =   1; Gy_Kernel[2][1] =  2; Gy_Kernel[2][2] =  1;

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

//Calculating Gray-Level Image Histogram
void MainWindow::Calculate_Intensity_Hist(){
    for(int i=0; i<256; i++) this->histogram[i]=0;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            this->histogram[image_intensity.get_pixel(i, j).blue]++;
        }
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

//Calculating Colored-Image Histogram
void MainWindow::Calculate_Colored_Hist(){
    for(int i=0; i<256; i++){
        this->red_histogram[i]=this->blue_histogram[i]=this->green_histogram[i]=0;
    }

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            this->red_histogram[image.get_pixel(i, j).red]++;
            this->blue_histogram[image.get_pixel(i, j).blue]++;
            this->green_histogram[image.get_pixel(i, j).green]++;
        }
    }
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

//Thresholding Function for creating binary image
void MainWindow::Threshold(int T, bool display){
    this->image_binary=this->image_intensity;

    unsigned char background_intensity;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            rgb_t color=image_binary.get_pixel(i, j);
            if(i==0 && j==0){
                background_intensity=color.red<T?0: 255;
            }
            unsigned char intensity;
            if(background_intensity==0){
                intensity=color.red<T?0: 255;
            }
            else{
                intensity=color.red<T?255: 0;
            }

            color.red=color.green=color.blue=intensity;
            image_binary.set_pixel(i, j, color);
        }
    }
    if(display){
        image_binary.save_image("threshold.bmp");
        display_Image("threshold.bmp", "Image Segmentation: Otsu Thresholding");
    }
}

void MainWindow::Otsu_Threshold(bool display){
    Calculate_Intensity_Hist();
    int T=0; double max=0;
    for(int i=0; i<256; i++){
        double Weight_b=0, Mean_b=0, Weight_f=0, Mean_f=0;
        for(int j=0; j<i; j++){
            Weight_b+=this->histogram[j];
            Mean_b+=(this->histogram[j]*j);
        }
        for(int j=i; j<256; j++){
            Weight_f+=this->histogram[j];
            Mean_f+=(this->histogram[j]*j);
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

    std::cout<<"The threshold point is "<<T<<std::endl;
    Threshold(T, display);
}

//Otsu Thresholding Method
void MainWindow::on_actionOtsu_Threshold_triggered()
{
    Otsu_Threshold(true);
}

//Random Number generating Function
int MainWindow::generate_random(int max){
    int min=0;
    double x = rand()/static_cast<double>(RAND_MAX);
    int random = min + static_cast<int>( x * (max - min) );
    return random;
}

//K-Means Method for Thresholding-Binary Image
void MainWindow::on_actionK_Means_Binary_triggered()
{
    int k=2;
    int past_mean_centroid[k];
    int present_mean_centroid[k];

    std::vector<int> centroid[k];

    Calculate_Intensity_Hist();
    present_mean_centroid[0]=generate_random(255);
    present_mean_centroid[1]=generate_random(255);

    int count=0;
    do{
        for(int i=0; i<k; i++) {
            past_mean_centroid[i]=present_mean_centroid[i];
            centroid[i].clear();
        }

        for(int i=0; i<256; i++){
            int nearest=0; double distance=0;
            for(int j=0; j<k; j++){
                double d=abs(present_mean_centroid[j]-i);
                if(d>distance){
                    distance=d; nearest=j;
                }
            }
            centroid[nearest].push_back(i);
        }

        for(int i=0; i<k; i++){
            double mean=0, hist=0;
            for(int j=0; j<(int)centroid[i].size(); j++){
                mean+=(centroid[i][j]*histogram[centroid[i][j]]); hist+=histogram[centroid[i][j]];
            }
            present_mean_centroid[i]=mean/hist;
        }
        count++;
        std::cout<<past_mean_centroid[0]<<"--"<<present_mean_centroid[0]<<std::endl;
    }while(count<10);//while(!((past_mean_centroid[0]==present_mean_centroid[0])&&(past_mean_centroid[1]==present_mean_centroid[1])));

    this->image_binary=this->image_intensity;

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            rgb_t color=image_binary.get_pixel(i, j);

            int group;
            for(int c=0; c<k; c++){
                std::vector<int>::iterator it=std::find(centroid[c].begin(), centroid[c].end(), (int)color.red);
                if(it!=centroid[c].end()){
                    group=c;
                    break;
                }
            }

            unsigned char intensity=group==0?0: 255;
            color.red=color.green=color.blue=intensity;
            image_binary.set_pixel(i, j, color);
        }
    }

    image_binary.save_image("threshold.bmp");
    display_Image("threshold.bmp", "Image Segmentation: K-Means Clustering Binary");
}

//Check if centroids are similar
bool MainWindow::is_centroids_same(rgb_t *C1, rgb_t *C2, int N){
    for(int i=0; i<N; i++){
        if(abs((int)C1[i].red-(int)C2[i].red)<=0.1){
            return false;
        }
        else if(abs((int)C1[i].blue-(int)C2[i].blue)<=0.1){
            return false;
        }
        else if(abs((int)C1[i].green-(int)C2[i].green)<=0.1){
            return false;
        }
        else{}
    }
    return true;
}

// K-Means Method for Thresholding-Colored Image
void MainWindow::on_actionK_Means_triggered()
{
    QString nstring = QInputDialog::getText(0,"Input number of clusters","Input");
    int k= nstring.toInt();
    int iteration=40;

    //Initializing k clusters
    rgb_t past_mean_centroid[k];
    rgb_t present_mean_centroid[k];

    //For grouping
    Neighbor centroid[k];

    Calculate_Colored_Hist();
    for(int i=0; i<k; i++){
        int x=generate_random(static_cast<int>(image.width()));
        int y=generate_random(static_cast<int>(image.height()));
        present_mean_centroid[i]=image.get_pixel(x, y);
        //present_mean_centroid[i]={(unsigned char)generate_random(255), (unsigned char)generate_random(255), (unsigned char)generate_random(255)};
    }
    int count=0;
    do{
        for(int i=0; i<k; i++) {
            past_mean_centroid[i]=present_mean_centroid[i];
            centroid[i].neighbor.clear();
        }

        for(int i=0; i<width; i++){
            for(int j=0; j<height; j++){
                int nearest=0; double distance=0;
                rgb_t color=image.get_pixel(i, j);
                for(int c=0; c<k; c++){
                    double d=sqrt((pow(present_mean_centroid[c].red-color.red, 2)+
                                  pow(present_mean_centroid[c].blue-color.blue, 2)+
                                  pow(present_mean_centroid[c].green-color.green, 2)));
                    if(d>distance){
                        distance=d; nearest=c;
                    }
                }
                Point P={i, j};
                centroid[nearest].neighbor.push_back(P);
            }
        }

        //Average mean of each clusters

        for(int i=0; i<k; i++){
            double mean_red=0, mean_blue=0, mean_green=0;
            if (count==0) std::cout<<centroid[i].neighbor.size()<<std::endl;
            for(int j=0; j<(int)centroid[i].neighbor.size(); j++){
                int x=centroid[i].neighbor[j].x;
                int y=centroid[i].neighbor[j].y;
                rgb_t color=image.get_pixel(x, y);
                mean_red+=color.red; mean_blue+=color.blue; mean_green+=color.green;
            }
            rgb_t new_centroid={static_cast<unsigned char>(mean_red/k),
                               static_cast<unsigned char>(mean_green/k),
                               static_cast<unsigned char>(mean_blue/k)};
            present_mean_centroid[i]=new_centroid;
        }
        std::cout<<"iteration "<<count<<std::endl;
        count++;
    }/*while(count<iteration);*/while(!is_centroids_same(present_mean_centroid, past_mean_centroid, k));

    this->image_binary=this->image;
    for(int i=0; i<k; i++){
        std::cout<<centroid[i].neighbor.size()<<std::endl;
        for(int j=0; j<(int)centroid[i].neighbor.size(); j++){
            int x=centroid[i].neighbor[j].x;
            int y=centroid[i].neighbor[j].y;
            image_binary.set_pixel(x, y, Color[i]);//present_mean_centroid[i]);
        }
    }

    image_binary.save_image("threshold.bmp");
    display_Image("threshold.bmp", "Image Segmentation: K-Means Clustering");
}

//Convolution Function
bitmap_image MainWindow::convolute(std::vector<std::vector<double>> Kernel, bool normal){
    bitmap_image output=normal?this->image: this->smoothed_image;

    //Gradient X=Image Matrix*KernelX and Gradient Y=Image Matrix*Kernel
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            double Convolute_red=0, Convolute_blue=0, Convolute_green=0;
            for (int k=-1; k<=1; k++){
                for (int l=-1; l<=1; l++){
                    if (normal){    //For Colored Filter e.g mean Filter
                        Convolute_red+= Kernel[k+1][l+1] * image.get_pixel(i+k, j+l).red;
                        Convolute_blue+= Kernel[k+1][l+1] * image.get_pixel(i+k, j+l).blue;
                        Convolute_green+= Kernel[k+1][l+1] * image.get_pixel(i+k, j+l).green;
                    }
                    else{   //For Intensity Filters e.g gaussian and sobel filter
                        Convolute_red+= Kernel[k+1][l+1] * image_intensity.get_pixel(i+k, j+l).red;
                        Convolute_blue=Convolute_green=Convolute_red;
                    }
                }
            }
            /*
            Convolute_red=Convolute_red>255? 255:Convolute_red;
            Convolute_red=Convolute_red<0? 0:Convolute_red;

            Convolute_blue=Convolute_blue>255? 255:Convolute_blue;
            Convolute_blue=Convolute_blue<0? 0:Convolute_blue;

            Convolute_green=Convolute_green>255? 255:Convolute_green;
            Convolute_green=Convolute_green<0? 0:Convolute_green;
            */

            rgb_t convolute={Convolute_red, Convolute_green, Convolute_blue};

            output.set_pixel(i, j, convolute);
        }
    }
    return output;
}

// Display filter of input Image
void MainWindow::on_actionFilter_triggered()
{
    bitmap_image conv=convolute(Mean_Kernel, true);
    this->image=conv;
    conv.save_image("convolute.bmp");
    display_Image("convolute.bmp", "Convolute");
}

//get minimum label during collision in Connected Component Analysis
int MainWindow::get_minimum_Label(std::vector<int> array){
    int min=1000;
    for(int i=0; i<array.size(); i++){
        if(array[i]<min){min=array[i];}
    }
    return min;
}

//Connected Component Analysis
bitmap_image MainWindow::Connected_Component_Analysis(){
    label_matrix.clear();
    this->objects.clear();
    this->no_of_objects=0;

    bitmap_image output=this->image_binary;
    label_matrix.resize(this->image_binary.width(), std::vector<int>(this->image_binary.height()));

    for(int i=0; i<label_matrix.size(); i++){
        for(int j=0; j<label_matrix[0].size(); j++){
            label_matrix[i][j]=0;
        }
    }

    int label=1;

    //Connected Component Analysis....
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            if(image_binary.get_pixel(i, j).red==0){
                continue;
            }

            //Array for storing labels for each pixels
            std::vector<int> collision;
            for (int k=-1; k<=1; k++){
                for (int l=-1; l<=1; l++){
                    if(label_matrix[i+k][j+l]!=0){
                        //add label if not in collision list
                        if(std::find(collision.begin(), collision.end(), label_matrix[i+k][j+l])==collision.end()){
                            collision.push_back(label_matrix[i+k][j+l]);
                        }
                    }
                }
            }
            if(collision.empty()){ //new label
                label++;
                label_matrix[i][j]=label;
            }
            else if(collision.size()==1){   //only one label
                label_matrix[i][j]=collision[0];
            }
            else{ //collision between labels
                label_matrix[i][j]=get_minimum_Label(collision);

                for (int s=1; s<this->width-1; s++){
                    for (int t=1; t<this->height-1; t++){
                        //if label matrix[s][t] in collision vector
                        if(std::find(collision.begin(), collision.end(), label_matrix[s][t])!=collision.end()){
                            label_matrix[s][t]=label_matrix[i][j];
                        }
                    }
                }
            }
        }
    }

    std::vector<int> labels;
    int index;

    for (int i=0; i<this->width; i++){
        for (int j=0; j<this->height; j++){
            if(std::find(labels.begin(), labels.end(), label_matrix[i][j])==labels.end()){
                labels.push_back(label_matrix[i][j]);
                Object obj; obj.label=label_matrix[i][j]; obj.color_index=no_of_objects;
                objects.push_back(obj);
                no_of_objects++;
                //qDebug()<<"Label "<<label_matrix[i][j]<<" and color is "<<no_of_objects-1;
            }
            for(int c=0; c<objects.size(); c++){
                if(objects[c].label==label_matrix[i][j]){
                    index=objects[c].color_index;
                    break;
                }
            }
            output.set_pixel(i, j, this->Color[index]);
        }
    }

    for (int i=0; i<this->width; i++){
        for (int j=0; j<this->height; j++){
            if(label_matrix[i][j]!=0){
                std::vector<int>::iterator it=std::find(labels.begin(), labels.end(), label_matrix[i][j]);
                label_matrix[i][j]=std::distance(labels.begin(), it);
            }
        }
    }

    no_of_objects--;
    qDebug()<<"Number of Objects is: "<<no_of_objects;
    this->MsgBox.setText("There are "+QString::number(no_of_objects)+" objects in image");
    this->MsgBox.exec();
    return output;
}

//Displaying each objects after CCA
void MainWindow::on_actionConnected_Component_Analysis_triggered()
{
    bitmap_image CCA=Connected_Component_Analysis();
    CCA.save_image("CCA.bmp");
    display_Image("CCA.bmp", "Connected Component Analysis");
}

//Morphology
//Dilation Operation
void MainWindow::Dilate(){
    bitmap_image temp_image=image_binary;
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            if(image_binary.get_pixel(i, j).red==255) continue;
            if(Structuring_Element[1][1]==0) continue;

            bool set_foreground=false;
            for (int k=-1; k<=1; k++){
                for (int l=-1; l<=1; l++){
                    if(image_binary.get_pixel(i+k, j+l).red==255 && Structuring_Element[k+1][l+1]==255){
                        set_foreground=true;
                        break;
                    }
                }
                if(set_foreground) break;
            }
            if(set_foreground){
                rgb_t color; unsigned char intensity=255;
                color.red=color.green=color.blue=intensity;
                temp_image.set_pixel(i, j, color);
            }
        }
    }
    image_binary=temp_image;
}

void MainWindow::on_actionDilation_triggered()
{
    //for (int i=0; i<12; i++)
    Dilate();
    image_binary.save_image("dilation.bmp");
    display_Image("dilation.bmp", "Morphology: Dilation");
}

void MainWindow::Erosion(){
    bitmap_image temp_image=image_binary;
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            if(image_binary.get_pixel(i, j).red==0) continue;
            if(Structuring_Element[1][1]==0) continue;

            bool set_background=false;
            for (int k=-1; k<=1; k++){
                for (int l=-1; l<=1; l++){
                    if(Structuring_Element[k+1][l+1]==255 && image_binary.get_pixel(i+k, j+l).red==0){
                        set_background=true;
                        break;
                    }
                }
                if(set_background) break;
            }
            if(set_background){
                unsigned char intensity=0; rgb_t color;
                color.red=color.green=color.blue=intensity;
                temp_image.set_pixel(i, j, color);
            }
        }
    }
    image_binary=temp_image;
}

void MainWindow::on_actionErosion_triggered()
{
    //for (int i=0; i<12; i++)
    Erosion();
    image_binary.save_image("erosion.bmp");
    display_Image("erosion.bmp", "Morphology: Erosion");
}

void MainWindow::on_actionOpen_2_triggered()
{
    Erosion(); Dilate();
    image_binary.save_image("opening.bmp");
    display_Image("opening.bmp", "Morphology: Opening");
}

void MainWindow::on_actionClosing_triggered()
{
    Dilate(); Erosion();
    image_binary.save_image("closing.bmp");
    display_Image("closing.bmp", "Morphology: Closing");
}

void MainWindow::on_actionBoundary_Extraction_triggered()
{
    bitmap_image A=image_binary;
    Erosion();
    for(int i=0; i<this->width; i++){
        for(int j=0; j<this->height; j++){
            unsigned char intensity=(A.get_pixel(i, j).red==255 && image_binary.get_pixel(i, j).red==0)? 255: 0;
            rgb_t color;
            color.red=color.green=color.blue=intensity;
            image_binary.set_pixel(i, j, color);
        }
    }
    image_binary.save_image("boundary_Extraction.bmp");
    display_Image("boundary_Extraction.bmp", "Morphology: Boundary Extraction");
}

void MainWindow::on_actionDraw_Object_Edge_triggered()
{
    //Threshold Image
    //Otsu_Threshold(false);
    bounded_image_binary=image_binary;

    //Boundary Detection
    bitmap_image A=image_binary;
    Erosion();
    for(int i=0; i<this->width; i++){
        for(int j=0; j<this->height; j++){
            unsigned char intensity=(A.get_pixel(i, j).red==255 && image_binary.get_pixel(i, j).red==0)? 255: 0;
            rgb_t color;
            color.red=color.green=color.blue=intensity;
            image_binary.set_pixel(i, j, color);
        }
    }

    //Connected Component Analysis
    Connected_Component_Analysis();

    //Retreiving Binary Image
    image_binary=bounded_image_binary;

    //Setting color of each object edges or boundary box
    for(int i=0; i<this->width; i++){
        for(int j=0; j<this->height; j++){
            if(label_matrix[i][j]!=0){
                bounded_image_binary.set_pixel(i, j, this->Color[0]);
            }
        }
    }
    bounded_image_binary.save_image("bounded_image.bmp");
    display_Image("bounded_image.bmp", "Morphology: Edge-Bounded Image");
}

void MainWindow::on_actionDraw_Bound_Box_triggered()
{
    //Threshold Image
    //Otsu_Threshold(false);
    bounded_image_binary=image_binary;

    //Boundary Detection
    bitmap_image A=image_binary;
    Erosion();
    for(int i=0; i<this->width; i++){
        for(int j=0; j<this->height; j++){
            unsigned char intensity=(A.get_pixel(i, j).red==255 && image_binary.get_pixel(i, j).red==0)? 255: 0;
            rgb_t color;
            color.red=color.green=color.blue=intensity;
            image_binary.set_pixel(i, j, color);
        }
    }
    //Connected Component Analysis
    Connected_Component_Analysis();
    //Creating boundary box from label matrix
    //Begin
    objects.clear();
    objects.resize(no_of_objects+1);
    for(int i=0; i<this->width; i++){
        for(int j=0; j<this->height; j++){
            if(label_matrix[i][j]!=0){
                if(i<objects[label_matrix[i][j]].min_width) objects[label_matrix[i][j]].min_width=i;
                if(i>objects[label_matrix[i][j]].max_width) objects[label_matrix[i][j]].max_width=i;
                if(j<objects[label_matrix[i][j]].min_height) objects[label_matrix[i][j]].min_height=j;
                if(j>objects[label_matrix[i][j]].max_height) objects[label_matrix[i][j]].max_height=j;
                label_matrix[i][j]=0; //turn edge boundaries to black
            }
        }
    }

    for(int c=1; c<=no_of_objects; c++){
        objects[c].label=objects[c].color_index=c;
        objects[c].width=objects[c].max_width-objects[c].min_width+1;
        objects[c].height=objects[c].max_height-objects[c].min_height+1;
        //top
        for(int i=objects[c].min_width; i<=objects[c].max_width; i++) label_matrix[i][objects[c].min_height]=c;
        //bottom
        for(int i=objects[c].min_width; i<=objects[c].max_width; i++) label_matrix[i][objects[c].max_height]=c;
        //left
        for(int i=objects[c].min_height; i<=objects[c].max_height; i++) label_matrix[objects[c].min_width][i]=c;
        //right
        for(int i=objects[c].min_height; i<=objects[c].max_height; i++) label_matrix[objects[c].max_width][i]=c;
    }
    //End Boundary Box

    //Retreiving Binary Image
    image_binary=bounded_image_binary;

    //Setting color of each object edges or boundary box
    for(int i=0; i<this->width; i++){
        for(int j=0; j<this->height; j++){
            if(label_matrix[i][j]!=0){
                bounded_image_binary.set_pixel(i, j, this->Color[0]);
            }
        }
    }
    bounded_image_binary.save_image("bounded_image.bmp");
    display_Image("bounded_image.bmp", "Morphology: Box-Bounded Image");
}

//η-Function
double MainWindow::eta(double p, double q, int l){
    return mu(p,q,l)/pow(mu(0,0,l), gamma(p, q)); //is it power of gamma?
}

//μ-Function
double MainWindow::mu(double p, double q, int l){
    double mean_r=moment(1, 0, l)/moment(0, 0, l);
    double mean_c=moment(0, 1, l)/moment(0, 0, l);
    double sum=0;
    for(int i=objects[l].min_width; i<=objects[l].max_width; i++){
        for(int j=objects[l].min_height; j<=objects[l].max_height; j++){
            int pixel=image_binary.get_pixel(i, j).red==0? 0:1;
            sum+=(pow((i-mean_r),p)*pow((j-mean_c),q)*pixel);
        }
    }
    return sum;
}

//γ-Function
double MainWindow::gamma(double p, double q){
    return ((p+q)/2)+1;
}

//moment-Function
double MainWindow::moment(double p, double q, int l){
    double sum=0;
    for(int i=objects[l].min_width; i<=objects[l].max_width; i++){
        for(int j=objects[l].min_height; j<=objects[l].max_height; j++){
            int pixel=image_binary.get_pixel(i, j).red==0? 0:1;
            sum+=(pow(i,p)*pow(j,q)*pixel);
        }
    }
    return sum;
}

void MainWindow::Feature_Extraction(bool save_RSI_Feature_vector){
    on_actionDraw_Bound_Box_triggered();

    std::ofstream outData;
    if(save_RSI_Feature_vector)outData.open("Objects_Features");

    for(int c=1; c<=no_of_objects; c++){
        objects[c].Feature_vector.resize(7);
        double a=eta(2,0,c);
        double b=eta(0,2,c);
        double h=eta(1,1,c);
        double d=eta(3,0,c)-(3*eta(1,2,c));
        double e=(3*eta(2,1,c))-eta(0,3,c);
        double f=eta(3,0,c)+eta(1,2,c);
        double g=eta(2,1,c)+eta(0,3,c);

        //φ1
        objects[c].Feature_vector[0]=a+b;
        //φ2
        objects[c].Feature_vector[1]=pow(a-b, 2) + (4*pow(h, 2));
        //φ3
        objects[c].Feature_vector[2]=pow(d, 2) + pow(e, 2);
        //φ4
        objects[c].Feature_vector[3]=pow(f, 2) + pow(g, 2);
        //φ5
        objects[c].Feature_vector[4]=(d*f*(pow(f, 2) - (3*pow(g, 2))))+(e*g*((3*pow(f, 2)) - pow(g, 2)));
        //φ6
        objects[c].Feature_vector[5]=((a-b)*(pow(f, 2)-pow(g, 2)))+(4*h*f*g);
        //φ7
        objects[c].Feature_vector[6]=(e*f*(pow(f, 2) - (3*pow(g, 2))))+(d*g*((3*pow(f, 2)) - pow(g, 2)));

        if(save_RSI_Feature_vector){
            outData<<"Object "<<c<<std::endl;
            for(int k=0; k<7; k++){
                if(k!=6) outData<<objects[c].Feature_vector[k]<<" ";
                else outData<<objects[c].Feature_vector[k];
            }
            outData<<std::endl;
        }

        //saving each objects as a bmp file
        bitmap_image obj(objects[c].width, objects[c].height);
        for(int i=objects[c].min_width; i<=objects[c].max_width; i++){
            for(int j=objects[c].min_height; j<=objects[c].max_height; j++){
                obj.set_pixel(i-objects[c].min_width, j-objects[c].min_height,image_binary.get_pixel(i, j));
            }
        }
        obj.save_image("./objects/object"+std::to_string(c)+".bmp");
    }
    if(save_RSI_Feature_vector) outData.close();
}

void MainWindow::on_actionFeature_Extraction_triggered()
{
    Feature_Extraction(true);
}

void MainWindow::Training(){
    training_objects.clear();

    QString fname = QFileDialog::getOpenFileName(NULL, QObject::tr("Select Training Data"));
    if(fname.isEmpty()){
        this->MsgBox.setText("Select an image");
        this->MsgBox.exec();
        return;
    }

    std::string filename=fname.toStdString();
    std::ifstream inData(filename, std::ios::in);

    std::string line, object_name;
    int object=1, count;
    std::vector<double> total_phi(7);

    while(std::getline(inData, line)){
        std::vector<double> phi(7, 0);
        if(line==("Object "+std::to_string(object))){
            for(int i=0; i<7; i++) total_phi[i]=0;
            inData>>object_name;    //get object name
            inData>>count;
            for(int i=0; i<count; i++){
                for(int j=0; j<7; j++) inData>>phi[j]; //get object RST vectors
                for(int j=0; j<7; j++) total_phi[j]+=phi[j];
            }
            object++;
            Object Obj; Obj.name=object_name;
            for(int i=0; i<7; i++) total_phi[i]/=count;
            Obj.Feature_vector= total_phi;
            training_objects.push_back(Obj);
        }
        else{

        }
   }
   inData.close();

   for(int i=0; i<training_objects.size(); i++){
       std::cout<<"Object Name: "<<training_objects[i].name<<std::endl;
       for(int j=0; j<7; j++){
           std::cout<<training_objects[i].Feature_vector[j]<<" ";
       }
       std::cout<<std::endl;
   }
}



void MainWindow::on_actionTrain_triggered()
{
    Training();
}

double MainWindow::Euclidean(std::vector<double> v1, std::vector<double>v2){
    double result=0;
    for(int i=0; i<7; i++){
        result+=pow(v1[i]-v2[i], 2);
    }
    return sqrt(result);
}


void MainWindow::Test(){
    Feature_Extraction(false);

    std::string text="";
    std::vector<std::string> object_set;
    for(int i=1; i<=no_of_objects; i++){
        int index=0; double min=DBL_MAX;
        for(int j=0; j<training_objects.size(); j++){
            double distance=Euclidean(objects[i].Feature_vector, training_objects[j].Feature_vector);
            if(distance<min){
                min=distance;
                index=j;
            }
        }
        objects[i].name=training_objects[index].name;
        text+="object"+std::to_string(i)+" is a "+objects[i].name+"\n";

        //add object name if not in list
        if(std::find(object_set.begin(), object_set.end(), objects[i].name)==object_set.end()){
            object_set.push_back(objects[i].name);
        }
    }

    //Classifying each objects by saving each class in a bmp file
    rgb_t black_color;  //black pixel
    black_color.red=black_color.green=black_color.blue=0;
    text+="...................\n";
    for(int set=0; set<object_set.size(); set++){
        int count=0;
        bitmap_image object_class=image_binary;
        for(int c=1; c<=no_of_objects; c++){
            if(objects[c].name!=object_set[set]){   //if the object is not in class turn its pixel black
                for(int i=objects[c].min_width; i<=objects[c].max_width; i++){
                    for(int j=objects[c].min_height; j<=objects[c].max_height; j++){
                        object_class.set_pixel(i, j, black_color);
                    }
                }
            }
            else{count++;}
        }
        text+="Class "+object_set[set]+": "+std::to_string(count)+" Objects\n";
        object_class.save_image("./class/"+object_set[set]+".bmp");
    }

    this->MsgBox.setText(QString::fromUtf8(text.c_str()));
    this->MsgBox.exec();
}

void MainWindow::on_actionTest_triggered()
{
    Test();
}

//Smoothening
void MainWindow::Smoothening_Gaussian_Filter(){
    this->smoothed_image=this->image_intensity;
    this->smoothed_image=convolute(Gaussian_Kernel, false);
}

//Gx
bitmap_image MainWindow::Calc_Horizontal_Gradient(){
    Smoothening_Gaussian_Filter();
    return convolute(Gx_Kernel, false);
}

//Gy
bitmap_image MainWindow::Calc_Vertical_Gradient(){
    Smoothening_Gaussian_Filter();
    return convolute(Gy_Kernel, false);
}

//Calculating Gradient and Orientation
void MainWindow::Calc_Gradient_n_Direction(){
    Smoothening_Gaussian_Filter();
    this->gradient=this->smoothed_image;
    this->direction=this->smoothed_image;

    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            double G_x=0, G_y=0;
            for (int k=-1; k<=1; k++){
                for (int l=-1; l<=1; l++){
                    G_x+= Gx_Kernel[k+1][l+1] * smoothed_image.get_pixel(i+k, j+l).red;
                    G_y+= Gy_Kernel[k+1][l+1] * smoothed_image.get_pixel(i+k, j+l).red;
                }
            }
            double G=sqrt(pow(G_x, 2) + pow(G_y, 2));
            rgb_t Gradient={G, G, G};
            this->gradient.set_pixel(i, j, Gradient);

            double angle=atan(G_y/G_x)*180/M_PI;
            rgb_t theta={angle, angle, angle};
            this->direction.set_pixel(i, j, theta);
        }
    }
}

//Non-Maximum Suppression
void MainWindow::Non_maxima_Suppression(){
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            int theta=this->direction.get_pixel(i, j).red;
            while (theta<0) theta+=360; //theta in range of 0 and 360
            theta%=180; //theta=orientation%180 so no need to calculate lower quadrants
            double G=this->gradient.get_pixel(i, j).red;
            double P1, P2;
            if (theta<=22.5 || theta>157.5){    //rule 0-degree
                P1=this->gradient.get_pixel(i, j-1).red;
                P2=this->gradient.get_pixel(i, j+1).red;
            }
            else if(theta<=67.5){   //rule 45-degree
                P1=this->gradient.get_pixel(i+1, j-1).red;
                P2=this->gradient.get_pixel(i-1, j+1).red;
            }
            else if(theta<=112.5){   //rule 90-degree
                P1=this->gradient.get_pixel(i+1, j).red;
                P2=this->gradient.get_pixel(i-1, j).red;
            }
            else if(theta<=157.5){   //rule 135-degree
                P1=this->gradient.get_pixel(i-1, j-1).red;
                P2=this->gradient.get_pixel(i+1, j+1).red;
            }
            if(G<P1 || G<P2){
                this->image_edge_dect.set_pixel(i, j, black);
            }
        }
    }
}

//Hysterisis Thresholding
void MainWindow::Hysteresis_Thresholding(int T_low, int T_high){
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            int G=this->gradient.get_pixel(i, j).red;
            if(G<T_low){    //remove it since its not an edge
                this->image_edge_dect.set_pixel(i, j, black);
            }
            else if(G>T_low && G<T_high){
                /*If all neighbors are less than T_high then its not an edge
                 so we can remove it*/
                bool isedge=false;
                for (int k=-1; k<=1; k++){
                    for (int l=-1; l<=1; l++){
                        if(image_edge_dect.get_pixel(i+k, j+l).red>T_high){
                            isedge=true;
                            break;
                        }
                    }
                    if(isedge) break;
                }
                if(!isedge) this->image_edge_dect.set_pixel(i, j, black);   //not an edge
            }

        }
    }
}

//Canny Edge Detection
void MainWindow::Canny_EdgeDectector(){
    Calc_Gradient_n_Direction();
    image_edge_dect=gradient;

    //Non-Maxima Suppression
    Non_maxima_Suppression();
    image_edge_dect.save_image("non_maxima.bmp");
    display_Image("non_maxima.bmp", "Canny Edge Detector: Non-Maxima Supression");

    //Hysterisis Thresholding
    QString nstring = QInputDialog::getText(0,"Input Low Threshold","Input");
    int T_low=nstring.toInt();
    nstring.clear();
    nstring = QInputDialog::getText(0,"Input High Threshold","Input");
    int T_high=nstring.toInt();
    Hysteresis_Thresholding(T_low, T_high);
}

//Hough Thransform Line Segment
std::pair<int, int> MainWindow::Hough_Transform_Line_Detector(){
    int d_max=sqrt(pow(this->width, 2)+pow(this->height, 2));
    int theta_max=180;
    int accuracy=1;
    theta_max=int(theta_max/accuracy);
    int Hough_Counter[d_max][theta_max];

    //Initializing Counter
    for(int i=0; i<d_max; i++){
        for(int j=0; j<theta_max; j++){
            Hough_Counter[i][j]=0;
        }
    }

    //Finding H(d, theta) for each edge points
    double Deg2Rad=M_PI/180;
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            if(image_edge_dect.get_pixel(i, j).red!=0){   //is edge
                for(int theta=0; theta<theta_max; theta+=accuracy){
                     int d=abs(int((i*cos(theta*Deg2Rad))+(j*sin(theta*Deg2Rad))));
                     Hough_Counter[d][theta]++;
                }
            }
        }
    }

    //Finding maximum H(d, theta)
    int Line_d=0, Line_theta=0, max_counter=-1;
    for(int i=0; i<d_max; i++){
        for(int j=0; j<theta_max; j++){
            if(Hough_Counter[i][j]>max_counter){
                max_counter=Hough_Counter[i][j];
                Line_d=i; Line_theta=j;
                if(max_counter>=2) Draw_Line(i, j);
            }
        }
    }
    return std::make_pair(Line_d, Line_theta);
}

void MainWindow::Draw_Line(int d, int theta){
    double Deg2Rad=M_PI/180;
    if(theta==0){
        for (int i=1; i<this->width-1; i++){
            int j=int((d/cos(theta*Deg2Rad))-(i*tan(theta*Deg2Rad)));
            Line_detect.set_pixel(i, j, Color[0]);
        }
    }
    else{
        for (int j=1; j<this->height; j++){
            int i=int((d/sin(theta*Deg2Rad))-(j/tan(theta*Deg2Rad)));
            Line_detect.set_pixel(i, j, Color[0]);
        }
    }
}
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
    int d=Line.first, theta=Line.second;

    Draw_Line(d, theta);
    Line_detect.save_image("line_detection.bmp");
    display_Image("line_detection.bmp", "Hough transform: Line Detection");

}

void MainWindow::on_actionHough_Transform_Circle_triggered()
{

}

