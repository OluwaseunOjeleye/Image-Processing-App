

# Image Processing

This project consists of image processing algorithms and they are all implemented in C++.

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
