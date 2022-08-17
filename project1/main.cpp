#include<iostream>
#include<opencv2/opencv.hpp>
#include<checkCard.h>
#include<vector>
#include<algorithm>

using namespace std;
using namespace cv;
int main() {
	Check demo;
	Mat m1 = imread("src/ocr_a_reference.png");//��ȡģ��
	Mat dst;
	vector<Mat>mubiao(10);
	vector< vector<int>>mbHash;
	dst = ~m1;//��m1��ת��������ȡģ��
	demo.FindMoban(dst, mubiao);//��10��ģ�崢������
	mbHash = demo.mobanHash(mubiao);//�����ÿ��ģ��Ĺ�ϣ���浽һ���������棬�Ѿ���ȡ��ģ���һ����Ϣ
	//��ȡ���п������ҷŵ�vector
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
		demo.FindCardRoi(super[i], cardNumber, Re);//�����п��з��ϵ�ͼ��ָ�����������������Ϣ��¼
		demo.result(super[i], cardNumber, Re, mbHash);//�������Ĺ�ϣ��Ƚϣ�����������϶ȵ�ֱ��д��ͼ���ϡ�
		string str ="card_"+ to_string(i + 1);//��ʾ
		imshow(str, super[i]);
	}
	waitKey(0);
	return 0;
}