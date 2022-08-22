#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include<shoot.h>
#include<math.h>
using namespace std;
using namespace cv;

float CameraJuzhen[3][3] = { 1576.70020,  0.000000000000,  635.46084,  0.000000000000,  1575.77707,  529.83878,  0.000000000000,  0.000000000000,  1.000000000000 };
Mat CameraMatrix = Mat(3, 3, CV_32FC1, CameraJuzhen);//相机内置矩阵
float JIbianJUzhen[1][5] = { -0.08325,  0.21277,  0.00022,  0.00033,  0 };
Mat JibianMatrix = Mat(1, 5, CV_32FC1, JIbianJUzhen);//畸变参数


// 绘制旋转矩形 

void CheckJiaban::Drawrotedrec(Mat& src, RotatedRect& rotatedrect, const Scalar& color, int thickness)
{
	// 提取旋转矩形的四个角点
	Point2f points[4];
	rotatedrect.points(points);

	// 构建轮廓
	vector<vector<Point>>Contours;
	vector<Point> contours;
	for (int i = 0; i != 4; ++i) {
		contours.push_back(Point2i(points[i]));
	}
	Contours.push_back(contours);//放进轮廓数组里面
	// 绘制轮廓，即旋转矩形
	drawContours(src, Contours, 0, color, thickness);//利用四个点画出来
}
//求两点距离的函数
float distance(const cv::Point2f& a, const cv::Point2f& b)
{
	float res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	return res;
}


//将图片进行矫正
Mat CheckJiaban::jiaozheng(Mat& image) {
	//方案一
	// Mat newCameraMatrix = Mat(3, 3, CV_32FC1);
	/*Mat map1;
	Mat map2;*/

	/*initUndistortRectifyMap(CameraMatrix, JibianMatrix, newCameraMatrix,Size(1280,1024), CV_32FC1,map1,map2);
	remap(image, dst, map1, map2, INTER_LINEAR);*///旋转矩阵R未知

	//方案二
	Mat dst;
	undistort(image, dst, CameraMatrix, JibianMatrix);//进行矫正
	return dst;
	//imshow("jiaozhen", dst);
}


//将图片进行预处理，得到二值化图片
Mat CheckJiaban::chuli(Mat& image) {
	Mat imgGray, imgRed, imgThre, imgBi, imgDil;
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
	threshold(imgGray, imgThre, 50, 255, THRESH_BINARY);//二值化
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgThre, imgDil, kernel2);//进行膨胀
	//Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	//morphologyEx(imgDil, imgBi, MORPH_TOPHAT, kernel1);//去除杂带点
	//imshow("red", imgRed);//仅仅为了调试可以显示
	//namedWindow("thre", 0);
	//imshow("thre", imgDil);
	return imgDil;
}
//11111111111111111111111111111111111111111111111111
//找到灯条，并将其画出  ，返回灯条的最小矩形
//vector<RotatedRect> CheckJiaban::findLight(Mat& image, Mat& src) {
//	vector<vector<Point>> contours;//轮廓的点集
//	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//将轮廓找出
//	//drawContours(src, contours, -1, Scalar(0, 255, 0));
//	vector<vector<Point>> conploy(contours.size());//拟合点集
//	vector<RotatedRect> minrec(contours.size());//最小矩形的点集
//	for (int i = 0; i < contours.size(); i++) {
//		float peri = arcLength(contours[i], true);
//		approxPolyDP(contours[i], conploy[i], peri * 0.02, true);//拟合
//		Rect re = boundingRect(contours[i]);
//		double w = re.width;
//		double h = re.height;
//		double bili = h / w;
//		if (bili > 2.5) {//因为灯条长宽比最少也是2.54
//			minrec.push_back(minAreaRect(contours[i]));//将最小矩形传入
//		}
//	}
//	//rectangle(src, Re[0], Scalar(0, 0, 255));
//	//vector<Rect>re(Re.size());
//	vector<RotatedRect> Minrec;//满足条件的最小矩形
//	for (int i = 0; i < minrec.size(); i++) {
//		Size2f tempwh = minrec[i].size;
//		double w = tempwh.width;
//		double h = tempwh.height;
//		double bili1 = h / w;//这是应为它的长宽提取是有顺序，所以长宽比与宽长比是有共通性的
//		double bili2 = w / h;
//		if (bili1 > 2.5 || bili2 > 2.5) {//
//			//Drawrotedrec(src, minrec[i], Scalar(0, 0, 255), 1);
//			//(src, minrec[i], Scalar(0, 0, 255));
//			Minrec.push_back(minrec[i]);//满足条件的矩形放入
//		}
//	}
//	//imshow("dad", src);//调试用的
//	return Minrec;
//}

//将甲板的矩形找出
//RotatedRect CheckJiaban::findJiban(vector<RotatedRect>& light, Mat& src) {
//	Mat result = src.clone();
//	if (light.size() == 0) {
//		cout << "无法识别甲板" << endl;
//	}
//	//
//	vector<vec2RotatedRect>mubiao;//比较列表
//	for (int i = 0; i < light.size(); i++) {//一个从头找，另外一个反方向找
//		for (int j = light.size() - 1; j > i; j--) {
//			float temp = light[i].angle / light[j].angle;
//			if (temp > 0.9 && temp < 1.1) {//比较矩形的旋转角度，精度只能试着来
//				vec2RotatedRect	pipei(light[i], light[j]);
//				mubiao.push_back(pipei);
//			}
//		}
//	}
//
//	vector<double>area;
//	vector<vector<Point2i>> heti(mubiao.size());
//	vector<RotatedRect>roatedrects(mubiao.size());
//	for (int i = 0; i < mubiao.size(); i++) {
//
//
//		Point2f ps1[4];//将8个点合体
//
//		//先是4个
//		mubiao[i].arr[0].points(ps1);
//		for (int j = 0; j < 4; j++) {
//			heti[i].push_back(ps1[j]);
//		}
//		Point2f ps2[4];
//		mubiao[i].arr[1].points(ps2);
//		for (int j = 0; j < 4; j++) {
//			heti[i].push_back(ps2[j]);
//		}
//
//		roatedrects[i] = minAreaRect(heti[i]);//对已经匹配成功的灯条画出他们的矩形
//		Drawrotedrec(src, roatedrects[i], Scalar(0, 255, 0), 2);//画出已检测的甲板
//		double area_temp = roatedrects[i].size.width * roatedrects[i].size.height;
//		area.push_back(area_temp);//因为把面积作为比较的因素，把面积储存
//	}
//	int maxPosition = max_element(area.begin(), area.end()) - area.begin();
//	//面积最大的装甲板被检测到
//	Drawrotedrec(src, roatedrects[maxPosition], Scalar(0, 0, 255), 2);//画出目标矩形
//	//imshow("allJiaban", src);//这两个显示为了将不同的区分开来
//	//imshow("result", result);
//	return roatedrects[maxPosition];
//};






//2222222222222222222222222222222222222222222
vector<RotatedRect> CheckJiaban::findLight(Mat& image, Mat& src) {
	vector<vector<Point>> contours;//轮廓的点集
	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//将轮廓找出
	//drawContours(src, contours, -1, Scalar(0, 255, 0));
	//vector<vector<Point>> conploy(contours.size());//拟合点集
	vector<RotatedRect> minrec;//最小矩形的点集
	for (int i = 0; i < contours.size(); i++) {
		if (contours.size() < 1) break;
		float light_area = contourArea(contours[i]);//面积筛选
		if (light_area < 100) continue;
		if (contours[i].size() < 6) continue;//满足椭圆拟合矩形
		RotatedRect light_minrec = fitEllipse(contours[i]);
		double a = light_minrec.size.width;
		double b = light_minrec.size.height;
		// double w = max(a, b);
		// double h = min(a, b);
		 double wh_bili = b/a;
		 if (wh_bili < 3) continue;
			minrec.push_back(minAreaRect(contours[i]));//将最小矩形传入
			//Drawrotedrec(src, light_minrec, Scalar(255, 0, 255), 1);
		
	}
	
	//imshow("dad", src);//调试用的
	//waitKey(0);
	return minrec;
}



//将甲板的矩形找出
RotatedRect CheckJiaban::findJiban(vector<RotatedRect>& light, Mat& src) {
	Mat result = src.clone();
	if (light.size() == 0) {
		cout << "无法识别甲板" << endl;
	}
	//
	vector<vec2RotatedRect>mubiao;//比较列表
	for (int i = 0; i < light.size(); i++) {//一个从头找，另外一个反方向找
		for (int j = light.size() - 1; j > i; j--) {
			float angle_gap =abs( light[i].angle - light[j].angle);
			float y_gap = abs(light[i].center.y - light[j].center.y);
			if (angle_gap < 5 || angle_gap>85) {
				if(y_gap<50){
					vec2RotatedRect	pipei(light[i], light[j]);
					mubiao.push_back(pipei);
					Drawrotedrec(src, light[i], Scalar(255, 0, 255), 1);
				}
			}	
		}
	}
	imshow("dad", src);
	//waitKey(0);
	vector<double>area;
	vector<vector<Point2i>> heti(mubiao.size());
	vector<RotatedRect>roatedrects(mubiao.size());
	for (int i = 0; i < mubiao.size(); i++) {


		Point2f ps1[4];//将8个点合体

		//先是4个
		mubiao[i].arr[0].points(ps1);
		for (int j = 0; j < 4; j++) {
			heti[i].push_back(ps1[j]);
		}
		Point2f ps2[4];
		mubiao[i].arr[1].points(ps2);
		for (int j = 0; j < 4; j++) {
			heti[i].push_back(ps2[j]);
		}

		roatedrects[i] = minAreaRect(heti[i]);//对已经匹配成功的灯条画出他们的矩形
		float bili1 = roatedrects[i].size.width/ roatedrects[i].size.height;
		//float bili2 = roatedrects[i].size.height / roatedrects[i].size.width;
		if (bili1 < 3 ) {
			Drawrotedrec(src, roatedrects[i], Scalar(0, 255, 0), 2);//画出已检测的甲板
			double area_temp = roatedrects[i].size.width * roatedrects[i].size.height;
			area.push_back(area_temp);//因为把面积作为比较的因素，把面积储存
		}
	}
	int maxPosition = max_element(area.begin(), area.end()) - area.begin();
	//面积最大的装甲板被检测到
	Drawrotedrec(src, roatedrects[maxPosition], Scalar(0, 0, 255), 2);//画出目标矩形
	//imshow("allJiaban", src);//这两个显示为了将不同的区分开来
	//imshow("result", result);
	return roatedrects[maxPosition];
};

//22222222222222222222222222222222222222222222222222


void CheckJiaban::zuobiao(RotatedRect& jiaban, Mat& src) {//应该选择已经匹配成功的一个灯条作为相机坐标转为世界坐标的模板，要不然，可能不是灯条
	//主要是甲板的四个点作为检测，这样就不用角点检测，
	//用甲板的尺寸作为转为世界坐标的依据
	Point2f ps[4];
	vector<Point2f>Ps;//这个为了pnp测距的时候用的，跟ps一样的
	jiaban.points(ps);//将四个点取下
	Point2f pmin = ps[0];//找到最高的点
	float maxDistance = 0;
	//找到最高的点，和最长的边
	for (int i = 0; i < 4; i++) {
		Ps.push_back(ps[i]);
		if (ps[i].y < pmin.y) {
			pmin = ps[i];
		}
		//cout << ps[i] << endl;//仅仅为了显示选择的点是否正确
		int j = i + 1;
		if (j == 4) {
			break;
		}
		else {
			float temp = distance(ps[i], ps[j]);
			//cout <<"四个点的长度： " << temp << endl;
			if (temp > maxDistance) {
				maxDistance = temp;
			}

		}
	}

	//cout <<"max is: " << pmin << endl;//仅仅为了显示选择的点是否正确
	Point2f newPs[4];//找到世界坐标
	float biliChi = 14 / maxDistance;//像素距离与实际的比例尺
	for (int i = 0; i < 4; i++) {
		newPs[i] = (ps[i] - pmin) * biliChi;//将最高的点作为原点，之后再按比例转换
		//cout << newPs[i] << endl;//仅仅为了显示选择的点是否正确
	}

	//cout <<"调整后" << distance(newPs[0], newPs[1]) << endl;仅为了显示
	//cout<<"调整后"<< distance(newPs[2], newPs[1]) << endl;

	vector<Point3f> objP;//将世界坐标放进数组
	for (int i = 0; i < 4; i++) {
		Point3f temp(newPs[i].x, newPs[i].y, 0);
		objP.push_back(temp);
		//cout <<"shijie: " << objP[i] << endl;
	}
	//这部分是求解pnp
	Mat rvecs = Mat::zeros(3, 1, CV_64FC1);//旋转向量
	Mat tvecs = Mat::zeros(3, 1, CV_64FC1);//平移向量
	solvePnP(objP, Ps, CameraMatrix, JibianMatrix, rvecs, tvecs);//求解pnp
	Mat rotM = Mat::eye(3, 3, CV_64F);//旋转矩阵
	Mat rotT = Mat::eye(3, 3, CV_64F);//平移矩阵

	//将旋转向量变换成旋转矩阵，平移也一样  //旋转角度不用算
	//Rodrigues(rvecs, rotM); 
	//Rodrigues(tvecs, rotT);
	//计算深度  //
	Mat P;
	P = (rotM.t()) * tvecs;
	//将深度输出
	cout << P << endl;
	cout << "diastance is:" << P.at<double>(2, 0) << "cm" << endl;
	string str = to_string(P.at<double>(2, 0)) + "cm";
	putText(src, str, jiaban.center, FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 255));
}