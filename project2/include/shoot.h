#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class vec2RotatedRect {//存放2个灯条的旋转矩阵的一个类，有点像vec2b
public:
	vector<RotatedRect> arr;
	vec2RotatedRect(RotatedRect& a, RotatedRect& b) {
		arr.push_back(a);
		arr.push_back(b);
	}
	//这样就可以只有两个灯条的类了
	//但是不知道为什么不可以初始化为长度2
};
class CheckJiaban {
public:
	Mat jiaozheng(Mat& image);
	Mat chuli(Mat& image);
	vector<RotatedRect> findLight(Mat&image,Mat&src);
	void findJiban(vector<RotatedRect>&light, Mat& src);
};