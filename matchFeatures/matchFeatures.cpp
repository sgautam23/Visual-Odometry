#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
//#include "opencv2/calib3d/calib3d.hpp"

using namespace cv;
using namespace std;

// Detect and match SURF features between two frames

int  main (int argc, char ** argv)

{
	VideoCapture cap(0); //device 0

	if( !cap.isOpened())
		return -1;  //couldn't open the device


	int minHessian=600;
	SurfFeatureDetector detector( minHessian );
	SurfDescriptorExtractor extractor;

	std::vector<KeyPoint> keypoints1, keypoints2;
	//FastFeatureDetector fastDetector(20);
	//std::vector<KeyPoint> surfKeypoints, fastKeypoints;

	Mat It1,It2;  //Frame 1 and 2
	Mat fIt1, fIt2;
	Mat descriptor1, descriptor2;
	Mat img_matches;

	std::vector< DMatch > matches;
	std::vector< DMatch > good_matches;

	cap>>It1; // capture first frame
	detector.detect(It1,keypoints1); // Get keypoints for first frame
	extractor.compute( It1,keypoints1,descriptor1);

	FlannBasedMatcher matcher;

	double max_dist = 0; double min_dist = 100;

	Mat E;


	for(;;)
	{	
		//Capturing a frame and detecting features
		// Comparing features with second frame

		cap>>It2; // capture second frame

		detector.detect(It2,keypoints2); //Get keypoints
		extractor.compute( It2,keypoints2,descriptor2);

		matcher.match(descriptor1,descriptor2, matches);

		
		for( int i = 0; i < descriptor1.rows; i++ )
			{ double dist = matches[i].distance;
				if( dist < min_dist ) min_dist = dist;
				if( dist > max_dist ) max_dist = dist;
			}

			for( int i = 0; i < descriptor1.rows; i++ )
			{ 
				if( matches[i].distance <= max(2*min_dist, 0.02) )
					{ good_matches.push_back( matches[i]); }
			}



		//drawMatches( It1, keypoints1, It2, keypoints2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );


			drawKeypoints(It1,keypoints1,fIt1,Scalar::all(-1), DrawMatchesFlags::DEFAULT);
			drawKeypoints(It2,keypoints2,fIt2,Scalar::all(-1), DrawMatchesFlags::DEFAULT);


			E = findEssentialMat(keypoints1, keypoints2, 3.67, Point2d(0, 0), "RANSAC", 0.999, 1.0, noArray() );

		//using FLANN matcher to match descriptors;


		//cap>>It2; //Get camera frame

		//imshow("Camera Frame 1", It1);
		//imshow("Camera Frame 2", It2);
			//imshow("Keypoints Frame 1",fIt1);
		//	imshow("Keypoints Frame 2",fIt2);
		//imshow("Matches",img_matches);



			It1=It2;
			keypoints1=keypoints2;
			descriptor1=descriptor2;


			if( waitKey(1) == 27 ) 
		 	break;  //break when ESC key is pressed


		}




		return 0;

	}