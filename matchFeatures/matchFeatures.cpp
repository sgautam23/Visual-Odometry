#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
// #include "opencv2/nonfree/features2d.hpp"
#include <vector>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

// Detect and match SURF features between two frames

int  main (int argc, char ** argv)

{	
	int deviceNumber=1;
	VideoCapture cap(deviceNumber); //device 0

	if( !cap.isOpened())
		return -1;  //couldn't open the device


	int minHessian=600;
	

	//SURF
	Ptr<SURF> detector1= SURF::create( minHessian );
	Ptr<SURF> detector2 = SURF::create( minHessian );


	//FAST
	// FeatureDetector* detector1;
	// FeatureDetector* detector2;
	// detector1->create("FAST");
	// detector2->create("FAST");

	//std::vector<KeyPoint> surfKeypoints, fastKeypoints;

	Mat It1,It2;  //Frame 1 and 2
	Mat fIt1, fIt2;
	Mat descriptor1, descriptor2;
	std::vector<KeyPoint> keypoints1, keypoints2;

	Mat k =(Mat_<double>(3,3) << 794.204174, 0, 258.925189, 0, 790.914562, 240.688826, 0, 0, 1);
	
	Mat E, R, t, mask;

	

	cap>>It1; // capture first frame

	FlannBasedMatcher matcher;
	Mat img_matches;
	std::vector< DMatch > matches;
	std::vector< DMatch > good_matches;
	std::vector<Point2f> pts1, pts2;


	double max_dist = 0; double min_dist = 100;


	double focal = 3.7;
	cv::Point2d pp(0, 0);


	for(;;)
	{
		//Capturing a frame and detecting features
		// Comparing features with second frame

		cap>>It2; // capture second frame

		matches.clear();
		good_matches.clear();
		keypoints1.clear();
		keypoints2.clear();
		//SURF
		detector1->detectAndCompute( It1, mask, keypoints1, descriptor1 );//For second frame
		detector2->detectAndCompute( It2, mask, keypoints2, descriptor2 );//For second frame
		
		//FAST
		// detector1-> detect(It1,keypoints1);
		// detector2->detect(It2,keypoints2);
		
		matcher.match(descriptor1,descriptor2, matches);
		
		max_dist=0;
		min_dist=100;
		for( int i = 0; i < descriptor1.rows; i++ )
			{ 
				double dist = matches[i].distance;
				if( dist < min_dist ) 
					min_dist = dist;
				if( dist > max_dist ) 
					max_dist = dist;
			}

			for( int i = 0; i < descriptor1.rows; i++ )
			{
				if( matches[i].distance <= max(2*min_dist, 0.02) )
					{ good_matches.push_back( matches[i]); 
					}
			}

		drawMatches( It1, keypoints1, It2, keypoints2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

		drawKeypoints(It1,keypoints1,fIt1,Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(It2,keypoints2,fIt2,Scalar::all(-1), DrawMatchesFlags::DEFAULT);


		for (auto i : good_matches)
		{
			pts1.push_back(keypoints1[i.queryIdx].pt);
			pts2.push_back(keypoints2[i.queryIdx].pt);
		}

		Mat fundamental_matrix = findFundamentalMat(pts1, pts2, FM_RANSAC, 3, 0.99);
		cout<<fundamental_matrix;

	  //recovering the pose and the essential matrix

		// E = findEssentialMat(keypoints1,keypoints2, focal, pp, RANSAC, 0.999, 1.0, mask);
		// cout<<E<<endl;
	  //recoverPose(E, keypoints1,keypoints2, R, t, focal, pp, mask);

		//using FLANN matcher to match descriptors;

		// imshow("Camera Frame 1", It1);
		// imshow("Camera Frame 2", It2);
		//cout<<fIt1.size();
			// imshow("Keypoints Frame 1",fIt1);
			// imshow("Keypoints Frame 2",fIt2);
			imshow("Matches",img_matches);


		//Make old frame the new frame
			It1=It2;
		//	keypoints1=keypoints2;
		//	descriptor1=descriptor2;



			if( waitKey(1) == 27 )
		 	break;  //break when ESC key is pressed


		}




		return 0;

	}
