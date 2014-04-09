#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;


Mat src, src_gray, dst, detected_edges;

void show_result(const cv::Mat& labels, const cv::Mat& centers, int height, int width);

int main( int argc, char** argv )
{

	IplImage *img;

	img = cvLoadImage("2014-04-07/03_VIS_sv_090-0-0-0.png", -1);

	namedWindow( "fufu", 0);
	namedWindow( "fufuline", 0);
	src = Mat(img);

	//set to black useless pixels
	for(int i = 0 ; i < src.rows ; i++)
		for(int j = 0 ; j < src.cols ; j++)
		{
			int a = src.data[src.step[0]*i + src.step[1]* j + 0];
			int b = src.data[src.step[0]*i + src.step[1]* j + 1];
			int c = src.data[src.step[0]*i + src.step[1]* j + 2];
			if(( (a > 50 && b > 20 && c > 10) && (a < 200 && b < 90 && c < 80)) || ((a+b+c)/3 > 120) || (a > 100) )
			{
				src.data[src.step[0]*i + src.step[1]* j + 0] = 0;
				src.data[src.step[0]*i + src.step[1]* j + 1] = 0;
				src.data[src.step[0]*i + src.step[1]* j + 2] = 0;
			}

		}


	//Test with HSV
	Mat srcHSV;

	cvtColor(src, srcHSV, CV_BGR2HSV_FULL);

	for(int i = 0 ; i < src.rows ; i++)
		for(int j = 0 ; j < src.cols ; j++)
		{
			int a = src.data[src.step[0]*i + src.step[1]* j + 0];
			int b = src.data[src.step[0]*i + src.step[1]* j + 1];
			int c = src.data[src.step[0]*i + src.step[1]* j + 2];

			if( b < 100 )
			{
				src.data[src.step[0]*i + src.step[1]* j + 0] = 0;
				src.data[src.step[0]*i + src.step[1]* j + 1] = 0;
				src.data[src.step[0]*i + src.step[1]* j + 2] = 0;
			}

		}

	//dst = src;


	//K means

	//Mat labels;
	//double compactness;

	// Mat reshaped_image = dst.reshape(1, dst.cols * dst.rows);

 //    assert(reshaped_image.type() == CV_8UC1);
 //        
 //    Mat reshaped_image32f;
 //    reshaped_image.convertTo(reshaped_image32f, CV_32FC1, 1.0 / 255.0);
 //    assert(reshaped_image32f.type() == CV_32FC1);
	// Mat centers;
	// TermCriteria criteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0);
	// cvtColor(dst, dst, CV_BGR2GRAY);
	// kmeans(reshaped_image32f, 10, labels, criteria, 3, KMEANS_RANDOM_CENTERS, centers );
	cvtColor(srcHSV, dst, CV_HSV2BGR_FULL);
	cvtColor(dst, dst, CV_BGR2GRAY);


	Mat fu = Mat(src.rows, src.cols, DataType<int>::type);

	Canny(dst, fu, 40, 255, 3);
	cvtColor( fu, fu, COLOR_GRAY2BGR );

	Mat fufu;

	fu.convertTo(fufu, CV_8UC3);

#if 0
	vector<Vec2f> lines;

	HoughLinesP(fufu, lines, CV_PI/2, 2, 80, 30, 1);

	for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000*(-b)),
                  cvRound(y0 + 1000*(a)));
        Point pt2(cvRound(x0 - 1000*(-b)),
                  cvRound(y0 - 1000*(a)));
        line( fufu, pt1, pt2, Scalar(0,0,255), 3, 8 );
    }

#else
	vector<Vec4i> lines;
    HoughLinesP( fufu, lines, CV_PI/2, 2, 80, 30, 1);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        line( fufu, Point(lines[i][0], lines[i][1]),
            Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }

#endif

	imshow("fufuline", fufu);


	threshold(dst, dst, 40, 255, THRESH_OTSU);
	


	//erode(dst,dst, Mat(2,2, DataType<int>::type),Point(-1, -1), 1, 1, 1 );
	//dilate(dst,dst, Mat(2,2, DataType<int>::type),Point(-1, -1), 1, 1, 1 );

	imshow("fufu", dst);
	//show_result(labels, centers, dst.rows, dst.cols);



	
	imwrite("2014-04-07/WIP03_VIS_sv_090-0-0-0.png", dst);

	cvWaitKey(0);
	
	return 0;
}

//For displaying with k means
//void show_result(const cv::Mat& labels, const cv::Mat& centers, int height, int width)
//{
//        std::cout << "===\n";
//        std::cout << "labels: " << labels.rows << " " << labels.cols << std::endl;
//        std::cout << "centers: " << centers.rows << " " << centers.cols << std::endl;
//        assert(labels.type() == CV_32SC1);
//        assert(centers.type() == CV_32FC1);
//         
//        cv::Mat rgb_image(height, width, CV_8UC3);
//        cv::MatIterator_<cv::Vec3b> rgb_first = rgb_image.begin<cv::Vec3b>();
//        cv::MatIterator_<cv::Vec3b> rgb_last = rgb_image.end<cv::Vec3b>();
//        cv::MatConstIterator_<int> label_first = labels.begin<int>();
//         
//        cv::Mat centers_u8;
//        centers.convertTo(centers_u8, CV_8UC1, 255.0);
//        cv::Mat centers_u8c3 = centers_u8/*.reshape(3)*/;
//         
//        while ( rgb_first != rgb_last ) {
//                const cv::Vec3b& rgb = centers_u8c3.ptr<cv::Vec3b>(*label_first)[0];
//                *rgb_first = rgb;
//                ++rgb_first;
//                ++label_first;
//        }
//		cvtColor(rgb_image, rgb_image, CV_BGR2GRAY);
//		threshold(rgb_image, rgb_image, 0, 60, THRESH_OTSU);
//        cv::imshow("fufu", rgb_image);
//        cv::waitKey();
//}


//Old, work

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