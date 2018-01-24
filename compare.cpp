#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void process(const char* ims, const char* ims2)
{
	Mat img_in = imread(ims, 0);
	if (!img_in.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}
	Mat img_in2 = imread(ims2, 0);
	if (!img_in2.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	//unsigned size = img_in.rows * img_in.cols;
	unsigned nb_false_negative = 0;	// Nombre de pixels faux positif
	unsigned nb_false_positive = 0;	// Nombre de pixels faux negatif
	unsigned nb_white = 0, nb_black = 0; // Nombre de pixels censes etre positif ou negatif

	Mat img_false_negative = img_in & ~img_in2; // Image des faux negatifs
	Mat img_false_positive = ~(img_in | ~img_in2); // Image des faux positifs

	for (int i = 0; i < img_false_negative.rows; ++i){
		for (int j = 0; j < img_false_negative.cols; ++j){
			if (img_in.at<uchar>(i,j))
				nb_white++;
			else
				nb_black++;
			if (img_false_negative.at<uchar>(i,j))
				nb_false_negative++;
			if (img_false_positive.at<uchar>(i,j))
				nb_false_positive++;
		}
	}

	imshow("False negative", img_false_negative);
	imshow("False positive", img_false_positive);
	waitKey();

	cout<<"\n=> Test on file "<<ims2<<endl;
	if (nb_white)
		cout<<"Error percentage false negative: "<<((float)nb_false_negative/(float)nb_white)*100.0<<"%"<<endl;
	else
		cout<<"Error percentage false negative: None"<<endl;
	if (nb_black)
		cout<<"Error percentage false positive: "<<((float)nb_false_positive/(float)nb_black)*100.0<<"%"<<endl;
	else
		cout<<"Error percentage false positive: None"<<endl;

}

void usage (const char *s)
{
	cout<<"Usage: "<<s<<" im_true im_computed"<<endl;
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
