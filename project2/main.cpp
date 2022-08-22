#include<iostream>
#include<opencv2/opencv.hpp>
#include<vector>
#include<shoot.h>
using namespace std;
using namespace cv;
//已经可以将高亮度的全部图片进行识别，修改一下路径就可以读取低亮度的
//高亮度可能会误判一下最上方的灯条，但不影响最后的识别
//低亮度中倾斜角太大可能会识别出错

int main() {

	for (int i =0; i <=47; i++) {
		string path = "src/high/" + to_string(i) + ".jpg";
		Mat src = imread(path);
		if (src.empty()) continue;
		cout << "ImgNo." << i << " is:" << endl;
		CheckJiaban demo;
		//Mat jiaozheng = demo.jiaozheng(src);//选做是否矫正
		Mat chuli = demo.chuli(src);
		vector<RotatedRect>light = demo.findLight(chuli, src);
		RotatedRect jiaban = demo.findJiban(light, src);
		demo.zuobiao(jiaban, src);//选择已经匹配的灯管这样准确性很高
		imshow("result", src);
		string path2="output/"+to_string(i) + ".jpg";
		imwrite(path2, src);
		waitKey(1000);//一秒更新一次
	}


	//这部分就可以逐个调试查看
	
	//Mat src = imread("src/low/57.jpg");
	//CheckJiaban demo;
	//Mat jiaozheng = demo.jiaozheng(src);
	//Mat chuli= demo.chuli(jiaozheng);
	//vector<RotatedRect>light= demo.findLight(chuli,src);
	//RotatedRect jiaban = demo.findJiban(light,src);
	//demo.zuobiao(jiaban,src);//选择已经匹配的灯管这样准确性很高
	//imshow("result", src);
	////imwrite("output/7.jpg", src);
	//waitKey(0);
	return 0;
}