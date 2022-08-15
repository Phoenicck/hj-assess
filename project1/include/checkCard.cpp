#include<checkCard.h>
#include<opencv2/dnn.hpp>
using namespace cv;
using namespace std;

Mat Check::erzhihua(Mat& image) {
	Mat dst;
	cvtColor(image, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 160, 255, THRESH_BINARY);//二值化
	return dst;
}

void Check::showContours(Mat& image) {//将轮廓找出来
	Mat dst=erzhihua(image);
	vector < vector<Point>> arr;
	vector< Vec4i> hiera;
	
	findContours(dst, arr, hiera, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//找轮廓
	vector < vector<Point>> conploy(arr.size());
	vector<Rect>re((arr.size()));
	for (int i = 0; i < arr.size(); i++) {
		float peri = arcLength(arr[i], true);
		approxPolyDP(arr[i], conploy[i], peri * 0.02, true);
		re[i] = boundingRect(conploy[i]);//最大矩形
		rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
	}
	imshow("src", image);
	imshow("dst", dst);
}

void Check::tezheng(Mat& img1, Mat& img2) {
	Mat dst1, dst2, img3;
	cvtColor(img1, dst1, COLOR_BGR2GRAY);
	cvtColor(img2, dst2, COLOR_BGR2GRAY);
	Ptr<SIFT>fun_ptr = SIFT::create(500);
	vector<KeyPoint>arr1, arr2;
	
	Mat dstsift1, dstsift2;
	fun_ptr->detectAndCompute(dst1, Mat(), arr1, dstsift1);
	fun_ptr->detectAndCompute(dst2, Mat(), arr2, dstsift2);
	
	BFMatcher machter(NORM_L1);
	vector<DMatch>macarr;
	machter.match(dstsift1, dstsift2, macarr);
	drawMatches(img1, arr1, img2, arr2, macarr, img3);
	imshow("img3", img3);
	
};
