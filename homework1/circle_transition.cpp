#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void circle_transition(Mat image1, Mat image2);

/*int main()
{
	Mat image1 = imread("E:\\大礼包\\多媒体\\作业1多媒体（第一题题目更正）\\诺贝尔.jpg"); 
	Mat image2 = imread("E:\\大礼包\\多媒体\\作业1多媒体（第一题题目更正）\\lena.jpg");
	circle_transition(image1, image2);
	waitKey(0);
	return 0;
}*/

void circle_transition(Mat image1, Mat image2){
	double fps = 20;//视频压缩帧率
	CvSize size = cvSize(408, 408);//图片序列中每张图片的宽高
	CvVideoWriter *writer = cvCreateVideoWriter("circle_transition.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, size, true);
	int x = 0, y = 0;
	double r = 0;
	int x_max, y_max, r_max;
	x_max = image1.rows;
	y_max = image1.cols;
	r_max = int(sqrt(pow(float(x_max / 2), 2) + pow(float(y_max / 2), 2)));
	int t = 0;

	Mat result = image1;
	for (; t < r_max; t++) {
		for (x = max(0, x_max / 2 - t); x < min(x_max / 2 + t, x_max); x++) {
			uchar *data = image2.ptr<uchar>(x);
			uchar *new_data = result.ptr<uchar>(x);
			for (y = max(0, y_max / 2 - t); y < min(y_max / 2 + t, y_max); y++) {
				r = sqrt(pow(float(x - x_max / 2), 2) + pow(float(y - y_max / 2), 2));
				if (r <= t) {
					//如果在中心正方形的内切圆内，绘制新图片
					new_data[y * 3] = data[y * 3];
					new_data[y * 3 + 1] = data[y * 3 + 1];
					new_data[y * 3 + 2] = data[y * 3 + 2];
				}
			}
		}
		cvWriteFrame(writer, &IplImage(result));
	}

	for (int i = 0; i < 20; i++){
		//最后再加20帧的图片，因为int float转换有一部分不能完全转换
		cvWriteFrame(writer, &IplImage(image2));
	}
}