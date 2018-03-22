#ifndef _FIREDOOR
#define _FIREDOOR
#include <cv.h>  
#include <highgui.h>  
#include <math.h>  
#include <iostream>
#include <string>
#include <windows.h>
#include <opencv2/opencv.hpp>  
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>  
#include "cxcore.h"
#include "PrintUtils.h"

using namespace cv;
using namespace std;

class FireDoor{
private:
	Mat img;
	bool status;
	int point[8];
	Mat closeOperation(Mat &newimg);
	int hough(Mat &imgForLine);
	void correct();
	Mat getTopImg();
	Mat getBottomImg();
	double count(Mat &topimg, Mat &buttomimg);
public:
	explicit FireDoor(const string &imgPath, char** argv);
	FireDoor(const FireDoor &fireDoor);
	FireDoor(FireDoor &&fireDoor);
	FireDoor& operator=(const FireDoor &fireDoor);
	FireDoor& operator=(FireDoor &&fireDoor);
	~FireDoor();
	double calcBrightnessVariability();
	bool judgeByBrightness(double threshold);
	bool judgeByLines();
	bool judgeByCounting(double threshold);

};

#endif  