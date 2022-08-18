#include<iostream>
#include<opencv2/opencv.hpp>
#include<shoot.h>
using namespace std;
using namespace cv;

//目前先拿7号作为测试，也是现在比较好的一个测试结果
int main() {
	Mat src = imread("src/high/7.jpg");
	CheckJiaban demo;
	Mat jiaozheng = demo.jiaozheng(src);
	Mat chuli= demo.chuli(jiaozheng);
	vector<RotatedRect>light= demo.findLight(chuli,src);
	demo.findJiban(light,src);
	imshow("src", src);
	waitKey(0);
	return 0;
}