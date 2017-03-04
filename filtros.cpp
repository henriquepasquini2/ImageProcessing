/*Henrique Pasquini Santos nºUSP 8532252*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
using namespace cv;
using namespace std;

void averagefilter(Mat *I1, Mat *I2, int N) {

    int border = (N-1) / 2;

    /*Matriz aux com borda*/
    Mat aux(I1->size(),CV_8U,1);
    copyMakeBorder(*I1, aux, border, border, border, border, BORDER_REPLICATE);

    int media;

    for(int i = 0; i < I1->size().height; i++) {
        for(int j = 0; j < I1->size().width; j++) {
            media = 0;

            for(int x = -border ; x <= border ; x++) {
                for(int y = -border ; y <= border ; y++) {
                    media += (aux.at<uchar>(i+x+border,j+y+border))/(N*N);
                }
            }
            media = media/N*N;
            if(media > 255) media = 255;
            I2->at<uchar>(i, j) = media;
        }
    }
}

void insertionSort(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

void medianfilter(Mat *I1, Mat *I2, int N) {

    int border = (N - 1) / 2;

    Mat aux(I1->size(),CV_8U,1);
    copyMakeBorder(*I1, aux, border, border, border, border, BORDER_REPLICATE);

    int mediana;

//    for(int i = 0; i < I1->size().height; i++) {
//        for(int j = 0; j < I1->size().width; j++) {
//            printf("%d ", I1->at<uchar>(i,j));
//        }
//        printf("\n");
//    }
//    printf("\n");
//    for(int i = 0; i < aux.size().height; i++) {
//        for(int j = 0; j < aux.size().width; j++) {
//            printf("%d ", aux.at<uchar>(i,j));
//        }
//        printf("\n");
//    }
    for(int i = 0; i < I1->size().height; i++) {
        for(int j = 0; j < I1->size().width; j++) {
            int k = 0;
            int v[N*N];
            for(int x = -border ; x <= border ; x++) {
                for(int y = -border ; y <= border ; y++) {
                    v[k] = aux.at<uchar>(i+x+border,j+y+border);
                    k++;
                }
            }
            insertionSort(v);
            mediana = v[((N*N)-1)/2];
            I2->at<uchar>(i, j) = mediana;
        }
    }
//    printf("\n");
//    for(int i = 0; i < I2->size().height; i++) {
//        for(int j = 0; j < I2->size().width; j++) {
//            printf("%d ", I2->at<uchar>(i,j));
//        }
//        printf("\n");
//    }
}

int main(int argc, char *argv[])
{
    char imgname[50];
    cout << "Digite o nome da imagem: " << endl;
    scanf("%s", imgname);
    Mat I1 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
    if(I1.empty()){
       cout << "Nao foi possivel abrir a imagem" << endl;
       return -1;
    }
    Mat I2(I1.size(), CV_8U, 1);
    Mat I3(I1.size(), CV_8U, 1);
    Mat I4(I1.size(), CV_8U, 1);

    /*Imagem de entrada*/
    namedWindow( "Original", CV_WINDOW_AUTOSIZE );
    imshow("Original", I1);

    /*Imagens com filtro media*/
    /*Mask size = 3*/
    averagefilter(&I1, &I2, 3);
    /*Mask size = 5*/
    averagefilter(&I1, &I3, 5);
    /*Mask size = 9*/
    averagefilter(&I1, &I4, 9);

    namedWindow( "Average filter 3x3", CV_WINDOW_AUTOSIZE );
    imshow("Average filter 3x3", I2);
    namedWindow( "Average filter 5x5", CV_WINDOW_AUTOSIZE );
    imshow("Average filter 5x5", I3);
    namedWindow( "Average filter 9x9", CV_WINDOW_AUTOSIZE );
    imshow("Average filter 9x9", I4);

    Mat I5(I1.size(), CV_8U, 1);
    Mat I6(I1.size(), CV_8U, 1);
    Mat I7(I1.size(), CV_8U, 1);

    /*Imagens com filtro mediana*/
    /*Mask size = 3*/
    medianfilter(&I1, &I5, 3);
    /*Mask size = 5*/
    medianfilter(&I1, &I6, 5);
    /*Mask size = 9*/
    medianfilter(&I1, &I7, 9);

    namedWindow( "Median filter 3x3", CV_WINDOW_AUTOSIZE );
    imshow("Median filter 3x3", I5);
    namedWindow( "Median filter 5x5", CV_WINDOW_AUTOSIZE );
    imshow("Median filter 5x5", I6);
    namedWindow( "Median filter 9x9", CV_WINDOW_AUTOSIZE );
    imshow("Median filter 9x9", I7);

    waitKey(0);
    return 0;
}
