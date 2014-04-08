#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;


Mat src, src_gray, dst;
vector<Mat> ch;


int main( int argc, char** argv )
{



	IplImage *img;
	//src = imread("C:\Users\pié-pié\Documents\Visual Studio 2010\Projects\test\test\2014-02-15\test.jpg",0);
	//Mat src_resized;

	//resize(src, src_resized, Size(1028, 1227), 0, 0, INTER_AREA);
	
	//split(src, ch);
	img = cvLoadImage("2014-04-17/03_VIS_sv_090-0-0-0.png", CV_LOAD_IMAGE_GRAYSCALE);
/*	IplImage *destination = cvCreateImage ( cvSize((int)((img->width)/2) , (int)((img->height)/2 )), img->depth, img->nChannels );
	cvResize(img, destination)*/;
//	cvtColor( src, src_gray, CV_RGB2GRAY )-;
	namedWindow( "fufu", 0);
	src = Mat(img);
	threshold(src, dst, 150.0, 230.0, 1);

	//cvShowImage( "fufu", img);
	imshow("fufu", dst);

	cvWaitKey(0);
	
	return 0;
}