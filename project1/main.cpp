#include<iostream>
#include<opencv2/opencv.hpp>
#include<checkCard.h>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;
int main() {
	Mat m1 = imread("src/ocr_a_reference.png");
	Mat m2 = imread("src/credit_card_01.png");
	Mat dst;
	vector<Mat>mubiao(10);
	vector<Mat> cardNumber;

	vector<int>hash1;
	vector<int>hash2;
	vector< vector<int>>mbHash;
	Check demo;
	//demo.showContours(m1);
	//demo.findmubiao(m2);
	dst = ~m1;
	demo.findmoban(dst,mubiao);
	mbHash=demo.mobanHash(mubiao);
	vector<Rect>Re;
	demo.findcardnum(m2,cardNumber,Re);


	for (int i = 0; i < cardNumber.size(); i++) {
		vector<int> arr(10);
		vector<int>arr1 = demo.acgHash(cardNumber[i]);
		for (int j = 0; j < 10; j++) {
			arr[j] = demo.xiangsidu(mbHash[j], arr1);
		}
		int maxPosition = max_element(arr.begin(), arr.end()) - arr.begin();
		/*int x = cardNumber[i].tl().x;
		int y = cardNumber[i].tl().y;*/
		
		cout << "最相似的是：" << maxPosition << endl;
		int x = Re[i].tl().x;
		int y = Re[i].tl().y;
		string str = to_string(maxPosition);
		putText(m2, str, Point(x, y), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 255, 0));
	}
	//hash1 = demo.acgHash(cardNumber[5]);
	//hash2 = demo.acgHash(mubiao[7]);
	//int res = demo.xiangsidu(hash1, hash2);
	//demo.tezheng(mubiao[7], cardNumber[5]);
	//cout << "相似度为： " << res << endl;
	imshow("result", m2);
	waitKey(0);
	return 0;
}