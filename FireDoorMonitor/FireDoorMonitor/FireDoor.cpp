#include "FireDoor.h"

FireDoor::FireDoor(const String &imgPath, char** argv){
	Mat tempImg = imread(imgPath, 0);
	resize(tempImg, tempImg, Size(1920, 1080));
	img = tempImg;
	status = false;
	for(size_t i = 5, n = 0; i != 13; ++i, ++n){
		point[n] = atoi(*(argv + i));
	}
}

/* ���ĸ����Ƴ�Ա��ʱ�ò���
FireDoor::FireDoor(const FireDoor &fireDoor) : img(fireDoor.img), status(fireDoor.status){
	for(size_t i = 0; i != 8; ++i)
		point[i] = fireDoor.point[i];
}

FireDoor::FireDoor(FireDoor &&fireDoor) : img(std::move(fireDoor.img)), status(std::move(fireDoor.status)){
	for(size_t i = 0; i != 8; ++i)
		point[i] = std::move(fireDoor.point[i]);
}

FireDoor& FireDoor::operator=(const FireDoor &fireDoor){
	img = fireDoor.img;
	status = fireDoor.status;
	for(size_t i = 0; i != 8; ++i)
		point[i] = fireDoor.point[i];
	return *this;
}

FireDoor& FireDoor::operator=(FireDoor &&fireDoor){
	if(this != &fireDoor){
		img = std::move(fireDoor.img);
		status = std::move(fireDoor.status);
		for(size_t i = 0; i != 8; ++i)
			point[i] = std::move(fireDoor.point[i]);
	}
	return *this;
}
*/

FireDoor::~FireDoor() {}

bool FireDoor::judgeByBrightness(double threshold){
	logging("��ʼִ�����ȼ��", false);
	double bv = calcBrightnessVariability();;
	logging("ͼ������ȱ仯ֵΪ " + to_string((long double) bv), false);
	status = ((bv > threshold) ? true : status);
	return status;
}

double FireDoor::calcBrightnessVariability(){
	//Э������󣬾�ֵ����
	Mat covar, mean1;
	Mat tempImg;
	resize(img, tempImg, Size(1280, 720));
	calcCovarMatrix(tempImg, covar, mean1, CV_COVAR_NORMAL | CV_COVAR_ROWS);
	//��ֵ����׼��
	Mat mean2, stdDev;
	meanStdDev(mean1, mean2, stdDev);
	//�������ȱ仯�̶�
	return stdDev.at<double>(0, 0);
}

bool FireDoor::judgeByLines(){
	logging("��ʼִ���������", false);
	//632, 144, 675, 822
	int topLeftCornerCoor[] = { (point[0] + point[2])/2 - 90, point[1] };
	Mat imgCut(img, Rect(topLeftCornerCoor[0], topLeftCornerCoor[1], 180, 800));
	//��Ե��ȡ
	Mat imgForLine;
	Canny(imgCut, imgForLine, 10, 100, 3);
	//������
	imgForLine = closeOperation(imgForLine);
	//��Ե��ȡ
	Canny(imgForLine, imgForLine, 10, 100, 3);
	int lineNum = hough(imgForLine);
	logging("������ⷽ���У������������봦��⵽ " + to_string((long long) lineNum) + " ������", false);
	status = ((lineNum > 2) ? true : status);
	return status;
}

Mat FireDoor::closeOperation(Mat &imgForLine){
	Mat tempImgForLine;
	Mat element5(43, 43, CV_8U, cv::Scalar(1));	//5*5�����Σ�8λuchar�ͣ�ȫ1�ṹԪ��  
	morphologyEx(imgForLine, tempImgForLine, cv::MORPH_CLOSE, element5, Point(-1, -1), 1);
	return tempImgForLine;
}

//����任
int FireDoor::hough(Mat &imgForLine){
	Mat ColorimgForLine;
	cvtColor(imgForLine, ColorimgForLine, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(imgForLine, lines, 1, CV_PI / 180, 100, 400, 80);	//�����ڶ�������700ò�Ƹ���400 80
	
	int temp = 0;
	for (size_t i = 0; i < lines.size(); ++i){
		line(ColorimgForLine, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(255, 0, 255), 2, 8);
		++temp;
	}
	return temp;
}

bool FireDoor::judgeByCounting(double threshold){
	logging("��ʼִ�е������", false);
	//��������ͼƬ��С
	resize(img, img, Size(800, 600));
	//��ֵ�˲�
	medianBlur(img, img, 5);
	//����һ��canny��Ե���
	Canny(img, img, 30, 50, 3);
	//����
	correct();
	double value = count(getTopImg(), getBottomImg());
	logging("������ⷽ���У���������������ͷ��⵽��ֵΪ " + to_string((long double) value), false);
	status = ((value > threshold) ? true : status);
	return status;
}

void FireDoor::correct(){
	Mat tempImg = img.clone();
	//�������ŵ��ĸ��ǵ� (632, 144) (1307, 144) (657, 966) (1278, 966)
	vector<Point2f> cornersCoor;
	cornersCoor.push_back(Point2f(273, 88));
	cornersCoor.push_back(Point2f(537, 88));
	cornersCoor.push_back(Point2f(281, 537));
	cornersCoor.push_back(Point2f(524, 537));
	//�ĸ��ǵ��ӦҪУ����λ��
	vector<Point2f> cornersCoor_trans;
	cornersCoor_trans.push_back(Point2f(100, 100));
	cornersCoor_trans.push_back(Point2f(700, 100));
	cornersCoor_trans.push_back(Point2f(100, 500));
	cornersCoor_trans.push_back(Point2f(700, 500));
	//ͼ�������ӳ���ϵ
	Mat transform = getPerspectiveTransform(cornersCoor, cornersCoor_trans);
	warpPerspective(tempImg, img, transform, tempImg.size());
}

Mat FireDoor::getTopImg(){
	Mat topImg(img, Rect(126, 480, 550, 40));
	return topImg;
}
Mat FireDoor::getBottomImg(){
	Mat bottomImg(img, Rect(126, 497, 550, 12));
	return bottomImg;
}

double FireDoor::count(Mat &topimg, Mat &buttomimg){
	//������ͳ�Ƹ�����
	double pointnum0 = 0, pointnum1 = 0;
	for (MatIterator_<uchar> grayit = topimg.begin<uchar>(), grayend = topimg.end<uchar>(); grayit != grayend; ++grayit){
		if (*grayit > 0)
			++pointnum0;
	}
	for (MatIterator_<uchar> grayit = buttomimg.begin<uchar>(), grayend = buttomimg.end<uchar>(); grayit != grayend; ++grayit){
		if (*grayit > 0)
			++pointnum1;
	}
	return pointnum1 / pointnum0;
}



