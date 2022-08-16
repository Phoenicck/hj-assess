#pragma once

#include<opencv2/opencv.hpp>
#include<vector>
using namespace cv;

class Check {//用来检测银行卡图片的类
public:
	Mat erzhihua(Mat& image);//二值化
	void findmubiao(Mat& image);
	void moban(Mat& image);
	void tezheng(Mat& img1, Mat& img2);

};