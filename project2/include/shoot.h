#pragma once
#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//����һ��ֻ��װ������ת���ε��࣬����һ
class vec2RotatedRect {//���2����������ת�����һ���࣬�е���vec2b
public:
	vector<RotatedRect>arr;
	//arr.resize;
	vec2RotatedRect(RotatedRect& a, RotatedRect& b) {
		arr.push_back(a);
		arr.push_back(b);
	}
	//�����Ϳ���ֻ����������������
	//���ǲ�֪��Ϊʲô�����Գ�ʼ��Ϊ����2
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
