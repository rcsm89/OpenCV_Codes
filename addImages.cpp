#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <string.h>

using namespace cv;
using namespace std;

Mat img,img_filtro;
char endereco[1024];
const int c_max = 100;
int c;

int main(int argc, char** argv)
{

	for(int i = 1;i<=1155;i++){
		sprintf(endereco, "/home/ricardo/Área\ de\ Trabalho/DDSM/RealceLog/%d.jpg", i);
		printf("Imagem %d\n", i);
		img = imread(endereco,CV_LOAD_IMAGE_GRAYSCALE);
			if(!img.data){
				printf("Erro - imread\n");
				return -111;
			}
			//namedWindow("imagem original",CV_WINDOW_AUTOSIZE-1);
			//namedWindow("realce logaritmo",CV_WINDOW_AUTOSIZE-1);
			//imshow("imagem original", img);

			if(img.data == NULL){
				printf("Erro - Imread\n");
				//getchar();
				return -1;
			}

			Mat img_smooth;
//			medianBlur(img,img_smooth5, 5);
			img.convertTo(img,CV_8U);
			blur(img,img_smooth, Size(7,7));
			sprintf(endereco,"/home/ricardo/Área\ de\ Trabalho/DDSM/Smooth/%d.jpg", i);

//			namedWindow("teste", WINDOW_AUTOSIZE-1);

//			imshow("teste", img_smooth);
			imwrite(endereco ,img_smooth);
//			waitKey(0);

	}
	waitKey(0);

	return 0;
}
