#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<shoot.h>
using namespace std;
using namespace cv;

// ������ת���� 

void Drawrotedrec(Mat&src, RotatedRect& rotatedrect, const Scalar& color, int thickness)
{
	// ��ȡ��ת���ε��ĸ��ǵ�
	Point2f points[4];
	rotatedrect.points(points);

	// ��������
	vector<vector<Point>> Contours;    
	vector<Point> contours;
	for (int i = 0; i != 4; ++i) {
		contours.push_back(Point2i(points[i]));
	}
	Contours.push_back(contours);//�Ž�������������
	// ��������������ת����
	drawContours(src, Contours, 0, color, thickness);//�����ĸ��㻭����
}
//��ͼƬ���н���
Mat CheckJiaban::jiaozheng(Mat& image) {
	Mat CameraMatrix, newCameraMatrix, JibianMatrix;
	float CameraJuzhen[3][3] = {1576.70020,  0.000000000000,  635.46084,  0.000000000000,  1575.77707,  529.83878,  0.000000000000,  0.000000000000,  1.000000000000};
	CameraMatrix = Mat(3, 3, CV_32FC1,CameraJuzhen);//������þ���
	float JIbianJUzhen[1][5] = { -0.08325,  0.21277,  0.00022,  0.00033,  0 };
	JibianMatrix = Mat(1, 5, CV_32FC1, JIbianJUzhen);//�������
	newCameraMatrix = Mat(3, 3, CV_32FC1);
	/*Mat map1;
	Mat map2;*/
	Mat dst;
	/*initUndistortRectifyMap(CameraMatrix, JibianMatrix, newCameraMatrix,Size(1280,1024), CV_32FC1,map1,map2);
	remap(image, dst, map1, map2, INTER_LINEAR);*///��ת����Rδ֪

	undistort(image, dst, CameraMatrix, JibianMatrix);//���н���
	return dst;
	//imshow("jiaozhen", dst);
}


//��ͼƬ����Ԥ�����õ���ֵ��ͼƬ
Mat CheckJiaban::chuli(Mat& image) {
	Mat imgGray, imgRed,imgThre,imgBi,imgDil;
	//cvtColor(image, imgGray, COLOR_BGR2GRAY);
	vector<Mat> mv(3);
	split(image, mv);//ͨ������
	//for (int i = 0; i < 3; i++) {
	//	//string str = to_string(i+1);
	//	//namedWindow(str, 0);
	//	//imshow(str, mv[i]);//��ʾ����ͨ��
	//}
	mv[0] = 0, mv[1] = 0;
	merge(mv, imgRed);//�ϲ��õ���Ĳ���
	cvtColor(imgRed, imgGray, COLOR_BGR2GRAY);//תΪ�Ҷ�
	threshold(imgGray, imgThre, 50, 255,THRESH_BINARY);//��ֵ��
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgThre, imgDil, kernel2);//��������
	//Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(imgDil, imgBi, MORPH_TOPHAT, kernel1);//ȥ���Ӵ���
	//imshow("red", imgRed);
	//namedWindow("thre", 0);
	imshow("thre", imgDil);
	return imgDil;
}

//�ҵ������������仭��  �����ص�������С����
vector<RotatedRect> CheckJiaban::findLight(Mat& image,Mat&src) {
	vector<vector<Point>> contours;//�����ĵ㼯
	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//�������ҳ�
	//drawContours(src, contours, -1, Scalar(0, 255, 0));
	vector<vector<Point>> conploy(contours.size());//��ϵ㼯
	vector<RotatedRect> minrec(contours.size());//��С���εĵ㼯
	for (int i = 0; i < contours.size(); i++) {
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conploy[i], peri * 0.02, true);//���
		Rect re= boundingRect(contours[i]);
		double w = re.width;
		double h = re.height;
		double bili=h/w;
		if (bili > 2.5) {//��Ϊ�������������Ҳ��2.54
			minrec.push_back(minAreaRect(contours[i]));//����С���δ���
		}
	}
	//rectangle(src, Re[0], Scalar(0, 0, 255));
	//vector<Rect>re(Re.size());
	vector<RotatedRect> Minrec;//������������С����
	for (int i = 0; i <minrec.size(); i++) {
		Size2f tempwh= minrec[i].size;
		double w = tempwh.width;
		double h = tempwh.height;
		double bili1 = h/w;//����ӦΪ���ĳ�����ȡ����˳�����Գ������������й�ͨ�Ե�
		double bili2 = w/h;
		if (bili1 >2.5||bili2>2.5) {//
			//DrawRotatedRect(src, minrec[i], Scalar(0, 0, 255), 1, 8);
			//(src, minrec[i], Scalar(0, 0, 255));
			Minrec.push_back(minrec[i]);//���������ľ��η���
		}
	}
	imshow("dad", src);
	return Minrec;	
}

//���װ�ľ����ҳ�
void CheckJiaban::findJiban(vector<RotatedRect>& light,Mat&src) {
	Mat result = src.clone();
	if (light.size() == 0) {
		cout << "�޷�ʶ��װ�" << endl;
	}
	//
	vector<vec2RotatedRect>mubiao;//�Ƚ��б�
	for (int i = 0; i < light.size(); i++) {//һ����ͷ�ң�����һ����������
		for (int j=light.size() - 1; j>i; j--) {
			float temp = light[i].angle / light[j].angle;
			if (temp > 0.9 && temp < 1.1) {//�ȽϾ��ε���ת�Ƕȣ�����ֻ��������
				vec2RotatedRect	pipei(light[i],light[j]);
				mubiao.push_back(pipei);
			}
		}
	}
	//for (int i = 0; i < mubiao.size(); i++) {
	//	DrawRotatedRect(src, mubiao[i].arr[0], Scalar(0, 0, 255), 1, 8);//����ƥ��Ļ�����
	//	DrawRotatedRect(src, mubiao[i].arr[1], Scalar(0, 0, 255), 1, 8);
	//	/*Point2f center = (mubiao[i].arr[0].center + mubiao[i].arr[1].center)/2;*/
	//	//line(src, mubiao[i].arr[0].center, mubiao[i].arr[1].center, Scalar(255, 0, 255), 2);

	//	/*string str = "check_" + to_string(i);
	//	putText(src,str,)*/
	//}

	vector<double>area;
	for (int i = 0; i < mubiao.size(); i++) {

		vector<Point2i > heti;
		Point2f ps1[4];//��8�������

		//����4��
		mubiao[i].arr[0].points(ps1);
		for (int j = 0; j < 4; j++) {
			heti.push_back(ps1[j]);
		}
		Point2f ps2[4];
		mubiao[i].arr[1].points(ps2);
		for (int j = 0; j < 4; j++) {
			heti.push_back(ps2[j]);
		}
		
		RotatedRect re = minAreaRect(heti);
		Drawrotedrec(src, re, Scalar(0, 255, 0),2);
		double area_temp = re.size.width*re.size.height;
		area.push_back(area_temp);
	}
	int maxPosition = max_element(area.begin(), area.end()) - area.begin();
	//�ȸ���һ�ݿ����ܲ����ҵ����֮���ٸ�
	vector<Point2i > heti;
	Point2f ps1[4];//��8�������

	//����4��
	mubiao[maxPosition].arr[0].points(ps1);
	for (int j = 0; j < 4; j++) {
		heti.push_back(ps1[j]);
	}
	Point2f ps2[4];
	mubiao[maxPosition].arr[1].points(ps2);
	for (int j = 0; j < 4; j++) {
		heti.push_back(ps2[j]);
	}
	
	RotatedRect re = minAreaRect(heti);
	Drawrotedrec(src, re, Scalar(0,0, 255), 2);
	double area_temp = re.size.width * re.size.height;

	imshow("allJiaban", src);
	imshow("result", result);
};
