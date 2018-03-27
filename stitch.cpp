#include <iostream>
#include <fstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


Mat crop( Mat img , int x1, int x2, int y1, int y2)
{
	cout << (x2-x1)/2 << " " << (y2-y1)/2 << " " << x2-x1 << " " << y2-y1 << endl;
	Rect cropper = Rect(0,0,x2-x1, y2-y1);
	cout<< "Built rect" << endl;
	Mat cropped = img(cropper);
	cout << "built cropped" << endl;
	return cropped ; 
}

bool statuscheck(Stitcher::Status stat)
{
		if ( stat != Stitcher::OK)
	{
		cout <<"Can't stitch images\n" << endl;
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

	for ( int i = 1; i < 10; i++ )
	{
		stringstream ss;
		ss << "../New folder/Hole_0" << i << ".jpg";
		
		Mat img = imread(ss.str());
		cout << img.cols << endl;
		Mat cropped = crop(img,1, img.cols-1, 1, 965);

		imgs.push_back(img);
	}

	Mat pano;
	Stitcher::Status status = stitcher.stitch(imgs, pano);
	bool s =statuscheck(status);

	if (s == false)
		return -1;
	else
	{
		imwrite("stitched.jpg", pano);
		imshow("stitched", pano);
		waitKey(0);
		return 0;
	}

}