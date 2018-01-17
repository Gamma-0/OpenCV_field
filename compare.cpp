#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void process(const char* ims, const char* ims2)
{
	Mat img_in = imread(ims, CV_LOAD_IMAGE_COLOR);
	if (!img_in.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}
	Mat img_in2 = imread(ims2, CV_LOAD_IMAGE_COLOR);
	if (!img_in2.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	unsigned size = img_in.rows * img_in.cols;
	unsigned error = 0;
	Mat img_diff = img_in & ~img_in2;
	for (int i = 0; i < img_in.rows; ++i){
		for (int j = 0; j < img_in.rows; ++j){
			if (img_diff.at<uchar>(i,j))
				error++;
		}
	}
	cout<<"Error percentage: "<<((float)error/(float)size)*100.0<<"%"<<endl;
}

void usage (const char *s)
{
	cout<<"Usage: "<<s<<" ims"<<endl;
	exit(EXIT_FAILURE);
}

#define param 2

int main (int argc, char* argv[])
{
	if (argc != (param+1))
		usage(argv[0]);
	process(argv[1], argv[2]);
	return EXIT_SUCCESS;
}
