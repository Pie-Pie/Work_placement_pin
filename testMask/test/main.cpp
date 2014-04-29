#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;


Mat src, src_gray, dst, detected_edges;

void show_result(const cv::Mat& labels, const cv::Mat& centers, int height, int width);

int main( int argc, char** argv )
{///355 77

	IplImage *img;

	img = cvLoadImage("2014-04-07/03_VIS_sv_090-0-0-0.png", -1);

	namedWindow( "fufu", 0);
	namedWindow( "fufuline", 0);
	src = Mat(img);

	//set useless pixels to black
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
	//cvtColor( fu, fu, COLOR_GRAY2BGR );
	
	IplImage* fuIpl = new IplImage(fu);

	cvConvertScale(fuIpl, fuIpl, 256);

	Mat fufu(fuIpl);

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


	//Horizontal
	int rows = src.rows;

	int countMax = 0;
	int iMax;
	int sumMax;


	int compteur = 0;

	for(int i = 2000 ; i < fufu.rows - 300 ; i+=3)
	{
		for (int j = 0; j < fufu.cols; j++)
		{
			int a = src.data[src.step[0]*i + src.step[1]* j + 0];
			int b = src.data[src.step[0]*i+1 + src.step[1]* j + 0];
			if(a!=0)
			{
			  compteur++;
			}
			if(b!=0)
			{
			  compteur++;
			}

		}

		sumMax = compteur;
		if (sumMax > countMax)
		{
			countMax = sumMax;
			iMax = i;
		}
		compteur = 0;
	}

	//Vertical
	int cols = src.cols;

	int countMaxV = 0;
	int j1, j2;
	int i1, i2, itemp;
	bool suite = false;
	int sumMaxv;


	int compteurv = 0;

	for (int j = 0 ; j < fufu.cols ; j+=8)
	{
		for(int i = 2000 ; i < fufu.rows - 300 ; i++)
		{
			int a = src.data[src.step[1]*j + src.step[0]* i + 0];
			int b = src.data[src.step[1]*j+1 + src.step[0]* i + 0];
			int c = src.data[src.step[1]*j+2 + src.step[0]* i + 0];
			int d = src.data[src.step[1]*j+3 + src.step[0]* i + 0];
			int e = src.data[src.step[1]*j+4 + src.step[0]* i + 0];
			int f = src.data[src.step[1]*j+5 + src.step[0]* i + 0];

			if(a!=0)
			{
			  compteurv++;
			}
			if(b!=0)
			{
			  compteurv++;
			}
			if(c!=0)
			{
			  compteurv++;
			}
			if(d!=0)
			{
			  compteurv++;
			}
			if(e!=0)
			{
			  compteurv++;
			}
			if(f!=0)
			{
			  compteurv++;
			}

			itemp = i;

		}

		sumMaxv = compteurv;
		if (sumMaxv > countMaxV)
		{
			countMaxV = sumMaxv;
			if(suite == false && countMaxV > 300)
			{
				j1 = j;
				i1 = itemp;
				suite = true;
			}
			if(countMaxV > 300 && suite == true)
				j2 = j;
				i2 = itemp;

		}
		compteurv = 0;
	}
	threshold(dst, dst, 40, 255, THRESH_OTSU);

	imwrite("2014-04-07/WIP03_VIS_sv_090-0-0-0.png", dst);

	//find j1 and j2 extremities

	
	int a = dst.data[dst.step[0]*iMax + dst.step[1]* j1 + 0];
	int b = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 1];
	int c = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 2];
	int d = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 3];
	int e = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 4];
	int f = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 5];
	int g = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 6];
	int h = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 7];
	int k = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 8];
	int l = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 9];
	int m = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 10];
	int n = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 11];
	int o = dst.data[dst.step[0]*iMax + dst.step[1]* j1 - 12];
	if (a != 0)
	{
		j1 = j1;
		if (b != 0)
		{
			j1 = j1--;
			if (c != 0)
			{
				j1 = j1--;
				if (d != 0)
				{
					j1 = j1--;
					if (e != 0)
					{
						j1 = j1--;
						if( f != 0)
						{
							j1 = j1--;
							if( g != 0)
							{
								j1 = j1--;
								if( h != 0)
								{
									j1 = j1--;
									if( k != 0)	
									{
										j1 = j1--;
										if( l != 0)
										{
											j1 = j1--;
											if( m != 0)
											{
												j1 = j1--;
												if( n != 0)
												{
													j1 = j1--;
													if( o != 0)
													{
														j1 = j1--;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
		

	
	a = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 0];
	b = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 1];
	c = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 2];
	d = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 3];
	e = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 4];
	f = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 5];
	g = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 6];
	h = dst.data[dst.step[0]*iMax + dst.step[1]* j2 + 7];
	k = dst.data[dst.step[0]*iMax + dst.step[1]* j1 + 8];
	l = dst.data[dst.step[0]*iMax + dst.step[1]* j1 + 9];
	m = dst.data[dst.step[0]*iMax + dst.step[1]* j1 + 10];
	n = dst.data[dst.step[0]*iMax + dst.step[1]* j1 + 11];
	o = dst.data[dst.step[0]*iMax + dst.step[1]* j1 + 12];
	if (a != 0)
	{
		j2 = j2;
		if (b != 0)
		{
			j2 = j2++;
			if (c != 0)
			{
				j2 = j2++;
				if (d != 0)
				{
					j2 = j2++;
					if (e != 0)
					{
						j2 = j2++;
						if( f != 0)
						{
							j2 = j2++;
							if( g != 0)
							{
								j2 = j2++;
								if( h != 0)
								{
									j2 = j2++;
									if( k != 0)	
									{
										j2 = j2++;
										if( l != 0)
										{
											j2 = j2++;
											if( m != 0)
											{
												j2 = j2++;
												if( n != 0)
												{
													j2 = j2++;
													if( o != 0)
													{
														j2 = j2++;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}





	for (int i = iMax - 50 ; i < dst.rows - 200 ; i++)
	{
		for (int j = j1- 5 ; j <= j2 + 5 ; j++)
		{
			int a = dst.data[dst.step[0]*i + dst.step[1]* j + 0];

			if (a != 0 )
				dst.data[dst.step[0]*i + dst.step[1]* j + 0] = 0;
		}
	}
	
	for (int i = dst.rows - 200 ; i < dst.rows ; i++)
	{
		for (int j = 0 ; j < dst.cols ; j++)
		{
			int a = dst.data[dst.step[0]*i + dst.step[1]* j + 0];

			if (a != 0 )
				dst.data[dst.step[0]*i + dst.step[1]* j + 0] = 0;
		}
	}

	erode(dst,dst, Mat(3,3, DataType<int>::type),Point(-1, -1), 1, 1, 1 );
	dilate(dst,dst, Mat(3,3, DataType<int>::type),Point(-1, -1), 1, 1, 1 );
	imshow("fufuline", fufu);
	imshow("fufu", dst);
	//show_result(labels, centers, dst.rows, dst.cols);



	
	imwrite("2014-04-07/WIP_2_03_VIS_sv_090-0-0-0.png", dst);

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