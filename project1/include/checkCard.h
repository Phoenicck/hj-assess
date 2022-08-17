#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
using namespace cv;
using namespace std;
class Check {//用来检测银行卡图片的类
public:
	Mat imageChuli(Mat& image);
	void FindCardRoi(Mat& image, vector<Mat>&cardNumber , vector<Rect>&Re);
	void FindMoban(Mat& image, vector<Mat>&mubiao);
	vector<int> avghash(Mat& image);
	int xiangsidu(vector<int> &has1, vector<int>&has2);
	vector< vector<int>> mobanHash(vector<Mat>&mubiao);
	void result(Mat& image, vector<Mat>& cardNumber, vector<Rect>& Re, vector< vector<int>>&mbHash);
};