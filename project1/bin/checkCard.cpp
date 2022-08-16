#include<checkCard.h>
#include<opencv2/dnn.hpp>
#include <string>
#include<vector>
using namespace cv;
using namespace std;


Mat Check::erzhihua(Mat& image) {
	Mat dst;
	cvtColor(image, dst, COLOR_BGR2GRAY);
	threshold(dst, dst, 160, 255, THRESH_BINARY);//��ֵ��
	return dst;
}

//void Check::findcardnum(Mat& image, vector<Mat>&cardNumber) {//�������ҳ���
//	Mat dst=erzhihua(image);
//	vector <vector<Point>> arr;
//	vector<Vec4i> hiera;
//	
//	findContours(dst, arr, hiera, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//������
//	vector <vector<Point>> conploy(arr.size());
//	vector<Rect>re((arr.size()));
//	for (int i = 0; i < arr.size(); i++) {
//		float peri = arcLength(arr[i], true);
//		approxPolyDP(arr[i], conploy[i], peri * 0.02, true);
//		re[i] = boundingRect(conploy[i]);//������
//		if (re[i].area()> 300&&re[i].area()<550) {
//			Mat temp = image(re[i]);
//			cardNumber.push_back(temp);
//			rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
//		}
//		//rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
//	}
//
//	imshow("card", image);
//	imshow("carddst", dst);
//	/*imshow("mubiao", mubiao[2]);
//	cout << mubiao.size() << endl;
//	cout <<"width: " << mubiao[5].cols << endl;
//	cout << "heigth: " << mubiao[5].rows << endl;*/
//	//return mubiao;
//}
void Check::findcardnum(Mat& image, vector<Mat>&cardNumber, vector<Rect>&Re) {//�������ҳ���
	Mat dst=erzhihua(image);
	vector <vector<Point>> arr;
	vector<Vec4i> hiera;

	findContours(dst, arr, hiera, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//������
	vector <vector<Point>> conploy(arr.size());
	vector<Rect> re(arr.size());
	for (int i = 0; i < arr.size(); i++) {
		float peri = arcLength(arr[i], true);
		approxPolyDP(arr[i], conploy[i], peri * 0.02, true);
		re[i] = boundingRect(conploy[i]);//������
		if (re[i].area()> 300&&re[i].area()<550) {
			Mat temp = image(re[i]);
			cardNumber.push_back(temp);
			rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
			Re.push_back(re[i]);
		}
		//rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
	}

	//imshow("card", image);
	//imshow("carddst", dst);
	/*imshow("mubiao", mubiao[2]);
	cout << mubiao.size() << endl;
	cout <<"width: " << mubiao[5].cols << endl;
	cout << "heigth: " << mubiao[5].rows << endl;*/
	//return mubiao;
}



void Check::findmoban(Mat& image, vector<Mat>&mubiao) {//�������ҳ���
	Mat dst = erzhihua(image);
	vector < vector<Point>> arr;
	findContours(dst, arr, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//������
	vector < vector<Point>> conploy(arr.size());
	vector<Rect>rec((arr.size()));

	Mat imgRGB = ~image;
	for (int i = 0; i < arr.size(); i++) {
		rec[i] = boundingRect(arr[i]);//������
		rectangle(imgRGB, rec[i].tl(), rec[i].br(), Scalar(255, 0, 255));//������
	}
	int paixu[10];//���������ε����Ͻǽ�������
	for (int i = 0; i < 10; i++) {
		paixu[i]=rec[i].tl().x;
	}
	sort(paixu, paixu+10);
	//for (int i = 0; i < 10; i++) {//����������ӡ
	//	cout << paixu[i] << endl;
	//}
	
	for (int i = 0; i < rec.size(); i++) {
		for (int j = 0; j < 10; j++) {
			if (rec[i].tl().x == paixu[j]) {
				Mat temp = image(rec[i]);
				mubiao[j] = temp;
			}
		}
	}
	
	for (int i = 0; i < 10; i++) {
		int x = rec[i].tl().x;
		int y = rec[i].tl().y;
		string str = to_string(9-i);
		putText(imgRGB, str, Point(x, y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
	}
	imshow("moban", imgRGB);
	//imshow("mobandst", dst);
	
}

void Check::tezheng(Mat& img1, Mat& img2) {//�������������
	Mat dst1, dst2, img3;
	cvtColor(img1, dst1, COLOR_BGR2GRAY);
	cvtColor(img2, dst2, COLOR_BGR2GRAY);//��תΪ�Ҷ�
	Ptr<SIFT>fun_ptr = SIFT::create(100);//��������ָ��
	vector<KeyPoint>arr1, arr2;
	Mat dstsift1, dstsift2;
	fun_ptr->detectAndCompute(dst1, Mat(), arr1, dstsift1);//�Һͼ��������������
	fun_ptr->detectAndCompute(dst2, Mat(), arr2, dstsift2);
	
	BFMatcher machter(NORM_L1);
	vector<DMatch>macarr;
	machter.match(dstsift1, dstsift2, macarr);//���б���ƥ��
	drawMatches(img1, arr1, img2, arr2, macarr, img3);
	imshow("img3", img3);
};

vector<int> Check::acgHash(Mat& image) {
	Mat dst,gray_dst;
	resize(image, dst, Size(8, 8));//����С8*8
	cvtColor(dst, gray_dst, COLOR_BGR2GRAY);
	float sum = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++)
		{
			sum += gray_dst.at<uchar>(col, row);
		}
	}
	float avg = sum / 64;//��ֵ
	
	//cout << sum / 64 << endl;

	vector<int>ahash;
	for (int row = 0; row < 8; row++) {//���ϣ�����
		for (int col = 0; col < 8; col++)
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
	return ahash;//�������ϣ��
}

int Check::xiangsidu(vector<int> &has1, vector<int>&has2) {//�Ƚ����ƶ�
	int n=0;
	for (int i = 0; i < 64; i++) {
		if (has1[i] == has2[i]) {
			n++;
		}
		
	}
	return n;
}

vector< vector<int>> Check::mobanHash(vector<Mat>& mubiao)//��ģ���hash���������
{
	vector< vector<int>>mbhash;
	for (int i = 0; i < mubiao.size(); i++) {
		vector<int>temp;
		temp = acgHash(mubiao[i]);
		
		mbhash.push_back(temp);
	}
	return mbhash;
}