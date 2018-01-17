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
labelingColor(Mat ims)
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

	int l=0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			int p = i*cols+j;
			if(roots[p]==p)
				roots[p] = l++;
			else
				roots[p] = roots[roots[p]];
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
	int max_l = 0;
	for(int i=1; i<l; i++){
		if (roots_size[max_l] < roots_size[i])
			max_l = i;
		cout<<roots_size[i]<<endl;

	}

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			if (roots[i*cols+j] > 0){
				if (roots[i*cols+j] == max_l){
					img_color.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
				} else {
					if (roots_size[roots[i*cols+j]] < 10){

					} else {
						img_color.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
					}
				}
			}
		}
	}

	//imshow("colorname", img_color);
	cout<<"labeling: "<< l << " components detected"<<endl;
	delete [] roots;
	delete [] roots_size;

	return img_color;
}



void process(const char* ims)
{
	Mat img_in = imread(ims, CV_LOAD_IMAGE_COLOR);
	if (!img_in.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	imshow(ims, img_in);
	waitKey();

	unsigned int size = img_in.rows * img_in.cols;
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
	inRange(img_hsv, Scalar(0, 210, 0), Scalar(255, 255, 255), img_hsv); //Threshold the image
	imshow(ims, img_hsv);
	waitKey();


	Mat field = labelingColor(img_hsv);

	erode(field, field, getStructuringElement(MORPH_ELLIPSE, Size(20, 10)) );
	dilate( field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

	dilate( field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
	erode(field, field, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

	//	cout<<size<<endl;
	imshow(ims, field );
	waitKey();

	
	Mat canny_output;
	int thresh = 100;
	
	/// Detect edges using canny
	Canny(  field, canny_output, thresh, thresh*2, 3 );
	//	imshow(ims,canny_output);
	//waitKey();
	
	
	
	/// Find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );  
	/// Draw contours
	unsigned int max=0;
	unsigned int max_2=0;
	int max_index=0;
	int max_index_2=0;
	
	
	  
	/// match close edges
	for(unsigned int i = 0; i< contours.size(); i++ ){
	  drawContours(canny_output, contours, i    ,255 , 2, 8, hierarchy, 0, Point() );
	}
	
	//	imshow(ims,canny_output);
	//	waitKey();
	
	//find contours
	findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );  
	
	//detect border contours
	unsigned int border_x=0,border_y=0;
       	for(unsigned int i = 0; i< contours.size(); i++ )
	  {
	    for (unsigned int j = 0; j< contours[i].size(); j++ )
	      {	cerr<<"contour"<<i<<","<<j<<" : x="<<contours[i][j].x<<",y="<<contours[i][j].y<<endl;
		if(contours[i][j].x==1) border_x++;
		if((contours[i][j].y==1)||(contours[i][j].y== canny_output.cols))border_y++; 
	      }
	    if( (contours[i].size()>max)&& (border_x>0)&&(border_y>0))
	      {
		max =contours[i].size();
		max_index=i;
		
		
	      }
	    border_x=0,border_y=0;
	  }
	
	if((max_index!=0)&&(field.at<Vec3b>(0,0) == Vec3b(0,0,0))){
	  drawContours( img_in, contours, max_index,255 , 2, 8, hierarchy, 0, Point() );
	  
	  imshow(ims,canny_output);
	  waitKey();
	  imshow(ims,img_in);
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
