#include <iostream>
#include <fstream>
#include <vector>

#include "opencv2/opencv.hpp"
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;

vector<string> readparameters ( string filename )
{
    vector<string> parameters ;
    ifstream inFile;
    char x;
    
    inFile.open(filename);
    if (!inFile) {
        cerr << "Unable to open parameter file";
        exit(1);
    }

    while (inFile >> x) 
    {
        string sum;
        if ( x == ':')
        {

            while ( inFile.peek() != '\n' && inFile >> x )
            {
                sum += x;
            }

            parameters.push_back (sum);
        }
    }

    inFile.close();
    return parameters ;

}
bool stob ( std::string truefalse )
{
  if ( truefalse == "true" || truefalse == "1")
  {
    return true;
  }
  else
  {
    return false;
  }
}
void crop( Mat &img , vector<int> dimensions )
{
    bool nonzero = false;
    for ( int i = 0; i < dimensions.size(); i++ )
    {
        if ( dimensions[i] != 0 )
        {
            nonzero = true;
            break;
        }
    }

    if ( nonzero )
    {
        int x1 = dimensions[0]; int x2 = dimensions[1]; 
        int y1 = dimensions[2]; int y2 = dimensions[3];
        Rect cropper = Rect(x1,y1,x2, y2);
        img = img(cropper);
    }
}

vector < Mat > video2frames ( string filename, vector<int> dimensions, int skip_steps, int max_size=200 , int start = 0, bool showset = false)
{
    vector < Mat > frameset;
    Mat single_frame;
    VideoCapture video ( filename );
    video.set( 1, start );
    int i = 0;
    while ( video.read ( single_frame )  && frameset.size() < max_size)
    {
        i++;
        if ( i % skip_steps == 0 )
        {
            crop ( single_frame, dimensions );
            stringstream  s ( "frame" );
            s << i;
            if ( showset )
            {
                imshow(s.str(), single_frame);
                waitKey(0);
                destroyAllWindows ();
            }

            frameset.push_back ( single_frame );
        }
        
    }

    return frameset;
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