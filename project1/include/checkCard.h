#pragma once

#include<opencv2/opencv.hpp>

using namespace cv;

class Check {//����������п�ͼƬ����
public:
	Mat erzhihua(Mat& image);//��ֵ��
	void showContours(Mat& image);
	void tezheng(Mat& img1, Mat& img2);

};