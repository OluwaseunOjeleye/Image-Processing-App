#include "mainwindow.h"
#include "ui_mainwindow.h"

//Calculating Gray-Level Image Histogram
void MainWindow::Calculate_Intensity_Hist(){
    for(int i=0; i<256; i++) this->histogram[i]=0;
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            this->histogram[image_intensity.get_pixel(i, j).blue]++;
        }
    }
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

//Inverting Background
void MainWindow::Invert_Binary_Image_Background(){
    rgb_t background_color=this->image_binary.get_pixel(0, 0);
    if(background_color.red==0) return;

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            if(image_binary.get_pixel(i, j).red==255){
                image_binary.set_pixel(i,j, black);
            }
            else{
                 image_binary.set_pixel(i,j, white);
            }
        }
    }

}

//**********************************Image Thresholding********************************************************
//Thresholding Function for creating binary image
void MainWindow::Threshold(int T, bool display){
    this->image_binary=this->image_intensity;

    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            if(image_binary.get_pixel(i, j).red<T){
                image_binary.set_pixel(i, j, black);
            }
            else{
                image_binary.set_pixel(i, j, white);
            }
        }
    }
    Invert_Binary_Image_Background();

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
    int past_mean_centroid[k];  //Past K1 and K2 centroid mean
    int present_mean_centroid[k];   //New K1 and K2 centroid mean

    std::vector<int> centroid[k];   //Gray level values in K1 and K2
    int old_Error[k], new_Error[k]; //For changes in old centroid and new centroid

    Calculate_Intensity_Hist();
    for(int i=0; i<k; i++){
        present_mean_centroid[i]=generate_random(255);
        new_Error[i]=0;
    }

    do{
        for(int i=0; i<k; i++) {
            past_mean_centroid[i]=present_mean_centroid[i];
            old_Error[i]=new_Error[i];
            centroid[i].clear();
        }

        //Classifying gray level values in K1 and K2 depending on the nearest centroid
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

        //Calculating mean of each centroids
        for(int i=0; i<k; i++){
            double mean=0, hist=0;
            for(int j=0; j<(int)centroid[i].size(); j++){
                mean+=(centroid[i][j]*histogram[centroid[i][j]]); hist+=histogram[centroid[i][j]];
            }
            present_mean_centroid[i]=mean/hist;
            new_Error[i]=abs(present_mean_centroid[i]-past_mean_centroid[i]);
        }
    }while(!(new_Error[0]==old_Error[0] && new_Error[1]==old_Error[1])); //Terminate while there are no changes in cluster centres

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
            if (group==0) image_binary.set_pixel(i, j, black);
            else image_binary.set_pixel(i, j, white);
        }
    }
    Invert_Binary_Image_Background();

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


//**********************************Convolution Function********************************************************
bitmap_image MainWindow::convolute(std::vector<std::vector<double>> Kernel, bool normal){
    bitmap_image output=normal?this->image: this->smoothed_image;

    int padding_size=(int)Kernel.size()/2;
    //Gradient X=Image Matrix*KernelX and Gradient Y=Image Matrix*Kernel
    for (int i=padding_size; i<this->width-padding_size; i++){
        for (int j=padding_size; j<this->height-padding_size; j++){
            double Convolute_red=0, Convolute_blue=0, Convolute_green=0;
            for (int k=0; k<Kernel.size(); k++){
                for (int l=0; l<Kernel.size(); l++){
                    if (normal){    //For Colored Filter e.g mean Filter
                        Convolute_red+= Kernel[k][l] * image.get_pixel(i+k-padding_size, j+l-padding_size).red;
                        Convolute_blue+= Kernel[k][l] * image.get_pixel(i+k-padding_size, j+l-padding_size).blue;
                        Convolute_green+= Kernel[k][l] * image.get_pixel(i+k-padding_size, j+l-padding_size).green;
                    }
                    else{   //For Intensity Filters e.g gaussian and sobel filter
                        Convolute_red+= Kernel[k][l] * image.get_pixel(i+k-padding_size, j+l-padding_size).red;
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

//**********************************Connected Component Analysis********************************************************
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

//********************************** Morphology ********************************************************
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

//Erosion Operation
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

void MainWindow::Opening(){
    Erosion(); Dilate();
}

void MainWindow::Closing(){
    Dilate(); Erosion();
}

//Boundary Extraction
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

//********************************** Representation ********************************************************
// Draw Edge around each object
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

//Draw Bound boxes around each objects
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

//********************************** Feature Extraction using RSI-Invariant *******************************************
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

//********************************** Training ********************************************************
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

//********************************** Testing ********************************************************
//Euclidean distance function
double MainWindow::Euclidean(std::vector<double> v1, std::vector<double>v2){
    double result=0;
    for(int i=0; i<7; i++){
        result+=pow(v1[i]-v2[i], 2);
    }
    return sqrt(result);
}

// Testing
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
    text+="...................\n";
    for(int set=0; set<object_set.size(); set++){   //for each class of objects in test image
        int count=0;
        bitmap_image object_class=image;
        for(int c=1; c<=no_of_objects; c++){       //for each objects in test image
            if(objects[c].name!=object_set[set]){   //if the object is not in class(i.e object name!=class name) turn its pixel black
                for(int i=objects[c].min_width; i<=objects[c].max_width; i++){
                    for(int j=objects[c].min_height; j<=objects[c].max_height; j++){
                        object_class.set_pixel(i, j, image.get_pixel(0, 0)); //make the pixel of non class objects the pixel of background
                    }
                }
            }
            else{count++;}
        }
        text+="Class "+object_set[set]+": "+std::to_string(count)+" Objects\n";
        object_class.save_image("./class/"+object_set[set]+".bmp");

        //Displaying the Class and its objects on screen
        display_Image("./class/"+object_set[set]+".bmp", "Class: "+object_set[set]);
    }

    this->MsgBox.setText(QString::fromUtf8(text.c_str()));
    this->MsgBox.exec();
}


//********************************** Edge, Line and Circle Detection ********************************************************
//Smoothening- Gaussian Filter
void MainWindow::Smoothening_Gaussian_Filter(){
    this->smoothed_image=this->image_intensity;
    this->smoothed_image=convolute(Gaussian_Kernel, false);
}

//Gx- Sobel Filter
bitmap_image MainWindow::Calc_Horizontal_Gradient(){
    Smoothening_Gaussian_Filter();
    return convolute(Gx_Kernel, false);
}

//Gy- Sobel Filter
bitmap_image MainWindow::Calc_Vertical_Gradient(){
    Smoothening_Gaussian_Filter();
    return convolute(Gy_Kernel, false);
}

//Calculating Gradient and Orientation- Sobel Filter
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
            G=G>255? 255: G;
            rgb_t Gradient={G, G, G};
            this->gradient.set_pixel(i, j, Gradient);


            double angle=G_x==0? 90: atan(G_y/G_x)*180/M_PI;
            rgb_t theta={angle, angle, angle};
            this->direction.set_pixel(i, j, theta);
        }
    }

    for(int i=0; i<this->width; i++){
        this->gradient.set_pixel(i, 0, black);
        this->gradient.set_pixel(i, this->height-1, black);
    }

    for(int j=0; j<this->height; j++){
        this->gradient.set_pixel(0, j, black);
        this->gradient.set_pixel(this->width-1, j, black);
    }
}

//Non-Maximum Suppression
void MainWindow::Non_maxima_Suppression(){
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            double theta=this->direction.get_pixel(i, j).red;
            double G=this->gradient.get_pixel(i, j).red;
            double P1, P2;
            if ((theta>-22.5 && theta<=22.5) || (theta>157.5 && theta<= -157.5)){    //rule 0-degree- Horizontal
                P1=this->gradient.get_pixel(i, j+1).red;
                P2=this->gradient.get_pixel(i, j-1).red;
            }
            else if((theta>-157.5 && theta<=-112.5) || (theta>22.5 && theta<= 67.5)){   //rule 45-degree
                P1=this->gradient.get_pixel(i+1, j-1).red;
                P2=this->gradient.get_pixel(i-1, j+1).red;
            }
            else if((theta>-112.5 && theta<=-67.5) || (theta>67.5 && theta<= 112.5)){   //rule 90-degree- Vertical
                P1=this->gradient.get_pixel(i+1, j).red;
                P2=this->gradient.get_pixel(i-1, j).red;
            }
            else if((theta>-67.5 && theta<=-22.5) || (theta>112.5 && theta<=157.5)){   //rule 135-degree
                P1=this->gradient.get_pixel(i+1, j+1).red;
                P2=this->gradient.get_pixel(i-1, j-1).red;
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
            double G=this->gradient.get_pixel(i, j).red;
            if(G<T_low){    //below T_low
                this->image_edge_dect.set_pixel(i, j, black); //remove it since its not an edge
            }
            else if(G>=T_low && G<=T_high){ //Inbetween Thresholds
                //If all neighbors are less than T_high then its not an edge
                 //so we can remove it
                bool isedge=false;
                for (int k=-1; k<=1; k++){
                    for (int l=-1; l<=1; l++){
                        if(image_edge_dect.get_pixel(i+k, j+l).red>T_high){
                            this->image_edge_dect.set_pixel(i, j, white);
                            isedge=true;
                            break;
                        }
                    }
                    if(isedge) break;
                }
                if(!isedge) this->image_edge_dect.set_pixel(i, j, black);   //not an edge
            }
            else{   //above T_high
                this->image_edge_dect.set_pixel(i, j, white);   //is edge
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

//Function for drawing lines
void MainWindow::Draw_Line(int d, int theta){
    double Deg2Rad=M_PI/180;
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            if(d==int(i*cos(theta*Deg2Rad)+j*sin(theta*Deg2Rad))){
                Line_detect.set_pixel(i, j, Color[0]);
            }
        }
    }
}

//Hough Transform Line Segment
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
                     int d=i*cos(theta*Deg2Rad)+j*sin(theta*Deg2Rad);
                     Hough_Counter[d][theta]++;
                }
            }
        }
    }

    QString nstring = QInputDialog::getText(0,"Input Number of lines","Input");
    int n_lines=nstring.toInt();
    //ordered array of lines- increasing from left to right
    std::pair<int, int> Lines[n_lines];
    for(int i=0; i<n_lines; i++){
        Lines[i]=std::make_pair(0, 0);
    }

    //Finding n-lines with maximum H(d, theta)
    for(int i=0; i<d_max; i++){
        for(int j=0; j<theta_max; j++){
            if(n_lines==1){
                if(Hough_Counter[i][j]>Hough_Counter[Lines[0].first][Lines[0].second]){
                    Lines[0]=std::make_pair(i, j);
                }
            }
            else{
                int pos=-1, c=0;
                for(c=0; c<n_lines-1; c++){
                    if(c==0 && Hough_Counter[i][j]<=Hough_Counter[Lines[c].first][Lines[c].second]){// less than first element
                            break;
                    }
                    if((Hough_Counter[i][j]>Hough_Counter[Lines[c].first][Lines[c].second])&&
                            (Hough_Counter[i][j]<Hough_Counter[Lines[c+1].first][Lines[c+1].second])){ //element in between two elements(left and right elements)
                        pos=c;
                        break;
                    }
                    if(c+1==n_lines-1){ //greater than last element
                        pos=n_lines-1;
                        break;
                    }
                }
                if(pos!=-1){ //element added to top n-lines
                    if(pos!=0){
                        //shifting right elements to left
                        for(c=0; c<pos; c++){
                            Lines[c]=Lines[c+1];
                        }
                    }
                    Lines[pos]=std::make_pair(i, j);
                }
            }
        }
    }

    for(int i=0; i<n_lines; i++){
        //std::cout<<"d="<<Lines[i].first<<"theta="<<Lines[i].second<<std::endl;
        Draw_Line(Lines[i].first, Lines[i].second);
    }
    return Lines[n_lines-1];
}

//Function for drawing circle
void MainWindow::Draw_Circle(int a, int b, int r){
    double Deg2Rad=M_PI/180;
    if(r==0) return;
    for (int theta=0; theta<360; theta++){
        int i=int(a+(r*cos(theta*Deg2Rad)));
        int j=int(b-(r*sin(theta*Deg2Rad)));
        if((i>=1 && i<this->width-1) && (j>=1 && j<this->height-1)) Circle_detect.set_pixel(i, j, Color[1]);
    }
}

//Hough Transform - Circle Detection
std::tuple<int, int, int> MainWindow::Hough_Transform_Circle_Detector(){
    QString nstring = QInputDialog::getText(0,"Input minimum radius","Input");
    int r_min=nstring.toInt();
    nstring.clear();
    nstring=QInputDialog::getText(0,"Input maximum radius","Input");
    int r_max=nstring.toInt();

    int a_max=this->width;
    int b_max=this->height;

    //Initializing Counter
    std::vector<std::vector<std::vector<int>>> Hough_Counter;
    for(int i=0; i<a_max; i++){
        Hough_Counter.push_back(std::vector<std::vector<int>>());
        for(int j=0; j<b_max; j++){
            Hough_Counter[i].push_back(std::vector<int>());
            for(int k=r_min; k<=r_max; k++){
                Hough_Counter[i][j].push_back(0);
            }
        }
    }

    //Finding H(a, b, radius) for each edge points
    int theta_max=360, accuracy=1;
    double Deg2Rad=M_PI/180;
    for (int i=1; i<this->width-1; i++){
        for (int j=1; j<this->height-1; j++){
            if(image_edge_dect.get_pixel(i, j).red!=0){   //is edge
                for(int r=0; r<=r_max-r_min; r++){
                    for(int theta=0; theta<theta_max; theta+=accuracy){
                        int a=i-((r+(r_max-r_min))*cos(theta*Deg2Rad));
                        int b=j+((r+(r_max-r_min))*sin(theta*Deg2Rad));
                        if((a>=1 && a<this->width-1) && (b>=1 && b<this->height-1)) Hough_Counter[a][b][r]++;
                    }
                }
            }
        }
    }


    nstring.clear();
    nstring=QInputDialog::getText(0,"Input number of circles","Input");
    int n_circles=nstring.toInt();
    //ordered array of lines- increasing from left to right
    std::tuple<int, int, int> Circles[n_circles];
    for(int i=0; i<n_circles; i++){
        Circles[i]=std::make_tuple(0, 0, 0);
    }


    //Finding n-circles maximum H(a, b, r)
    for(int i=0; i<a_max; i++){
        for(int j=0; j<b_max; j++){
            for(int r=0; r<=r_max-r_min; r++){
                if(n_circles==1){
                    if(Hough_Counter[i][j][r]>Hough_Counter[std::get<0>(Circles[0])][std::get<1>(Circles[0])][std::get<2>(Circles[0])]){
                        Circles[0]=std::make_tuple(i, j, r);
                    }
                }
                else{
                    int pos=-1, c=0;
                    for(c=0; c<n_circles-1; c++){
                        if(c==0 && Hough_Counter[i][j][r]<=Hough_Counter[std::get<0>(Circles[c])][std::get<1>(Circles[c])][std::get<2>(Circles[c])]){// less than first element
                                break;
                        }
                        if((Hough_Counter[i][j][r]>Hough_Counter[std::get<0>(Circles[c])][std::get<1>(Circles[c])][std::get<2>(Circles[c])])&&
                                (Hough_Counter[i][j][r]<Hough_Counter[std::get<0>(Circles[c+1])][std::get<1>(Circles[c+1])][std::get<2>(Circles[c+1])])){ //element in between two elements(left and right elements)
                            pos=c;
                            break;
                        }
                        if(c+1==n_circles-1){ //greater than last element
                            pos=n_circles-1;
                            break;
                        }
                    }
                    if(pos!=-1){ //element added to top n-lines
                        if(pos!=0){
                            //shifting right elements to left
                            for(c=0; c<pos; c++){
                                Circles[c]=Circles[c+1];
                            }
                        }
                        Circles[pos]=std::make_tuple(i, j, r);
                    }
                }
            }
        }
    }

    int a, b, r;
    for(int i=0; i<n_circles; i++){
        a=std::get<0>(Circles[i]);
        b=std::get<1>(Circles[i]);
        r=std::get<2>(Circles[i])+(r_max-r_min);
        //std::cout<<"a="<<a<<" b="<<b<<" r="<<r<<std::endl;
        Draw_Circle(a, b, r);
    }
    return std::make_tuple(a, b, r);
}
