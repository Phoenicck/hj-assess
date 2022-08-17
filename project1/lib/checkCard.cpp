#include<checkCard.h>
#include<opencv2/dnn.hpp>
#include <string>
#include<vector>
using namespace cv;
using namespace std;

Mat Check::imageChuli(Mat& image) {//图像预处理
	Mat imgGray,imgThre,imgBi,imgdilate;
	cvtColor(image, imgGray, COLOR_BGR2GRAY);//转为灰度
	threshold(imgGray, imgThre, 160, 255, THRESH_BINARY);//二值化
	//进行顶帽运算 把太大的取掉
	//对于card05巨大作用
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(19, 19));
	morphologyEx(imgThre,imgBi, MORPH_TOPHAT, kernel1);
	//再进行膨胀，为了把图像轮廓显得更清晰
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgBi, imgdilate, kernel2);
	return imgdilate;
}

//找出card中的目标区域，并且进行图像提取，最后记录下目标区域的矩形位置
void Check::FindCardRoi(Mat& image, vector<Mat>& cardNumber, vector<Rect>& Re) {
	Mat dst = imageChuli(image);//预处理

	vector <vector<Point>> arr;
	findContours(dst, arr, RETR_EXTERNAL, CHAIN_APPROX_TC89_L1);//找轮廓
	vector <vector<Point>> conploy(arr.size());
	vector<Rect> re(arr.size());
	for (int i = 0; i < arr.size(); i++) {
		float peri = arcLength(arr[i], true);
		approxPolyDP(arr[i], conploy[i], peri * 0.02, true);//进行拟合，尽量减少点
		re[i] = boundingRect(conploy[i]);//最大矩形
		//比例区域
		double a = re[i].height;
		double b = re[i].width;
		double bili = a / b;
		//目标在y方向上的筛选
		double c = image.rows;
		double e = 0.4 * c;
		double f = 0.7 * c;
		//cout << bili << endl;//输出目标区域长款比例
		// 手动找各个图片中目标roi的面积，v0.4
		//card1 300 550
		//card2 715 900
		//card3 200 290
		//card4 
		//card5 220 330

		//基于面积和长宽比进行简单筛选
		if (re[i].area() > 200 && re[i].area() < 1000 && bili > 1.4 && bili < 2.1) {
			
			double d = re[i].tl().y;
			//基于y方向进行筛选
			if (d > e && d < f) {
				Mat temp = image(re[i]);
				rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
				cardNumber.push_back(temp);//将分割后的图像储存
				Re.push_back(re[i]);//记录目标区域的位置
			}
		}
		//rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
	}
}

//将模板的各个数字图像记录，并且进行排序
//因为模板数字黑色，使用前，先反转一下，再传入
//传回各个数字的分割数字
void Check::FindMoban(Mat& image, vector<Mat>&mubiao) {
	Mat dst = imageChuli(image);
	vector <vector<Point>> arr;
	findContours(dst, arr, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//找轮廓
	vector <vector<Point>> conploy(arr.size());
	vector<Rect>rec((arr.size()));
	
	Mat imgRGB = ~image;//反转回来，方便显示
	for (int i = 0; i < arr.size(); i++) {
		rec[i] = boundingRect(arr[i]);//最大矩形
		rectangle(imgRGB, rec[i].tl(), rec[i].br(), Scalar(255, 0, 255));//各个区域画矩形
	}
	int paixu[10];//将各个矩形的左上角点进行排序
	for (int i = 0; i < 10; i++) {
		paixu[i]=rec[i].tl().x;
	}
	sort(paixu, paixu+10);
	//for (int i = 0; i < 10; i++) {//将排序结果打印
	//	cout << paixu[i] << endl;
	//}
	//再将图像按排序结果储存
	for (int i = 0; i < rec.size(); i++) {
		for (int j = 0; j < 10; j++) {
			if (rec[i].tl().x == paixu[j]) {
				Mat temp = image(rec[i]);
				mubiao[j] = temp;
			}
		}
	}
	
	//这个是将模板的数字显示出来，9-i是因为图像经过反转
	for (int i = 0; i < 10; i++) {
		int x = rec[i].tl().x;
		int y = rec[i].tl().y;
		string str = to_string(9-i);//转为string类型
		putText(imgRGB, str, Point(x, y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
	}
	imshow("moban", imgRGB);
	//imshow("mobandst", dst);
	
}

//利用均值哈希算法进行匹配
//这个函数是提取每个图像的哈希表（严格来说不是，类哈希表）

vector<int> Check::avghash(Mat& image) {
	Mat dst,gray_dst;
	resize(image, dst, Size(16, 16));//先缩小为16*16，原本是8*8，精度不够
	cvtColor(dst, gray_dst, COLOR_BGR2GRAY);
	//统计像素点的总和，再求平均值
	float sum = 0;
	for (int row = 0; row < 16; row++) {
		for (int col = 0; col < 16; col++)
		{
			sum += gray_dst.at<uchar>(col, row);
		}
	}
	float avg = sum / 256;//均值
	
	//cout << sum / 64 << endl;
	//类哈希表计算，比均值大标记为1，否则为0
	vector<int>ahash;
	for (int row = 0; row < 16; row++) {
		for (int col = 0; col < 16; col++)
		{
			int xiangsu= gray_dst.at<uchar>(col, row);
			if (xiangsu > avg) {
				ahash.push_back(1);
			}
			else
			{
				ahash.push_back(0);
			}
		}
	}
	return ahash;//返回类哈希表
}
//返回两个图像的相似度
int Check::xiangsidu(vector<int> &has1, vector<int>&has2) {
	int n=0;
	for (int i = 0; i < 256; i++) {
		if (has1[i] == has2[i]) {
			n++;
		}
	}
	return n;
}

//计算模板的类哈希表，并且将10个全部储存，所以返回类型为vector< vector<int>>
vector< vector<int>> Check::mobanHash(vector<Mat>& mubiao)//把模板的hash表先算出来
{
	vector< vector<int>>mbhash;
	for (int i = 0; i < mubiao.size(); i++) {
		vector<int>temp;
		temp = avghash(mubiao[i]);
		mbhash.push_back(temp);
	}
	return mbhash;
}

//进行最后的相似度比较，传入一个卡的目标区域，逐个跟模板比较
//image：原图像，cadnumber：目标区域，Re：目标区域的位置，mbhash ：模板的类哈希表
void Check::result(Mat& image, vector<Mat>& cardNumber, vector<Rect>& Re, vector< vector<int>>&mbHash) {
	for (int i = 0; i < cardNumber.size(); i++) {
		vector<int> arr(10);
		vector<int>arr1 =avghash(cardNumber[i]);//获取银行卡的目标区域的类哈希表
		//获取相似度
		for (int j = 0; j < 10; j++) {
			arr[j] =xiangsidu(mbHash[j], arr1);
		}
		//找到最大的相似度
		int maxPosition = max_element(arr.begin(), arr.end()) - arr.begin();
		/*int x = cardNumber[i].tl().x;
		int y = cardNumber[i].tl().y;*/
		cout << "最相似的是：" << maxPosition << endl;
		int x = Re[i].tl().x;
		int y = Re[i].tl().y;
		string str = to_string(maxPosition);//下标就对应数字
		putText(image, str, Point(x, y), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 255, 0));
	}
	//imshow("resu", image);
}