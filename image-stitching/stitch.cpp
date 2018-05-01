#include "includes.h"

int main ( int argc, char** argv)
{
	stringstream paramfile ( argv[1] );
	vector<string> params = readparameters ( paramfile.str() );

	string filename = params[0];
	string savefile = params[1];
	int skip		= stoi ( params[2] );
	int start		= stoi ( params[3] );
	int max_images  = stoi ( params[4] );
	vector<int> dimensions;
	for ( int i = 5; i <=8; i++)
	{
		dimensions.push_back ( stoi ( params[i] ) );
	}
	bool showset	= stob ( params[9] );
	stringstream savestream;
	savestream << savefile << "startframe" << start << "step" << skip << "max_images" << max_images << ".jpg";

	vector<Mat> imgs = video2frames ( filename, dimensions, skip, max_images, start, showset );
	Stitcher stitcher = Stitcher::createDefault();

	Mat pano;

	Stitcher::Status status = stitcher.stitch(imgs, pano);
	bool s =statuscheck(status);

	if (s == false)
		return -1;
	else
	{
		imwrite(savestream.str(), pano);
		imshow("stitched", pano);
		waitKey(0);
		destroyAllWindows ();
		return 0;
	}

}