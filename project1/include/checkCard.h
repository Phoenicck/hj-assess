#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
using namespace cv;
using namespace std;
class Check {//用来检测银行卡图片的类
public:
	Mat erzhihua(Mat& image);//二值化
	void findcardnum(Mat& image, vector<Mat>&cardNumber , vector<Rect>&Re);
	void findmoban(Mat& image, vector<Mat>&mubiao);
	void tezheng(Mat& img1, Mat& img2);
	vector<int> acgHash(Mat& image);
	int xiangsidu(vector<int> &has1, vector<int>&has2);
	vector< vector<int>> mobanHash(vector<Mat>&mubiao);

};