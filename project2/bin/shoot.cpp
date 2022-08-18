#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<shoot.h>
using namespace std;
using namespace cv;

// 绘制旋转矩形 

void Drawrotedrec(Mat&src, RotatedRect& rotatedrect, const Scalar& color, int thickness)
{
	// 提取旋转矩形的四个角点
	Point2f points[4];
	rotatedrect.points(points);

	// 构建轮廓
	vector<vector<Point>> Contours;    
	vector<Point> contours;
	for (int i = 0; i != 4; ++i) {
		contours.push_back(Point2i(points[i]));
	}
	Contours.push_back(contours);//放进轮廓数组里面
	// 绘制轮廓，即旋转矩形
	drawContours(src, Contours, 0, color, thickness);//利用四个点画出来
}
//将图片进行矫正
Mat CheckJiaban::jiaozheng(Mat& image) {
	Mat CameraMatrix, newCameraMatrix, JibianMatrix;
	float CameraJuzhen[3][3] = {1576.70020,  0.000000000000,  635.46084,  0.000000000000,  1575.77707,  529.83878,  0.000000000000,  0.000000000000,  1.000000000000};
	CameraMatrix = Mat(3, 3, CV_32FC1,CameraJuzhen);//相机内置矩阵
	float JIbianJUzhen[1][5] = { -0.08325,  0.21277,  0.00022,  0.00033,  0 };
	JibianMatrix = Mat(1, 5, CV_32FC1, JIbianJUzhen);//畸变参数
	newCameraMatrix = Mat(3, 3, CV_32FC1);
	/*Mat map1;
	Mat map2;*/
	Mat dst;
	/*initUndistortRectifyMap(CameraMatrix, JibianMatrix, newCameraMatrix,Size(1280,1024), CV_32FC1,map1,map2);
	remap(image, dst, map1, map2, INTER_LINEAR);*///旋转矩阵R未知

	undistort(image, dst, CameraMatrix, JibianMatrix);//进行矫正
	return dst;
	//imshow("jiaozhen", dst);
}


//将图片进行预处理，得到二值化图片
Mat CheckJiaban::chuli(Mat& image) {
	Mat imgGray, imgRed,imgThre,imgBi,imgDil;
	//cvtColor(image, imgGray, COLOR_BGR2GRAY);
	vector<Mat> mv(3);
	split(image, mv);//通道分离
	//for (int i = 0; i < 3; i++) {
	//	//string str = to_string(i+1);
	//	//namedWindow(str, 0);
	//	//imshow(str, mv[i]);//显示三个通道
	//}
	mv[0] = 0, mv[1] = 0;
	merge(mv, imgRed);//合并得到红的部分
	cvtColor(imgRed, imgGray, COLOR_BGR2GRAY);//转为灰度
	threshold(imgGray, imgThre, 50, 255,THRESH_BINARY);//二值化
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgThre, imgDil, kernel2);//进行膨胀
	//Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(imgDil, imgBi, MORPH_TOPHAT, kernel1);//去除杂带点
	//imshow("red", imgRed);
	//namedWindow("thre", 0);
	imshow("thre", imgDil);
	return imgDil;
}

//找到灯条，并将其画出  ，返回灯条的最小矩形
vector<RotatedRect> CheckJiaban::findLight(Mat& image,Mat&src) {
	vector<vector<Point>> contours;//轮廓的点集
	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//将轮廓找出
	//drawContours(src, contours, -1, Scalar(0, 255, 0));
	vector<vector<Point>> conploy(contours.size());//拟合点集
	vector<RotatedRect> minrec(contours.size());//最小矩形的点集
	for (int i = 0; i < contours.size(); i++) {
		float peri = arcLength(contours[i], true);
		approxPolyDP(contours[i], conploy[i], peri * 0.02, true);//拟合
		Rect re= boundingRect(contours[i]);
		double w = re.width;
		double h = re.height;
		double bili=h/w;
		if (bili > 2.5) {//因为灯条长宽比最少也是2.54
			minrec.push_back(minAreaRect(contours[i]));//将最小矩形传入
		}
	}
	//rectangle(src, Re[0], Scalar(0, 0, 255));
	//vector<Rect>re(Re.size());
	vector<RotatedRect> Minrec;//满足条件的最小矩形
	for (int i = 0; i <minrec.size(); i++) {
		Size2f tempwh= minrec[i].size;
		double w = tempwh.width;
		double h = tempwh.height;
		double bili1 = h/w;//这是应为它的长宽提取是有顺序，所以长宽比与宽长比是有共通性的
		double bili2 = w/h;
		if (bili1 >2.5||bili2>2.5) {//
			//DrawRotatedRect(src, minrec[i], Scalar(0, 0, 255), 1, 8);
			//(src, minrec[i], Scalar(0, 0, 255));
			Minrec.push_back(minrec[i]);//满足条件的矩形放入
		}
	}
	imshow("dad", src);
	return Minrec;	
}

//将甲板的矩形找出
void CheckJiaban::findJiban(vector<RotatedRect>& light,Mat&src) {
	Mat result = src.clone();
	if (light.size() == 0) {
		cout << "无法识别甲板" << endl;
	}
	//
	vector<vec2RotatedRect>mubiao;//比较列表
	for (int i = 0; i < light.size(); i++) {//一个从头找，另外一个反方向找
		for (int j=light.size() - 1; j>i; j--) {
			float temp = light[i].angle / light[j].angle;
			if (temp > 0.9 && temp < 1.1) {//比较矩形的旋转角度，精度只能试着来
				vec2RotatedRect	pipei(light[i],light[j]);
				mubiao.push_back(pipei);
			}
		}
	}
	//for (int i = 0; i < mubiao.size(); i++) {
	//	DrawRotatedRect(src, mubiao[i].arr[0], Scalar(0, 0, 255), 1, 8);//将已匹配的画出来
	//	DrawRotatedRect(src, mubiao[i].arr[1], Scalar(0, 0, 255), 1, 8);
	//	/*Point2f center = (mubiao[i].arr[0].center + mubiao[i].arr[1].center)/2;*/
	//	//line(src, mubiao[i].arr[0].center, mubiao[i].arr[1].center, Scalar(255, 0, 255), 2);

	//	/*string str = "check_" + to_string(i);
	//	putText(src,str,)*/
	//}

	vector<double>area;
	for (int i = 0; i < mubiao.size(); i++) {

		vector<Point2i > heti;
		Point2f ps1[4];//将8个点合体

		//先是4个
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
	//先复制一份看看能不能找到最大，之后再改
	vector<Point2i > heti;
	Point2f ps1[4];//将8个点合体

	//先是4个
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
