#pragma once

#include<opencv2/opencv.hpp>

using namespace cv;

class Check {//用来检测银行卡图片的类
public:
	Mat erzhihua(Mat& image);//二值化
	void showContours(Mat& image);
	void tezheng(Mat& img1, Mat& img2);

};