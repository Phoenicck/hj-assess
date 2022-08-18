#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class CheckJiaban {
public:
	Mat jiaozheng(Mat& image);
	Mat chuli(Mat& image);
	void findJiban(Mat&image,Mat&src);
};