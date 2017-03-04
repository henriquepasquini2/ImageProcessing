/*HENRIQUE PASQUINI SANTOS  Nº USP 8532252*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace cv;
using namespace std;

void PrewittFilter(Mat *I1, Mat *I2)
{
    int N = 3;
    int border = (N-1) / 2;
    Mat Px(N,N,CV_32F);
    Mat Py(N,N,CV_32F);

    /*Matriz Prewitt Horizontal*/
    Px.at<int>(0,0) = 1; Px.at<int>(0,1) = 0; Px.at<int>(0,2) = -1;
    Px.at<int>(1,0) = 1; Px.at<int>(1,1) = 0; Px.at<int>(1,2) = -1;
    Px.at<int>(2,0) = 1; Px.at<int>(2,1) = 0; Px.at<int>(2,2) = -1;

    /*Matriz Prewitt Vertical*/
    Py.at<int>(0,0) =  1; Py.at<int>(0,1) =  1; Py.at<int>(0,2) =   1;
    Py.at<int>(1,0) =  0; Py.at<int>(1,1) =  0; Py.at<int>(1,2) =   0;
    Py.at<int>(2,0) = -1; Py.at<int>(2,1) = -1; Py.at<int>(2,2) =  -1;

    /*Matriz auxiliar com borda*/
    Mat aux(I1->size(),CV_8U,1);
    copyMakeBorder(*I1, aux, border, border, border, border, BORDER_REPLICATE);

    int soma, sumX, sumY;
    float PT;
    for(int i = 0; i < I1->size().height; i++) {
        for(int j = 0; j < I1->size().width; j++) {
            sumX = 0; sumY = 0; soma = 0;
            for(int x = -border ; x <= border ; x++) {
                for(int y = -border ; y <= border ; y++) {
                    sumX += Px.at<int>(x+border, y+border) * (int)aux.at<uchar>(i+x+border, j+y+border);
                }
            }
            for(int x = -border ; x <= border ; x++) {
                for(int y = -border ; y <= border ; y++) {
                    sumY += Py.at<int>(x+border, y+border) * (int)aux.at<uchar>(i+x+border, j+y+border);
                }
            }
            PT = sqrt(((double)(sumX*sumX)) + ((double)(sumY*sumY)));
            if(PT > 255) PT = 255;
            if (PT < 0) PT = 0;
            //PT = 255 - PT;
            I2->at<uchar>(i,j) = PT;
        }
    }
}
void SobelFilter(Mat *I1, Mat *I2)
{
    int N = 3;
    int border = (N-1) / 2;
    Mat Sx(N,N,CV_32F);
    Mat Sy(N,N,CV_32F);

    /*Matriz Sobel Horizontal*/
    Sx.at<int>(0,0) = 1; Sx.at<int>(0,1) = 0; Sx.at<int>(0,2) = -1;
    Sx.at<int>(1,0) = 2; Sx.at<int>(1,1) = 0; Sx.at<int>(1,2) = -2;
    Sx.at<int>(2,0) = 1; Sx.at<int>(2,1) = 0; Sx.at<int>(2,2) = -1;

    /*Matriz Sobel Vertical*/
    Sy.at<int>(0,0) =  1; Sy.at<int>(0,1) =  2; Sy.at<int>(0,2) =   1;
    Sy.at<int>(1,0) =  0; Sy.at<int>(1,1) =  0; Sy.at<int>(1,2) =   0;
    Sy.at<int>(2,0) = -1; Sy.at<int>(2,1) = -2; Sy.at<int>(2,2) =  -1;

    /*Matriz auxiliar com borda*/
    Mat aux(I1->size(),CV_8U,1);
    copyMakeBorder(*I1, aux, border, border, border, border, BORDER_REPLICATE);

    int soma, sumX, sumY;
    float ST;
    for(int i = 0; i < I1->size().height; i++) {
        for(int j = 0; j < I1->size().width; j++) {
            sumX = 0; sumY = 0; soma = 0;
            for(int x = -border ; x <= border ; x++) {
                for(int y = -border ; y <= border ; y++) {
                    sumX += Sx.at<int>(x+border, y+border) * (int)aux.at<uchar>(i+x+border, j+y+border);
                }
            }
            for(int x = -border ; x <= border ; x++) {
                for(int y = -border ; y <= border ; y++) {
                    sumY += Sy.at<int>(x+border, y+border) * (int)aux.at<uchar>(i+x+border, j+y+border);
                }
            }
            ST = sqrt(((double)(sumX*sumX)) + ((double)(sumY*sumY)));
            if(ST > 255) ST = 255;
            if (ST < 0) ST = 0;

            I2->at<uchar>(i,j) = ST;
        }
    }
}
int main(int argc, char *argv[])
{
    if(argc < 3) {
        cout << "./filt <param1> <param2> <param3>" << endl;
        cout << "<param1> = Imagem de entrada" << endl;
        cout << "<param2> = Imagem de saida apos Sobel filter" << endl;
        cout << "<param3> = Imagem de saida apos Prewitt filter" << endl;
        return -1;
    }
    const char *fileIn = argv[1];
    const char *fileOut1 = argv[2];
	const char *fileOut2 = argv[3];

    Mat I1 = imread(fileIn, CV_LOAD_IMAGE_GRAYSCALE);
    if(I1.empty()){
       cout << "Nao foi possivel abrir a imagem" << endl;
       return -1;
    }
    Mat I2(I1.size(),CV_8U,1);
    Mat I3(I1.size(),CV_8U,1);

    SobelFilter(&I1, &I2);
    PrewittFilter(&I1, &I3);

    /*Imagem de entrada*/
    namedWindow( "Original", CV_WINDOW_AUTOSIZE );
    imshow("Original", I1);
    /*Saida*/
    namedWindow("Sobel filter", CV_WINDOW_AUTOSIZE );
    imshow("Sobel filter", I2);

    namedWindow("Prewitt filter", CV_WINDOW_AUTOSIZE );
    imshow("Prewitt filter", I3);

    imwrite(fileOut1, I2);
    imwrite(fileOut2, I3);

    waitKey(0);
    return 0;
}
