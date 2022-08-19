#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<shoot.h>
#include<math.h>
using namespace std;
using namespace cv;

float CameraJuzhen[3][3] = { 1576.70020,  0.000000000000,  635.46084,  0.000000000000,  1575.77707,  529.83878,  0.000000000000,  0.000000000000,  1.000000000000 };
Mat CameraMatrix = Mat(3, 3, CV_32FC1, CameraJuzhen);//������þ���
float JIbianJUzhen[1][5] = { -0.08325,  0.21277,  0.00022,  0.00033,  0 };
Mat JibianMatrix = Mat(1, 5, CV_32FC1, JIbianJUzhen);//�������


// ������ת���� 

void CheckJiaban::Drawrotedrec(Mat&src, RotatedRect& rotatedrect, const Scalar& color, int thickness)
{
	// ��ȡ��ת���ε��ĸ��ǵ�
	Point2f points[4];
	rotatedrect.points(points);

	// ��������
	vector<vector<Point>>Contours;    
	vector<Point> contours;
	for (int i = 0; i != 4; ++i) {
		contours.push_back(Point2i(points[i]));
	}
	Contours.push_back(contours);//�Ž�������������
	// ��������������ת����
	drawContours(src, Contours, 0, color, thickness);//�����ĸ��㻭����
}
//���������ĺ���
float distance(const cv::Point2f& a, const cv::Point2f& b)
{
	float res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	return res;
}


//��ͼƬ���н���
Mat CheckJiaban::jiaozheng(Mat& image) {
	//����һ
	// Mat newCameraMatrix = Mat(3, 3, CV_32FC1);
	/*Mat map1;
	Mat map2;*/
	
	/*initUndistortRectifyMap(CameraMatrix, JibianMatrix, newCameraMatrix,Size(1280,1024), CV_32FC1,map1,map2);
	remap(image, dst, map1, map2, INTER_LINEAR);*///��ת����Rδ֪

	//������
	Mat dst;
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
	//imshow("red", imgRed);//����Ϊ�˵��Կ�����ʾ
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
			Drawrotedrec(src, minrec[i], Scalar(0, 0, 255),1);
			//(src, minrec[i], Scalar(0, 0, 255));
			Minrec.push_back(minrec[i]);//���������ľ��η���
		}
	}
	imshow("dad", src);//�����õ�
	return Minrec;	
}

//���װ�ľ����ҳ�
vector<vec2RotatedRect> CheckJiaban::findJiban(vector<RotatedRect>& light,Mat&src) {
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
	vector<vector<Point2i>> heti(mubiao.size());
	vector<RotatedRect>roatedrects(mubiao.size());
	for (int i = 0; i < mubiao.size(); i++) {

		
		Point2f ps1[4];//��8�������

		//����4��
		mubiao[i].arr[0].points(ps1);
		for (int j = 0; j < 4; j++) {
			heti[i].push_back(ps1[j]);
		}
		Point2f ps2[4];
		mubiao[i].arr[1].points(ps2);
		for (int j = 0; j < 4; j++) {
			heti[i].push_back(ps2[j]);
		}
		
		roatedrects[i] = minAreaRect(heti[i]);//���Ѿ�ƥ��ɹ��ĵ����������ǵľ���
		Drawrotedrec(src, roatedrects[i], Scalar(0, 255, 0),2);//�����Ѽ��ļװ�
		double area_temp = roatedrects[i].size.width* roatedrects[i].size.height;
		area.push_back(area_temp);//��Ϊ�������Ϊ�Ƚϵ����أ����������
	}
	int maxPosition = max_element(area.begin(), area.end()) - area.begin();
	//�������װ�װ屻��⵽
	Drawrotedrec(result, roatedrects[maxPosition], Scalar(0, 0, 255), 2);//����Ŀ�����
	imshow("allJiaban", src);//��������ʾΪ�˽���ͬ�����ֿ���
	imshow("result", result);
	return mubiao;
};

void CheckJiaban::zuobiao(RotatedRect& light) {//Ӧ��ѡ���Ѿ�ƥ��ɹ���һ��������Ϊ�������תΪ���������ģ�壬Ҫ��Ȼ�����ܲ��ǵ���
	//��Ҫ�ǶԵ������ĸ�����Ϊ��⣬�����Ͳ��ýǵ��⣬
	//�õ����ĳߴ���ΪתΪ�������������
	Point2f ps[4];
	vector<Point2f>Ps;//���Ϊ��pnp����ʱ���õģ���psһ����
	light.points(ps);//���ĸ���ȡ��
	Point2f pmin=ps[0];//�ҵ���ߵĵ�
	float maxDistance = 0;
	//�ҵ���ߵĵ㣬����ı�
	for (int i = 0; i < 4; i++) {
		Ps.push_back(ps[i]);
		if (ps[i].y < pmin.y) {
			pmin = ps[i];
		}
		cout << ps[i] << endl;//����Ϊ����ʾѡ��ĵ��Ƿ���ȷ
		int j = i + 1;
		if (j == 4) {
			break;
		}
		else {
			float temp = distance(ps[i], ps[j]);
			if (temp > maxDistance) {
				maxDistance = temp;
			}

		}
	}
	
	cout <<"max is: " << pmin << endl;//����Ϊ����ʾѡ��ĵ��Ƿ���ȷ
	Point2f newPs[4];//�ҵ���������
	float biliChi =14/maxDistance;//���ؾ�����ʵ�ʵı�����
	for (int i = 0; i < 4; i++) {
		newPs[i] = (ps[i] - pmin)*biliChi;//����ߵĵ���Ϊԭ�㣬֮���ٰ�����ת��
		cout << newPs[i] << endl;//����Ϊ����ʾѡ��ĵ��Ƿ���ȷ
	}

	vector<Point3f> objP;//����������Ž�����
	for (int i = 0; i < 4; i++) {
		Point3f temp(newPs[i].x,newPs[i].y, 0);
		objP.push_back(temp);
		cout <<"shijie: " << objP[i] << endl;
	}
	//�ⲿ�������pnp
	Mat rvecs = Mat::zeros(3, 1, CV_64FC1);//��ת����
	Mat tvecs = Mat::zeros(3, 1, CV_64FC1);//ƽ������
	solvePnP(objP,Ps,CameraMatrix,JibianMatrix,rvecs,tvecs);//���pnp
	Mat rotM = Mat::eye(3, 3, CV_64F);//��ת����
	Mat rotT = Mat::eye(3, 3, CV_64F);//ƽ�ƾ���

	//����ת�����任����ת����ƽ��Ҳһ��  //��ת�ǶȲ�����
	//Rodrigues(rvecs, rotM); 
	//Rodrigues(tvecs, rotT);
	//�������  //
	Mat P;
	P = (rotM.t()) * tvecs;
	//��������
	cout <<"diastance is:" << P.at<double>(2, 0) << "cm" << endl;
}