#include <iostream>
#include <fstream>

#include "opencv2/opencv.hpp"
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;


Mat crop( Mat img , int x1, int x2, int y1, int y2)
{
	Rect cropper = Rect(0,0,x2-x1, y2-y1);
	Mat cropped = img(cropper);
	return cropped ; 
}

vector<KeyPoint> detectFeatures ( Mat img )
{
	int minHessian = 400;
	static Ptr<xfeatures2d::SURF> detector = xfeatures2d::SURF::create(minHessian);
	vector<KeyPoint> keypoints;
	detector->detect ( img, keypoints);


	return keypoints;
}

void showKeyPoints ( vector<vector<KeyPoint> > keys, vector<Mat> imgs )
{
	for ( int i = 0; i < imgs.size(); i++ )
	{
		Mat img_keys ;
		drawKeypoints( imgs[i], keys[i], img_keys ) ;
		stringstream ss;
		ss << i;
		imshow ( ss.str(), img_keys );
	}
	waitKey(0);
}

bool statuscheck(Stitcher::Status stat)
{
		if ( stat != Stitcher::OK)
	{
		cout << "status issue: " <<  stat << endl;
		return false;
	}
	else
	{
		return true;
	}
}

int main ( int argc, char** argv)
{

	vector<Mat> imgs;
	Stitcher stitcher = Stitcher::createDefault();
	/*
	for ( int i = 1; i < 15; i++ )
	{
		stringstream ss;
		if ( i < 10 )
		{
			ss << "../brian-images/Hole_0" << i << ".jpg";
		}
		else
		{
			ss << "../brian-images/Hole_" << i << ".jpg";
		}
		
		Mat img = imread(ss.str());
		Mat cropped = crop(img,1, img.cols-1, 1, 965);
		Mat reduced ;
		resize (cropped, reduced, Size(), 0.5, 0.5) ;
		imgs.push_back ( reduced );
	}
	*/

	for ( int i = 0; i < 5; i++)
	{
		stringstream ss;
		if (i < 5 )
		{
			ss << "../brian-images/Hole_2_" << i+2 << ".jpg";
		}
		else
		{
			ss << "../brian-images/Hole_9_" << i-3 << ".jpg";
		}
		cout << ss.str() << endl;
		Mat img = imread(ss.str());
		Mat cropped = crop(img,1, img.cols-1, 1, 965);
		Mat reduced ;
		resize (cropped, reduced, Size(), 0.5, 0.5) ;
		imgs.push_back ( reduced );
	}

	Mat pano;

	Stitcher::Status status = stitcher.stitch(imgs, pano);
	bool s =statuscheck(status);

	if (s == false)
		return -1;
	else
	{
		imwrite("../stitched.jpg", pano);
		imshow("stitched", pano);
		waitKey(0);
		return 0;
	}

}