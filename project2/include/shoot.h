#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class vec2RotatedRect {//���2����������ת�����һ���࣬�е���vec2b
public:
	vector<RotatedRect> arr;
	vec2RotatedRect(RotatedRect& a, RotatedRect& b) {
		arr.push_back(a);
		arr.push_back(b);
	}
	//�����Ϳ���ֻ����������������
	//���ǲ�֪��Ϊʲô�����Գ�ʼ��Ϊ����2
};
class CheckJiaban {
public:
	Mat jiaozheng(Mat& image);
	Mat chuli(Mat& image);
	vector<RotatedRect> findLight(Mat&image,Mat&src);
	void findJiban(vector<RotatedRect>&light, Mat& src);
};