#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<shoot.h>
using namespace std;
using namespace cv;

//Ŀǰ����7����Ϊ���ԣ�Ҳ�����ڱȽϺõ�һ�����Խ��
int main() {
	Mat src = imread("src/high/7.jpg");
	CheckJiaban demo;
	Mat jiaozheng = demo.jiaozheng(src);
	Mat chuli= demo.chuli(jiaozheng);
	vector<RotatedRect>light= demo.findLight(chuli,src);
	RotatedRect jiaban = demo.findJiban(light,src);
	demo.zuobiao(jiaban,src);//ѡ���Ѿ�ƥ��ĵƹ�����׼ȷ�Ժܸ�
	imshow("result", src);
	imwrite("output/7.jpg", src);
	waitKey(0);
	return 0;
}