#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void process(const char* ims)
{
	Mat img_in = imread(ims, CV_LOAD_IMAGE_COLOR);
	if (!img_in.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	imshow(ims, img_in);
	waitKey();

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	// Convertir l'image dans l'espace HSV
	Mat img_hsv;
	cvtColor(img_in, img_hsv, CV_BGR2HSV);


	// Modifie les canaux HSV par les paramètres
	for (int i=0; i < img_hsv.rows ; i++) {
		for (int j=0; j < img_hsv.cols; j++) {
			img_hsv.at<Vec3b>(i,j)[1] = 255;
			img_hsv.at<Vec3b>(i,j)[2] = 255;
		}
	}

	// Sauvegarde la nouvelle image imd
	cvtColor(img_hsv, img_hsv, CV_HSV2BGR);

	imshow(ims, img_hsv);
	waitKey();

	inRange(img_hsv, Scalar(0, 210, 0), Scalar(255, 255, 255), img_hsv); //Threshold the image
	imshow(ims, img_hsv);

	waitKey();

	Scalar m = mean(img_hsv);
	cerr<<"mean= "<<m<<endl;

	if (m[0] <= 245){
		erode(img_hsv, img_hsv, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
		dilate( img_hsv, img_hsv, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

		imshow(ims,img_hsv );
		waitKey();

		Mat canny_output;
		int thresh = 100;
		/// Detect edges using canny
		Canny(  img_hsv, canny_output, thresh, thresh*2, 3 );
		imshow(ims,canny_output);
		waitKey();

		/// Find contours
		findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// Draw contours
		unsigned int max=0;
		int max_index=0;

		/// match close edges
		for(unsigned int i = 0; i< contours.size(); i++ ){
			drawContours(canny_output, contours, i, 255, 2, 8, hierarchy, 0, Point() );
		}

		imshow(ims,canny_output);
		waitKey();

		findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		for(unsigned int i = 0; i < contours.size(); i++ ){
			if(contours[i].size() > max){
				max = contours[i].size();
				max_index = i;
			}
		}
		drawContours(img_in, contours, max_index, 255, 2, 8, hierarchy, 0, Point());

		imshow(ims,img_in);
		waitKey();
	} else {
		cerr<< "tout est terrain"<<endl;
		copyMakeBorder(img_in, img_in, 2, 2, 2, 2, BORDER_CONSTANT, 255);
		imshow(ims, img_in);
		waitKey();
	}
}

void usage (const char *s)
{
	cout<<"Usage: "<<s<<" ims"<<endl;
	exit(EXIT_FAILURE);
}

#define param 1

int main (int argc, char* argv[])
{
	if (argc != (param+1))
		usage(argv[0]);
	process(argv[1]);
	return EXIT_SUCCESS;
}
