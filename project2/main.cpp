#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<shoot.h>
using namespace std;
using namespace cv;
//�Ѿ����Խ������ȵ�ȫ��ͼƬ����ʶ���޸�һ��·���Ϳ��Զ�ȡ�����ȵ�
//�����ȿ��ܻ�����һ�����Ϸ��ĵ���������Ӱ������ʶ��
//����������б��̫����ܻ�ʶ�����

int main() {

	for (int i =0; i <=47; i++) {
		string path = "src/high/" + to_string(i) + ".jpg";
		Mat src = imread(path);
		if (src.empty()) continue;
		cout << "ImgNo." << i << " is:" << endl;
		CheckJiaban demo;
		//Mat jiaozheng = demo.jiaozheng(src);//ѡ���Ƿ����
		Mat chuli = demo.chuli(src);
		vector<RotatedRect>light = demo.findLight(chuli, src);
		RotatedRect jiaban = demo.findJiban(light, src);
		demo.zuobiao(jiaban, src);//ѡ���Ѿ�ƥ��ĵƹ�����׼ȷ�Ժܸ�
		imshow("result", src);
		string path2="output/"+to_string(i) + ".jpg";
		imwrite(path2, src);
		waitKey(1000);//һ�����һ��
	}


	//�ⲿ�־Ϳ���������Բ鿴
	
	//Mat src = imread("src/low/57.jpg");
	//CheckJiaban demo;
	//Mat jiaozheng = demo.jiaozheng(src);
	//Mat chuli= demo.chuli(jiaozheng);
	//vector<RotatedRect>light= demo.findLight(chuli,src);
	//RotatedRect jiaban = demo.findJiban(light,src);
	//demo.zuobiao(jiaban,src);//ѡ���Ѿ�ƥ��ĵƹ�����׼ȷ�Ժܸ�
	//imshow("result", src);
	////imwrite("output/7.jpg", src);
	//waitKey(0);
	return 0;
}