#include <iostream>
#include <fstream>
#include <vector>

#include "opencv2/opencv.hpp"
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;

void get_images_from_dir (const string _filename, const string ext, vector<Mat> &image_set)
{
  vector < String > fn;
  glob ( _filename + "/*" + ext, fn, false);
  for ( size_t i = 0; i < fn.size(); i ++ )
  {
    image_set.push_back ( imread( fn[i] ) );
  }
}
void get_filenames_from_dir ( const string _filename, const string ext, vector<string> &filenames )
{
      vector < String > fn;
  glob ( _filename + "/*" + ext, fn, false);
  for ( size_t i = 0; i < fn.size(); i ++ )
  {
    filenames.push_back ( fn[i] );
  }
}

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

vector< vector<Mat> > video2frames ( string filename, vector<int> dimensions, int skip_steps, int max_size=200 , int start = 0, float resize_factor = 1, bool showset = false)
{
    vector < Mat > frameset;
    vector< vector < Mat > > full_image_stack;
    Mat single_frame;

    VideoCapture video ( filename );
    cout << "video capture: " << filename << endl;
    video.set( 1, start );

    int i = 0;
    while ( video.read ( single_frame ) )
    {
        i++;
        if ( i % skip_steps == 0 )
        {
            crop ( single_frame, dimensions );
            resize ( single_frame, single_frame, Size(), resize_factor, resize_factor);
            stringstream  s;
            s << "frame " << i + start;
            if ( showset )
            {
                imshow(s.str(), single_frame);
                waitKey(0);
                destroyAllWindows ();
            }

            frameset.push_back ( single_frame );
        }

        if ( frameset.size() >= max_size )
        {
            
            full_image_stack.push_back ( frameset );
            frameset.clear ();
        }
        
    }
    cout << "full stack size: " << full_image_stack.size() <<endl;
    return full_image_stack;
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