OpenCV_Codes
============
Feature Extraction and Haralick Descriptors
This program read a .txt file and open all of images from this file and extract features.
Implementations, Functions, Codes.

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace cv;
// matrix of coocurrence
float** CoOccurrence_Matrix_Calculation(Mat img, int distancia, int angulo) {
	float **cooccurance;
	int i, j;
	
	// allocate matrix of coocurrence
	cooccurance = (float**)malloc(256*sizeof(float*));
	if(cooccurance != NULL){
		for(i = 0;i < 256;){
			cooccurance[i] = (float*)malloc(sizeof(float)*256);
			if(cooccurance[i] != NULL){
				i++;
			}
		}
	}
	// inicialize matrix with zero
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			cooccurance[i][j] = 0;
		}
	}
	// Extract six features of each cooc-matrix
	for (i=distancia; i< (img.rows)-distancia; i++){
		for (j=1; j<img.cols; j++) {
			if(angulo == 0){
				cooccurance[img.at<uchar>(i,j)][img.at<uchar>(i,j+distancia)]+=1.0;
				cooccurance[img.at<uchar>(i,j+distancia)][img.at<uchar>(i,j)]+=1.0;
			}else
				if(angulo == 45){
					cooccurance[img.at<uchar>(i,j)][img.at<uchar>(i-distancia,j+distancia)]+=1.0;
					cooccurance[img.at<uchar>(i-distancia,j+distancia)][img.at<uchar>(i,j)]+=1.0;
				}else
					if(angulo == 90){
						cooccurance[img.at<uchar>(i,j)][img.at<uchar>(i-distancia,j)]+=1.0;
						cooccurance[img.at<uchar>(i-distancia,j)][img.at<uchar>(i,j)]+=1.0;
					}else
						if(angulo == 135){
							cooccurance[img.at<uchar>(i,j)][img.at<uchar>(i-distancia,j-distancia)]+=1.0;
							cooccurance[img.at<uchar>(i-distancia,j-distancia)][img.at<uchar>(i,j)]+=1.0;
						}
		}
	}
	return cooccurance;
}
// normalize all values of cooc-matrix
void normaliza(float** matrix){
	int i, j;
	float soma = 0;
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			soma += matrix[i][j];
		}
	}
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			matrix[i][j] /= soma ;
		}
	}
}

float mod(float num){
	if(num < 0)
	{
		num = (-1.0*num);
	}
	return num;
}
//formules of descriptors
float* medidas(float **mc){
	float* meds;
	int i, j;
	float sam=0, ent=0, ene=0, hom=0, dis=0, con=0;

	meds = (float*)malloc(6*sizeof(float));
	if(meds != NULL){
		//SECOND ANGULAR MOMENT
		for(i=0;i<256;i++){
			for(j=0;j<256;j++){
				sam += (mc[i][j]*mc[i][j]);
			}
		}
		meds[0] = sam;


		//ENTROPY
		for(i=0;i<256;i++){
			for(j=0;j<256;j++){
				if(mc[i][j] > 0)
					ent += (mc[i][j]*(log(mc[i][j])));
			}
		}
		meds[1] = (-1.0)*ent;


		//ENERGY
		for(i=0;i<256;i++){
			for(j=0;j<256;j++){
				ene += mc[i][j]*mc[i][j];
			}
		}
		meds[2] = sqrtf(ene);


		//HOMOGENEITY
		for(i=0;i<256;i++){
			for(j=0;j<256;j++){
				hom += mc[i][j]/(1+((i-j)*(i-j)));
			}
		}
		meds[3] = hom;


		//DISSIMILARITY
		for(i=0;i<256;i++){
			for(j=0;j<256;j++){
				dis += mc[i][j]*mod(i-j);
			}
		}
		meds[4] = dis;


		//CONTRAST
		for(i=0;i<256;i++){
			for(j=0;j<256;j++){
				con += mc[i][j]*(i-j)*(i-j);
			}
		}
		meds[5] = con;

	}

	return meds;
}

void printMedidas(float* med, int ka){
	int i;
	for(i=0;i<6;ka++,i++){
		printf("%d: %f\t", ka , med[i]);
	}
}

int main(int argc, char** argv){
	Mat /*img_original,*/ img_result;
	Mat img_log;//, img_equalize;
	double minVal,maxVal;
	float** mcooc;
	float *medHal;
	char end[1024];

	while(scanf("%s", end) != EOF){// read image local from file... while not end of file.
		if(strstr(end, "malignos")){
			printf("+1 ");
		}else if(strstr( end, "normais" )){
			printf("-0 ");
		}else if(strstr(end, "benignos")){
			printf("-1 ");
		}

		//img_original = imread(end, 0);

			//namedWindow("IMAGEM", WINDOW_AUTOSIZE-1);
			//imshow("IMAGEM", img_original);

			img_log = imread(end, CV_LOAD_IMAGE_GRAYSCALE);
			if(!img_log.data){
				cout << "Error to load image" << endl;
				return -1;
			}
			img_log.convertTo(img_log, CV_32F);
			img_log = img_log*(0.003921);
			img_log = img_log + 1;

			log(img_log, img_log);
			minMaxLoc(img_log,&minVal,&maxVal);

			//img_result = img_log - ((float)minVal);
			img_result = img_log;
			img_log = 255*(img_result/(float)maxVal);
			img_log.convertTo(img_log, CV_8U);

			//equalizeHist(img_log, img_equalize);
			//namedWindow("Realce Logaritmico", WINDOW_AUTOSIZE-1);
			//imshow("Realce Logaritmico",img_log);
			//namedWindow("Realce Logaritmico Equalizado", WINDOW_AUTOSIZE-1);
			//imshow("Realce Logaritmico Equalizado",img_equalize);
			//waitKey(0);
			int distancia = 1;
			int angulo = 0;
			//printf("DISTANCIA: ");
			//scanf("%d", &distancia);
			//printf("MEDIDAS PARA DISTANCIA %d", distancia);
				int k;
				while(angulo <= 135){
					mcooc = CoOccurrence_Matrix_Calculation(img_log, distancia, angulo);
					normaliza(mcooc);
					medHal = medidas(mcooc);
					if(angulo == 0){
						k=1;
						printMedidas(medHal, k);
					}else if(angulo == 45){
						k=7;
						printMedidas(medHal, k);
					}else if(angulo == 90){
						k=13;
						printMedidas(medHal, k);
					}else if(angulo == 135){
						k=19;
						printMedidas(medHal, k);
						printf("\n");
					}
					
				//	printf("\n");
					//printf("\nMedidas para o angulo %dº\n", angulo);
					angulo += 45;
				}
	}
free(medHal);
return 0;
}
