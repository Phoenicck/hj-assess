#pragma once

#include<opencv2/opencv.hpp>
#include<vector>
using namespace cv;

class Check {//����������п�ͼƬ����
public:
	Mat erzhihua(Mat& image);//��ֵ��
	void findmubiao(Mat& image);
	void moban(Mat& image);
	void tezheng(Mat& img1, Mat& img2);

};