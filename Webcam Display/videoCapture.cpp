#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

int  main (int argc, char ** argv)

{
	VideoCapture cap(0); //device 0

	if( !cap.isOpened())
		return -1;  //couldn't open the device


	Mat frame;

	for(;;)
	{
		
		cap>>frame; //Get camera frame

		imshow("Camera Frame", frame);

		 if( waitKey(1) == 27 ) 
		 	break;  //break when ESC key is pressed
		}




		return 0;
	
}