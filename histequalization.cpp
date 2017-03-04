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

void imhistogram(Mat I1, int histogram[])
{
    for(int i = 0; i < 256; i++) histogram[i] = 0;

    for (int x = 0; x < I1.size().height; x++)
        for (int y = 0; y < I1.size().width; y++)
            histogram[(int)I1.at<uchar>(x,y)]++;
}

void imprimehist(int histogram[], const char* nome)
{
    int hist[256];
    for(int i = 0; i < 256; i++) hist[i]=histogram[i];

    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w/256);

    Mat histI(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

    int max = hist[0];
    for(int i = 1; i < 256; i++){
        if(max < hist[i]){
            max = hist[i];
        }
    }

    for(int i = 0; i < 256; i++)
        hist[i] = ((double)hist[i]/max)*histI.size().width;


    for(int i = 0; i < 256; i++)
        line(histI, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h - hist[i]), Scalar(0,0,0), 1, 8, 0);


    namedWindow(nome, CV_WINDOW_AUTOSIZE);
    imshow(nome, histI);
}

void equalize(Mat *I1, Mat *I2, int histogram[], int eq_histogram[])
{
    int size = I1->size().height * I1->size().width;
    float alpha = 255.0/size;

    /*Probabilidade da intensidade*/
    float Prob[256];
    for(int i = 0; i < 256; i++)
        Prob[i] = (double)histogram[i] / size;

    /*Cumulative frequency histogram*/
    int cumhistogram[256];
    cumhistogram[0] = histogram[0];
    for(int i = 1; i < 256; i++)
        cumhistogram[i] = histogram[i] + cumhistogram[i-1];

    /*Ajuste da escala do histograma*/
    int esc[256];
    for(int i = 0; i < 256; i++)
        esc[i] = cvRound((double)cumhistogram[i] * alpha);

    /*Histograma equalizado*/
    float ProbEsc[256];
    for(int i = 0; i < 256; i++)
        ProbEsc[i] = 0;

    for(int i = 0; i < 256; i++)
        ProbEsc[esc[i]] += Prob[i];

    for(int i = 0; i < 256; i++)
        eq_histogram[i] = cvRound(ProbEsc[i]*255);

    /*Imagem equalizada*/
    for(int x = 0; x < I1->size().height; x++)
        for(int y = 0; y< I1->size().width; y++)
            I2->at<uchar>(x,y) = saturate_cast<uchar>(esc[I1->at<uchar>(x,y)]);

}

int main()
{
    char imgname[50];
    cout << "Digite o nome da imagem: " << endl;
    scanf("%s", imgname);
    Mat I1 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
    if(I1.empty()){
       cout << "Nao foi possivel abrir a imagem" << endl;
       return -1;
    }

    int histogram[256];
    int eq_histogram[256];
    imhistogram(I1, histogram);

    Mat I2(I1.size(), CV_8U, 1);

    equalize(&I1, &I2, histogram, eq_histogram);

    imprimehist(histogram, "Histogram");
    imprimehist(eq_histogram, "Equalized Histogram");
    namedWindow("Original");
    imshow("Original", I1);
    namedWindow("Equalized Image");
    imshow("Equalized Image",I2);

    waitKey();
    return 0;
}
