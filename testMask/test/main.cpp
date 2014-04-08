#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;


Mat src, src_gray, dst, detected_edges;



int main( int argc, char** argv )
{

	IplImage *img;

	img = cvLoadImage("2014-04-07/03_VIS_sv_090-0-0-0.png", -1);

	namedWindow( "fufu", 0);
	src = Mat(img);

	//blue color set to 0
	for(int i = 0 ; i < src.rows ; i++)
		for(int j = 0 ; j < src.cols ; j++)
		{
			int a = src.data[src.step[0]*i + src.step[1]* j + 0];
			int b = src.data[src.step[0]*i + src.step[1]* j + 1];
			int c = src.data[src.step[0]*i + src.step[1]* j + 2];
			if(( (a > 50 && b > 20 && c > 10) && (a < 200 && b < 105 && c < 100)) || ((a+b+c)/3 > 120) || (a > 100) )
			{
				src.data[src.step[0]*i + src.step[1]* j + 0] = 0;
				src.data[src.step[0]*i + src.step[1]* j + 1] = 0;
				src.data[src.step[0]*i + src.step[1]* j + 2] = 0;
			}

		}

	cvtColor(src, dst, CV_BGR2GRAY);

	threshold(dst, dst, 60, 255, THRESH_OTSU);

	imshow("fufu", dst);

	cvWaitKey(0);
	
	return 0;
}


//int main( int argc, char** argv )
//{
//
//	IplImage *img;
//
//	img = cvLoadImage("2014-04-07/03_VIS_sv_090-0-0-0.png", -1);
//
//	namedWindow( "fufu", 0);
//	src = Mat(img);
//
//	//blue color set to 0
//	for(int i = 0 ; i < src.rows ; i++)
//		for(int j = 0 ; j < src.cols ; j++)
//		{
//			int a = src.data[src.step[0]*i + src.step[1]* j + 0];
//			int b = src.data[src.step[0]*i + src.step[1]* j + 1];
//			int c = src.data[src.step[0]*i + src.step[1]* j + 2];
//			if(( (a > 50 && b > 20 && c > 10) && (a < 200 && b < 105 && c < 100)) || ((a+b+c)/3 > 120) || (a > 100) )
//			{
//				src.data[src.step[0]*i + src.step[1]* j + 0] = 0;
//				src.data[src.step[0]*i + src.step[1]* j + 1] = 0;
//				src.data[src.step[0]*i + src.step[1]* j + 2] = 0;
//			}
//
//		}
//
//	cvtColor(src, dst, CV_BGR2GRAY);
//
//	Canny(dst, dst,60, 250, 3);
//	imshow("fufu", dst);
//
//	cvWaitKey(0);
//	
//	return 0;
//}