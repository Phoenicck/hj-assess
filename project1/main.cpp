#include<iostream>
#include<opencv2/opencv.hpp>
#include<checkCard.h>

using namespace std;
using namespace cv;
int main() {
	Mat m1 = imread("src/ocr_a_reference.png");
	Mat m2 = imread("src/credit_card_01.png");

	Check demo;
	demo.showContours(m1);
	demo.showContours(m2);
	demo.tezheng(m1, m2);
	waitKey(0);
	return 0;
}