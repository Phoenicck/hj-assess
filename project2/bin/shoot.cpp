#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<shoot.h>
using namespace std;
using namespace cv;


//将图片进行矫正
Mat CheckJiaban::jiaozheng(Mat& image) {
	Mat CameraMatrix, newCameraMatrix, JibianMatrix;
	float CameraJuzhen[3][3] = {1576.70020,  0.000000000000,  635.46084,  0.000000000000,  1575.77707,  529.83878,  0.000000000000,  0.000000000000,  1.000000000000};
	CameraMatrix = Mat(3, 3, CV_32FC1,CameraJuzhen);//相机内置矩阵
	float JIbianJUzhen[1][5] = { -0.08325,  0.21277,  0.00022,  0.00033,  0 };
	JibianMatrix = Mat(1, 5, CV_32FC1, JIbianJUzhen);//畸变参数
	newCameraMatrix = Mat(3, 3, CV_32FC1);
	/*Mat map1;
	Mat map2;*/
	Mat dst;
	/*initUndistortRectifyMap(CameraMatrix, JibianMatrix, newCameraMatrix,Size(1280,1024), CV_32FC1,map1,map2);
	remap(image, dst, map1, map2, INTER_LINEAR);*///旋转矩阵R未知

	undistort(image, dst, CameraMatrix, JibianMatrix);//进行矫正
	return dst;
	//imshow("jiaozhen", dst);
}


//将图片进行预处理，得到二值化图片
Mat CheckJiaban::chuli(Mat& image) {
	Mat imgGray, imgRed,imgThre,imgBi,imgDil;
	//cvtColor(image, imgGray, COLOR_BGR2GRAY);
	vector<Mat> mv(3);
	split(image, mv);//通道分离
	//for (int i = 0; i < 3; i++) {
	//	//string str = to_string(i+1);
	//	//namedWindow(str, 0);
	//	//imshow(str, mv[i]);//显示三个通道
	//}
	mv[0] = 0, mv[1] = 0;
	merge(mv, imgRed);//合并得到红的部分
	cvtColor(imgRed, imgGray, COLOR_BGR2GRAY);//转为灰度
	threshold(imgGray, imgThre, 50, 255,THRESH_BINARY);//二值化
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgThre, imgDil, kernel2);//进行膨胀
	//Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(imgDil, imgBi, MORPH_TOPHAT, kernel1);//去除杂带点
	

	//imshow("red", imgRed);
	//namedWindow("thre", 0);
	imshow("thre", imgDil);
	return imgDil;
}

void  CheckJiaban::findJiban(Mat& image,Mat&src) {
	vector<vector<Point>> contours;
	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(src, contours, -1, Scalar(0, 255, 0));
	vector<vector<Point>> conploy(contours.size());
	vector<Rect>Re(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conploy[i], peri * 0.02, true);
		Re[i] = boundingRect(contours[i]);
		
	}
	//rectangle(src, Re[0], Scalar(0, 0, 255));
	//vector<Rect>re(Re.size());
	for (int i = 0; i <Re.size(); i++) {
		double w = Re[i].width;
		double h = Re[i].height;
		double bili = h/w;
		if (bili >2.5) {
			rectangle(src, Re[i], Scalar(0, 0, 255));
		}
	}
	imshow("dad", src);
}