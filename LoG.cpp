/*HENRIQUE PASQUINI SANTOS  Nº USP 8532252*/

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace cv;
using namespace std;

Mat *LoG(int P, float dp) {
	Mat *LoG = new Mat(P, P, CV_32F);

	int border = (P - 1) / 2;

	double p1 = (-1) * (1 / M_PI * dp * dp * dp * dp);
	double p2;
	double p3;

	int x, y;
    float soma;
	for(int i = 0; i < P; i++) {
		x = i - border;
		for(int j = 0; j < P; j++) {
			y = j - border;

			p2 = 1 - ((x*x + y*y) / (2*dp*dp));
			p3 = exp((-1) * (x*x + y*y) / (2*dp*dp));

			LoG->at<float>(i, j) = p1 * p2 * p3;
            printf("%f ", LoG->at<float>(i, j));
		}
        printf("\n");
    }
	return LoG;
}

void conv(Mat *I1, Mat* I2, Mat *w) {
    int border = w->size().height / 2;

    Mat aux(I1->size(),CV_8U,1);
    copyMakeBorder(*I1, aux, border, border, border, border, BORDER_REPLICATE);

    float soma;

    for(int i = 0; i < I1->size().height; i++) {
        for(int j = 0; j < I1->size().width; j++) {
            soma = 0;
            for(int x = -border ; x<= border ; x++) {
                for(int y= -border ; y<= border ; y++) {
                    soma += w->at<float>(border + x, border + y) * ((float) aux.at<uchar>(i + x + border, j + y + border));
                }
            }
            I2->at<float>(i, j) = soma;
        }
    }
}

void zerocrossing(Mat *I1, Mat* I2) {

    Mat aux = I1->clone();
    copyMakeBorder(*I1, aux, 1, 1, 1, 1, BORDER_REPLICATE);

    float soma;

    for(int i = 1; i < I1->size().height; i++) {
        for(int j = 1; j < I1->size().width; j++) {
            soma = 255.0;
            //left-right
            if (aux.at<float>(i, j - 1) * aux.at<float>(i, j + 1) < 0)
                soma = 0;
            //top-down
            else if (aux.at<float>(i - 1, j) * aux.at<float>(i + 1, j) < 0)
                soma = 0;
            //upperleft-downright
            else if (aux.at<float>(i - 1, j - 1) * aux.at<float>(i + 1, j + 1) < 0)
                soma = 0;
            //upperright-downleft
            else if (aux.at<float>(i - 1, j + 1) * aux.at<float>(i + 1, j - 1) < 0)
                soma = 0;

            I2->at<float>(i, j) = soma;
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        cout << "./filt <param1> <param2> <param3>" << endl;
        cout << "<param1> = Imagem de entrada" << endl;
        cout << "<param2> = Imagem de saida apos convolucao" << endl;
        cout << "<param3> = Imagem de saida apos zero crossing" << endl;
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

    int P = 7; /*Kernel size*/
    float sigma = 1.4;
    Mat I2(I1.size(), CV_32F, 1.0);
    Mat I3(I1.size(), CV_32F, 1.0);
    Mat *log = LoG(P, sigma);

    conv(&I1, &I2, log);
//    for(int i = 0; i < I2.size().height; i++) {
//        for(int j = 0; j < I2.size().width; j++) {
//            printf("%d ", I2.at<uchar>(i,j));
//        }
//        printf("\n");
//    }
    zerocrossing(&I2, &I3);

    I2.convertTo(I2, CV_8U);

    namedWindow( "Original", CV_WINDOW_AUTOSIZE );
    imshow("Original", I1);
    namedWindow( "Convolution", CV_WINDOW_AUTOSIZE );
    imshow("Convolution", I2);
    namedWindow( "Zero-crossings", CV_WINDOW_AUTOSIZE );
    imshow("Zero-crossings", I3);

    imwrite(fileOut1, I2);
    imwrite(fileOut2, I3);

    waitKey(0);
    return 0;
}
