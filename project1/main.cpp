#include<iostream>
#include<opencv2/opencv.hpp>
#include<checkCard.h>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;
int main() {
	Check demo;
	Mat m1 = imread("src/ocr_a_reference.png");//读取模板
	Mat dst;
	vector<Mat>mubiao(10);
	vector< vector<int>>mbHash;
	dst = ~m1;//将m1反转，方便提取模板
	demo.FindMoban(dst, mubiao);//将10个模板储存起来
	mbHash = demo.mobanHash(mubiao);//将其的每个模板的哈希表储存到一个数组里面，已经获取了模板的一切信息
	//读取银行卡，并且放到vector
	Mat m2 = imread("src/credit_card_01.png");
	Mat m3 = imread("src/credit_card_02.png");
	Mat m4 = imread("src/credit_card_03.png");
	Mat m5 = imread("src/credit_card_04.png");
	Mat m6 = imread("src/credit_card_05.png");
	vector<Mat>super = { m2,m3,m4,m5,m6 };

	for (int i = 0; i < 5; i++) {
		vector<Mat> cardNumber;
		vector<int>hash1;
		vector<int>hash2;
		vector<Rect>Re;
		demo.FindCardRoi(super[i], cardNumber, Re);//将银行卡中符合的图像分割挑出来，将矩形信息记录
		demo.result(super[i], cardNumber, Re, mbHash);//进行最后的哈希表比较，并且最高契合度的直接写在图像上。
		string str ="card_"+ to_string(i + 1);//显示
		imshow(str, super[i]);
	}
	waitKey(0);
	return 0;
}