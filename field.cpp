#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



int
_find(int p, int* roots)
{
	while(roots[p] != p)
		p = roots[p];
	return p;
}

int
_union(int r0, int r1, int* roots)
{
	if(r0 == r1) return r0;
	if(r0 == -1) return r1;
	if(r1 == -1) return r0;
	if(r0 <	r1){
		roots[r1] = r0;
		return r0;
	}else{
		roots[r0]=r1;
		return r1;
	}
}

int
_add(int p, int r, int* roots)
{
	if(r==-1)
		roots[p]=p;
	else
		roots[p]=r;
	return roots[p];
}

Mat
labeling(Mat ims)
{

	Mat img_color = ims.clone();
	cvtColor(img_color, img_color, CV_GRAY2BGR);

	int* roots = new int[ims.total()];
	int rows = ims.rows;
	int cols = ims.cols;
	int p = 0;
	int r = -1;
	uchar* ps = ims.data;

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			r = -1;

			if( j>0 && (*(ps-1)==(*ps)) )
				r = _union( _find(p-1, roots), r, roots);

			if( (i>0 && j>0) && (*(ps-1-cols)==(*ps)) )
				r = _union( _find(p-1-cols, roots), r, roots);

			if( i>0 && (*(ps-cols) == (*ps)) )
				r = _union(_find(p-cols, roots), r, roots);

			if( (j<(cols-1) && i>0) && (*(ps+1-cols)==(*ps)) )
				r = _union(_find(p+1-cols, roots), r, roots);

			r = _add(p, r, roots);

			p++;
			ps++;
		}
	}

	for(p=0; p<rows*cols; p++){
		roots[p] = _find(p, roots);
	}

	int l=1;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			int p = i*cols+j;
			if (img_color.at<Vec3b>(i, j)==Vec3b(0, 0, 0))
				roots[p] = 0;
			else {
				if(roots[p]==p)
					roots[p] = l++;
				else
					roots[p] = roots[roots[p]];
			}
		}
	}

	int* roots_size = new int[l];
	for(int i=0; i<l; i++){
		roots_size[i]=0;
	}

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			int p = i*cols+j;
			roots_size[roots[p]]++;
		}
	}
	int max_l = 1;
	for(int i=1; i<l; i++){
		if (roots_size[max_l] < roots_size[i])
			max_l = i;
	}

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			if (roots[i*cols+j] > 0){
				if (roots[i*cols+j] == max_l){
					img_color.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				} else {
					img_color.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				}
			}
		}
	}

	//imshow("colorname", img_color);
	//cout<<"labeling: "<< l << " components detected"<<endl;
	delete [] roots;
	delete [] roots_size;

	return img_color;
}



void process(const char* ims, const char* ims_out)
{
	Mat img_in = imread(ims, CV_LOAD_IMAGE_COLOR);
	if (!img_in.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	imshow(ims, img_in);
	waitKey();
	Mat mask(img_in.rows, img_in.cols, CV_8UC3, Scalar(0, 0, 0));
	//	unsigned int size = img_in.rows * img_in.cols;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	// Convertir l'image dans l'espace HSV
	Mat img_hsv;
	cvtColor(img_in, img_hsv, CV_BGR2HSV);


	/*
	H: 0-180
	S: 0-255
	V: 0-255
	*/
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

	// Recuperer le vert dans l'image
	inRange(img_hsv, Scalar(0, 200, 0), Scalar(255, 255, 255), img_hsv); //Threshold the image
	imshow(ims, img_hsv);
	waitKey();


	Mat field = labeling(img_hsv);

	erode(field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
	dilate( field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

	cvtColor(field, field, CV_BGR2GRAY);
	field = labeling(field);

	dilate( field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
	erode(field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

	//cout<<size<<endl;
	imshow(ims, field );
	waitKey();

	Mat field_gray;
	cvtColor(field, field_gray, CV_BGR2GRAY);
	/// Find contours
	findContours( field_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	//
	/// Find the convex hull object for each contour
	vector<vector<Point> >hull( contours.size() );
	//Find the field
	unsigned int max=0,  max_index=0;
	for( unsigned int i = 0; i < contours.size(); i++ ) {
		if (contours[i].size()>max) {
			max =contours[i].size();
			max_index=i;
		}
	}
	//	convexHull( Mat(contours[max_index]), hull, false );
	for( unsigned int i = 0; i < contours.size(); i++ ) {
		convexHull( Mat(contours[i]), hull[i], false );
	}
	//	drawContours( mask, hull, max_index,255,2 , 8, vector<Vec4i>(), 0, Point() );
	fillConvexPoly(mask, hull[max_index], Scalar(255,255,255));

	drawContours( img_in, hull, max_index, Scalar(0,255,0), 2, 8, vector<Vec4i>(), 0, Point() );
	drawContours( mask, hull, max_index, Scalar(255,255,255), 2, 8, vector<Vec4i>(), 0, Point() );


	mask = mask + field;

	imshow(ims,img_in);
	waitKey();
	imshow(ims,mask);
	waitKey();
	imwrite(string("result/")+string(ims_out)+string(".png"),mask);
}


void usage (const char *s)
{
  cout<<"Usage: "<<s<<" ims ims_out"<<endl;
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
