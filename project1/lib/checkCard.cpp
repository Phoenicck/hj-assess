#include<checkCard.h>
#include<opencv2/dnn.hpp>
#include <string>
#include<vector>
using namespace cv;
using namespace std;

Mat Check::imageChuli(Mat& image) {//ͼ��Ԥ����
	Mat imgGray,imgThre,imgBi,imgdilate;
	cvtColor(image, imgGray, COLOR_BGR2GRAY);//תΪ�Ҷ�
	threshold(imgGray, imgThre, 160, 255, THRESH_BINARY);//��ֵ��
	//���ж�ñ���� ��̫���ȡ��
	//����card05�޴�����
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(19, 19));
	morphologyEx(imgThre,imgBi, MORPH_TOPHAT, kernel1);
	//�ٽ������ͣ�Ϊ�˰�ͼ�������Եø�����
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgBi, imgdilate, kernel2);
	return imgdilate;
}

//�ҳ�card�е�Ŀ�����򣬲��ҽ���ͼ����ȡ������¼��Ŀ������ľ���λ��
void Check::FindCardRoi(Mat& image, vector<Mat>& cardNumber, vector<Rect>& Re) {
	Mat dst = imageChuli(image);//Ԥ����

	vector <vector<Point>> arr;
	findContours(dst, arr, RETR_EXTERNAL, CHAIN_APPROX_TC89_L1);//������
	vector <vector<Point>> conploy(arr.size());
	vector<Rect> re(arr.size());
	for (int i = 0; i < arr.size(); i++) {
		float peri = arcLength(arr[i], true);
		approxPolyDP(arr[i], conploy[i], peri * 0.02, true);//������ϣ��������ٵ�
		re[i] = boundingRect(conploy[i]);//������
		//��������
		double a = re[i].height;
		double b = re[i].width;
		double bili = a / b;
		//Ŀ����y�����ϵ�ɸѡ
		double c = image.rows;
		double e = 0.4 * c;
		double f = 0.7 * c;
		//cout << bili << endl;//���Ŀ�����򳤿����
		// �ֶ��Ҹ���ͼƬ��Ŀ��roi�������v0.4
		//card1 300 550
		//card2 715 900
		//card3 200 290
		//card4 
		//card5 220 330

		//��������ͳ���Ƚ��м�ɸѡ
		if (re[i].area() > 200 && re[i].area() < 1000 && bili > 1.4 && bili < 2.1) {
			
			double d = re[i].tl().y;
			//����y�������ɸѡ
			if (d > e && d < f) {
				Mat temp = image(re[i]);
				rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
				cardNumber.push_back(temp);//���ָ���ͼ�񴢴�
				Re.push_back(re[i]);//��¼Ŀ�������λ��
			}
		}
		//rectangle(image, re[i].tl(), re[i].br(), Scalar(255, 0, 255));
	}
}

//��ģ��ĸ�������ͼ���¼�����ҽ�������
//��Ϊģ�����ֺ�ɫ��ʹ��ǰ���ȷ�תһ�£��ٴ���
//���ظ������ֵķָ�����
void Check::FindMoban(Mat& image, vector<Mat>&mubiao) {
	Mat dst = imageChuli(image);
	vector <vector<Point>> arr;
	findContours(dst, arr, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//������
	vector <vector<Point>> conploy(arr.size());
	vector<Rect>rec((arr.size()));
	
	Mat imgRGB = ~image;//��ת������������ʾ
	for (int i = 0; i < arr.size(); i++) {
		rec[i] = boundingRect(arr[i]);//������
		rectangle(imgRGB, rec[i].tl(), rec[i].br(), Scalar(255, 0, 255));//�������򻭾���
	}
	int paixu[10];//���������ε����Ͻǵ��������
	for (int i = 0; i < 10; i++) {
		paixu[i]=rec[i].tl().x;
	}
	sort(paixu, paixu+10);
	//for (int i = 0; i < 10; i++) {//����������ӡ
	//	cout << paixu[i] << endl;
	//}
	//�ٽ�ͼ������������
	for (int i = 0; i < rec.size(); i++) {
		for (int j = 0; j < 10; j++) {
			if (rec[i].tl().x == paixu[j]) {
				Mat temp = image(rec[i]);
				mubiao[j] = temp;
			}
		}
	}
	
	//����ǽ�ģ���������ʾ������9-i����Ϊͼ�񾭹���ת
	for (int i = 0; i < 10; i++) {
		int x = rec[i].tl().x;
		int y = rec[i].tl().y;
		string str = to_string(9-i);//תΪstring����
		putText(imgRGB, str, Point(x, y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
	}
	imshow("moban", imgRGB);
	//imshow("mobandst", dst);
	
}

//���þ�ֵ��ϣ�㷨����ƥ��
//�����������ȡÿ��ͼ��Ĺ�ϣ���ϸ���˵���ǣ����ϣ��

vector<int> Check::avghash(Mat& image) {
	Mat dst,gray_dst;
	resize(image, dst, Size(16, 16));//����СΪ16*16��ԭ����8*8�����Ȳ���
	cvtColor(dst, gray_dst, COLOR_BGR2GRAY);
	//ͳ�����ص���ܺͣ�����ƽ��ֵ
	float sum = 0;
	for (int row = 0; row < 16; row++) {
		for (int col = 0; col < 16; col++)
		{
			sum += gray_dst.at<uchar>(col, row);
		}
	}
	float avg = sum / 256;//��ֵ
	
	//cout << sum / 64 << endl;
	//���ϣ����㣬�Ⱦ�ֵ����Ϊ1������Ϊ0
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
	return ahash;//�������ϣ��
}
//��������ͼ������ƶ�
int Check::xiangsidu(vector<int> &has1, vector<int>&has2) {
	int n=0;
	for (int i = 0; i < 256; i++) {
		if (has1[i] == has2[i]) {
			n++;
		}
	}
	return n;
}

//����ģ������ϣ�����ҽ�10��ȫ�����棬���Է�������Ϊvector< vector<int>>
vector< vector<int>> Check::mobanHash(vector<Mat>& mubiao)//��ģ���hash���������
{
	vector< vector<int>>mbhash;
	for (int i = 0; i < mubiao.size(); i++) {
		vector<int>temp;
		temp = avghash(mubiao[i]);
		mbhash.push_back(temp);
	}
	return mbhash;
}

//�����������ƶȱȽϣ�����һ������Ŀ�����������ģ��Ƚ�
//image��ԭͼ��cadnumber��Ŀ������Re��Ŀ�������λ�ã�mbhash ��ģ������ϣ��
void Check::result(Mat& image, vector<Mat>& cardNumber, vector<Rect>& Re, vector< vector<int>>&mbHash) {
	for (int i = 0; i < cardNumber.size(); i++) {
		vector<int> arr(10);
		vector<int>arr1 =avghash(cardNumber[i]);//��ȡ���п���Ŀ����������ϣ��
		//��ȡ���ƶ�
		for (int j = 0; j < 10; j++) {
			arr[j] =xiangsidu(mbHash[j], arr1);
		}
		//�ҵ��������ƶ�
		int maxPosition = max_element(arr.begin(), arr.end()) - arr.begin();
		/*int x = cardNumber[i].tl().x;
		int y = cardNumber[i].tl().y;*/
		cout << "�����Ƶ��ǣ�" << maxPosition << endl;
		int x = Re[i].tl().x;
		int y = Re[i].tl().y;
		string str = to_string(maxPosition);//�±�Ͷ�Ӧ����
		putText(image, str, Point(x, y), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 255, 0));
	}
	//imshow("resu", image);
}