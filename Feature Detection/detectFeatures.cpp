#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace std;

// Detect and match SURF features between two frames

int  main (int argc, char ** argv)

{
	VideoCapture cap(0); //device 0

	if( !cap.isOpened())
		return -1;  //couldn't open the device


	int minHessian=600;
	SurfFeatureDetector surfDetector( minHessian );

	FastFeatureDetector fastDetector(20);

	std::vector<KeyPoint> surfKeypoints, fastKeypoints;

	Mat It1,It2;  //Frame 1 and 2

	Mat fIt1, fIt2;

	cap>>It1;

	for(;;)
	{	
		//Capturing a frame and detecting SURF and FAST features

		surfDetector.detect(It1,surfKeypoints);
		fastDetector.detect(It1,fastKeypoints);

		drawKeypoints(It1,surfKeypoints,fIt1,Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(It1,fastKeypoints,fIt2,Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		



		//cap>>It2; //Get camera frame

		imshow("Camera Frame", It1);
		imshow("Surf Keypoints",fIt1);
		imshow("Fast Keypoints",fIt2);


		cap>>It1;

		 if( waitKey(1) == 27 ) 
		 	break;  //break when ESC key is pressed
		}




		return 0;
	
}