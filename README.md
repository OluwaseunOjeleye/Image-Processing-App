# Image Processing

This project consist of two Image Processing projects implemented in C++. These projects are:  

```
1. Object Detection and Classification based on Geometric Invariant Moment.  
2. Line and Circle detection based on Hough Transform.
```

## Project 1: Object Detection and Classification based on Geometric Invariant Moment

This project consist of the implementation of objects detection and classification program which detects and classifies objects (such as rice, beans, etc.) based on their geometric shape. The diagram below shows a typical image processing system and the sub-processes involved in this project:
![Alt text](../master/Test_images/documentation_Images/Fig1.png?raw=true "Title")

## Line and Circle detection based on Hough Transform

ddd

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites
Linux Machine with gcc compiler.

### Downloading
Cloning The GitHub Repository

```
git clone 
```
### How to use the program
Once you've cloned the repository, open the main.cpp file and modify a image using the operators shown below.


```
Reading image: "monalisa.jpg">>Img1 
Writing image: "output"<<Img1

Edge Detection:
Sobel Filter: Img1.SobelFilter()

Operators:
AND: Img3=Img1*Img2
OR: Img3=Img1+Img2
NOT: Img3=!Img1
EQUIVALENCE: Img3=Img1
```

### Compiling the program
Run this in the folder from the command-line:

```
make
```

## Authors
* **Jamiu Oluwaseun Ojeleye** 
