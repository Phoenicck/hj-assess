#include<checkCard.h>
#include<opencv2/dnn.hpp>
#include <string>
using namespace cv;
using namespace std;


Mat Check::erzhihua(Mat& image) {
	Mat dst;
	cvtColor(image, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 160, 255, THRESH_BINARY);//二值化
	return dst;
}

void Check::findmubiao(Mat& image) {//将轮廓找出来
	Mat dst=erzhihua(image);
	vector <vector<Point>> arr;
	vector<Vec4i> hiera;
	vector<Mat>mubiao;
	findContours(dst, arr, hiera, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//找轮廓
	vector <vector<Point>> conploy(arr.size());
	vector<Rect>re((arr.size()));
	for (int i = 0; i < arr.size(); i++) {
		float peri = arcLength(arr[i], true);
		approxPolyDP(arr[i], conploy[i], peri * 0.02, true);
		re[i] = boundingRect(conploy[i]);//最大矩形
		if (re[i].area()> 300&&re[i].area()<550) {
			Mat temp = image(re[i]);
			mubiao.push_back(temp);
		}
		//rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
	}

	imshow("src", image);
	imshow("dst", dst);
	/*imshow("mubiao", mubiao[2]);
	cout << mubiao.size() << endl;
	cout <<"width: " << mubiao[5].cols << endl;
	cout << "heigth: " << mubiao[5].rows << endl;*/
	//return mubiao;
}

void Check::moban(Mat& image) {//将轮廓找出来
	Mat temp = ~image;
	Mat dst = erzhihua(temp);
	vector < vector<Point>> arr;
	vector<Mat>mubiao(10);

	findContours(dst, arr, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//找轮廓
	vector < vector<Point>> conploy(arr.size());
	vector<Rect>re((arr.size()));

	
	for (int i = 0; i < arr.size(); i++) {
		re[i] = boundingRect(arr[i]);//最大矩形
		rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));//画矩形
	}
	int paixu[10];//将各个矩形的左上角进行排序
	for (int i = 0; i < 10; i++) {
		paixu[i]=re[i].tl().x;
	}
	sort(paixu, paixu+10);
	//for (int i = 0; i < 10; i++) {//将排序结果打印
	//	cout << paixu[i] << endl;
	//}

	for (int i = 0; i < re.size(); i++) {
		for (int j = 0; j < 10; j++) {
			if (re[i].tl().x == paixu[j]) {
				Mat temp = image(re[i]);
				mubiao[j] = temp;
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		int x = re[i].tl().x;
		int y = re[i].tl().y;
		string str = to_string(10-i);
		putText(image, str, Point(x, y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
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
