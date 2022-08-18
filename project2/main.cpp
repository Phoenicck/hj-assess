#include<iostream>
#include<opencv2/opencv.hpp>
#include<shoot.h>
using namespace std;
using namespace cv;

int main() {
	Mat src = imread("src/high/7.jpg");
	CheckJiaban demo;
	Mat jiaozheng = demo.jiaozheng(src);
	Mat chuli= demo.chuli(jiaozheng);
	demo.findJiban(chuli,src);
	imshow("src", src);
	waitKey(0);
	return 0;
}