#include "includes.h"

int main ( int argc, char** argv)
{
	stringstream paramfile ( argv[1] );
	vector<string> params = readparameters ( paramfile.str() );

	string filename = params[0];
	int skip		= stoi ( params[1] );
	int start		= stoi ( params[2] );
	int max_images  = stoi ( params[3] );
	vector<int> dimensions;
	for ( int i = 4; i <=7; i++)
	{
		dimensions.push_back ( stoi ( params[i] ) );
	}
	float resize_factor = stof ( params[8] );
	bool showset	= stob ( params[9] );
	bool imdirectory	= stob ( params[10] );
	bool viddirectory	= stob ( params[11] );
	string stitchtype	= 		params[12] ;
	stringstream savestream;
	vector<vector<vector<Mat>>> videoset;
	string ext;
	vector<string> videofiles;
	if ( !( imdirectory || viddirectory ) )
	{
		savestream << filename << "startframe" << start << 
						"step" << skip << "max_images" << 
						max_images << "resize" << resize_factor << ".jpg";

		vector<vector<Mat> > imgset = video2frames ( filename + ".mp4", 
										dimensions, skip, max_images, 
										start, resize_factor, showset );

		videoset.push_back ( imgset );
		videofiles.push_back ( filename );
	}
	else if (imdirectory)
	{

		vector<Mat> imgs;
		vector<vector<Mat>> set;
		ext = ".jpg";
		get_images_from_dir ( filename, ext, imgs );
		set.push_back(imgs);
		videoset.push_back(set);
		cout << imgs.size() << endl;
		videofiles.push_back ( filename );
	}
	else
	{
		ext = ".mp4";
		
		get_filenames_from_dir ( filename, ext, videofiles );
		for (size_t i = 0; i < videofiles.size (); i++)
		{
			vector< vector<Mat> > imgset = video2frames ( 
											videofiles[i], dimensions, skip, 
											max_images, start, resize_factor, showset);
			videoset.push_back ( imgset );

		}


	}

	Stitcher::Mode mode;
	if ( stitchtype == "pano" )
		{mode = Stitcher::PANORAMA; cout << "panorama stitch"<<endl;}
	else
		{mode = Stitcher::SCANS; cout << "scan stitch" << endl;}
	bool try_use_gpu = true;
	Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);

	for (size_t scansize = 0; scansize < videoset.size(); scansize++)
	{
		stringstream savestream;
		savestream << videofiles[scansize] << "startframe" << start << 
				"step" << skip << "max_images" << 
				max_images << "resize" << resize_factor;

		for (size_t numimages = 0; numimages < videoset[scansize].size(); numimages++ )
		{
			cout << videoset[scansize][numimages].size() << endl;
			
			Mat pano;
			Stitcher::Status status = stitcher->stitch(videoset[scansize][numimages], pano);
			bool s =statuscheck(status);

			if ( s == true )
			{
				stringstream minstream;
				minstream << savestream.str() << "s" << numimages << ".jpg";
				imwrite(minstream.str(), pano);
				cout << "Stitch " << numimages+1 << " of " << 
				videoset[scansize].size() << 
				" in set " << scansize+1 <<
				" of " << videoset.size() << " complete" << endl;
				// imshow("stitched", pano);
				// waitKey(0);
				// destroyAllWindows ();
				
			}
			else
			{
				cout << "Stitch " << numimages+1 << " of " << 
				videoset[scansize].size() << 
				" in set " << scansize+1 <<
				" of " << videoset.size() << " failed." << endl;
			}
		}
	}
	return 0;
}