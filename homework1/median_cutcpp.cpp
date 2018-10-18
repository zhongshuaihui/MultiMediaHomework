#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace cv;
using namespace std;

struct pixel {
	int bgr[3];
};
struct block {
	int start, end;
	int Pixel_num;
};

vector<pixel> pixels;

void block_init(block& b) {
	int maxi[3] = { 0, 0, 0 }; // [b, g, r]
	int mini[3] = { 255, 255, 255 };
	int max_num = 0;

	pixel p;
	for (int i = b.start; i < b.end; i++) {
		p = pixels.at(i);

		mini[0] = min(mini[0], p.bgr[0]);
		maxi[0] = max(maxi[0], p.bgr[0]);
		mini[1] = min(mini[1], p.bgr[1]);
		maxi[1] = max(maxi[1], p.bgr[1]);
		mini[2] = min(mini[2], p.bgr[2]);
		maxi[2] = max(maxi[2], p.bgr[2]);
	}
	b.Pixel_num = b.end - b.start;
}

void getNewColor(int* color, const block& b) {
	double sumColor[] = { 0.0, 0.0, 0.0 };
	pixel p;
	for (int i = b.start; i < b.end; i++) {
		p = pixels.at(i);

		sumColor[0] += (double)p.bgr[0];
		sumColor[1] += (double)p.bgr[1];
		sumColor[2] += (double)p.bgr[2];
	}
	for (int i = 0; i < 3; i++) {
		sumColor[i] /= b.Pixel_num;
		color[i] = (int)sumColor[i];
	}
}

int main() {
	Mat image = imread("E:\\大礼包\\多媒体\\作业1多媒体（第一题题目更正）\\redapple.jpg");
	for (int i = 0; i < image.rows; ++i)
	{
		Vec3b* point = image.ptr<Vec3b>(i);
		for (int j = 0; j < image.cols; j++)
		{
			pixel p;
			p.bgr[0] = (int)point[j][0];
			p.bgr[1] = (int)point[j][1];
			p.bgr[2] = (int)point[j][2];

			pixels.push_back(p);
		}
	}

	queue<block> blocks;

	// 初始化 blocks
	block init;
	init.start = 0;
	init.end = pixels.size();
	blocks.push(init);
	for (int i = 0; i < 100000; i++)
	{
		block parent, block1, block2;

		parent = blocks.front();
		// 1. 初始化block
		block_init(parent);
		if (parent.Pixel_num < 5) {
			continue;
		}
		blocks.pop();

		// 2. 中间值
		int median_index = (int)((parent.end - parent.start + 1) / 2);

		// 3. 分开block
		block1.start = parent.start;
		block1.end = parent.start + median_index;
		block2.start = parent.start + median_index;
		block2.end = parent.end;

		block_init(block1);
		block_init(block2);

		blocks.push(block1);
		blocks.push(block2);
	}

	// 将每一个blck设为相同颜色
	for (int i = 0; i <blocks.size(); i++)
	{
		block drawblock = blocks.front();
		blocks.pop();

		int color[3] = { 0, 0, 0 };
		getNewColor(color, drawblock);

		for (int j = 0; j < drawblock.Pixel_num; j++) {
			for (int k = 0; k < 3; k++) {
				pixels.at(drawblock.start + j).bgr[k] = color[k];
			}
		}
	}

	// then draw the average colors
	Mat result_img(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
	for (int row = 0; row < image.rows; row++) {
		for (int col = 0; col < image.cols; col++)
		{
			if ((row * image.cols + col) < pixels.size()) {
				for (int k = 0; k < 3; k++) {
					result_img.at<Vec3b>(row, col).val[k] = (uchar)pixels.at(row * image.cols + col).bgr[k];
				}
			}

		}
	}
	imwrite("median_cut_apple.jpg", result_img);

	return 0;
}