#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//定义一个只能装两个旋转矩形的类，方案一
class vec2RotatedRect {//存放2个灯条的旋转矩阵的一个类，有点像vec2b
public:
	vector<RotatedRect>arr;
	//arr.resize;
	vec2RotatedRect(RotatedRect& a, RotatedRect& b) {
		arr.push_back(a);
		arr.push_back(b);
	}
	//这样就可以只有两个灯条的类了
	//但是不知道为什么不可以初始化为长度2
};


class CheckJiaban {
public:
	void Drawrotedrec(Mat& src, RotatedRect& rotatedrect, const Scalar& color, int thickness);
	float distance(const cv::Point2f& a, const cv::Point2f& b);
	Mat jiaozheng(Mat& image);
	Mat chuli(Mat& image);
	vector<RotatedRect> findLight(Mat&image,Mat&src);
	RotatedRect findJiban(vector<RotatedRect>&light, Mat& src);
	void zuobiao(RotatedRect& light, Mat& src);
};
