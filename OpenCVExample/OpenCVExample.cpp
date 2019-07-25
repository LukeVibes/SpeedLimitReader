#ifdef _CH_
#pragma package <opencv>
#endif

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src;
Mat src_gray;

Mat bmp40;
Mat bmp80;
vector<vector<Point> > bmp_final;
vector<Vec4i> bmp_hierarchy;


Mat dst;
Mat edge;
Mat dst2;
vector<vector<Point> > countorResults;
vector<Vec4i> hierarchy;
RNG rng(12345);

String text = "unkown";
int stopsign = 0;

int canny_thresh = 120;
char* window_name = "Edge Map";


//Def: a simple function that using basic coordinate logic 
//     can correctly label the top right, top left, bottom right
//     bottom left corners of a rectangle shape.
void findOrder(vector<Point> points, Point2f* ordered) {
	double lowestSum = points[0].x + points[0].y;
	double highestSum = points[0].x + points[0].y;

	double highestDif = points[0].x - points[0].y;
	double lowestDif = points[0].x - points[0].y;

	Point2f topLeft;
	Point2f topRight;
	Point2f bottomLeft;
	Point2f bottomRight;

	for (int i = 0; i < points.size(); i++) {
		double sum = points[i].x + points[i].y;
		double dif = points[i].x - points[i].y;

		if (sum <= lowestSum) {
			lowestSum = sum;
			topLeft = points[i];
		}
		else if (sum >= highestSum) {
			highestSum = sum;
			bottomRight = points[i];
		}

		if (dif >= highestDif) {
			highestDif = dif;
			topRight = points[i];
		}
		else if (dif <= lowestDif) {
			lowestDif = dif;
			bottomLeft = points[i];
		}
	}
	ordered[0] = topLeft;
	ordered[1] = topRight;
	ordered[2] = bottomRight;
	ordered[3] = bottomLeft;
}

void CannyAndContoutIt(int, void*) {

	//Step One: Expose the edges
	Canny(dst, dst2, canny_thresh, canny_thresh * 2, 3);

	//Step Two: find the countors
	///IMPORTANT NOTE: I found that CHAIN_APPROX_SIMPLE reduces contour points too much, therefore i prefer  CHAIN_APPROX_NONE
	findContours(dst2, countorResults, hierarchy, RETR_LIST, CHAIN_APPROX_NONE, Point(0, 0));

	///some variables we will use later...
	vector<vector<Point> > contours_poly(countorResults.size());
	vector<Point> ourRec;
	vector<Rect> boundRect(countorResults.size());
	float largest = 0;
	float largest_i = -1;

	//Step Three: Make useable shapes out of contours + Find largest rectangle
	for (int i = 0; i < countorResults.size(); i++)
	{
		approxPolyDP(Mat(countorResults[i]), contours_poly[i], countorResults[i].size()*.008, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

		///finds largest rectangle, which is ourRec,
		if (largest < contourArea(Mat(contours_poly[i]))) {
			largest = contourArea(Mat(contours_poly[i]));
			if (contours_poly[i].size() == 4) {
				largest_i = i;
				cout << "big i: " << largest_i << endl;
				ourRec = contours_poly[i];
			}
		}


	}

	//Step Four: Draw contours
	Mat drawing = Mat::zeros(dst2.size(), CV_8UC3);
	for (int i = 0; i< countorResults.size(); i++)
	{
		float area = contourArea(Mat(contours_poly[i]));
		if (area < 0) { area = area * -1; } ///error proofing 
		if ((isContourConvex(Mat(contours_poly[i]))) && (area > 500)) {

			///Stop Sign Flag
			if (contours_poly[i].size() == 8 && largest_i == -1) {
				cout << "Stop sign \n";
				text = "Stop Sign";
				stopsign = 1;
			}


			///Speed Sign Flag
			else if ((contours_poly[i].size() == 4) && (i == largest_i)) {
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				drawContours(drawing, countorResults, i, color, 2, 8, hierarchy, 0, Point());
			}
		}
	}

	if (stopsign == 0) {
		RotatedRect box = minAreaRect(cv::Mat(ourRec)); ///convert to rectangle type


		///The correct way to order points 
		Point2f pts[4];
		findOrder(contours_poly[largest_i], pts);

		cout << pts[0] << "\n";
		cout << pts[1] << "\n";
		cout << pts[2] << "\n";
		cout << pts[3] << "\n";

		///WRONG LOGIC BUT WORKED BEFORE
		///pts[0] = contours_poly[largest_i][0];
		///pts[1] = contours_poly[largest_i][3];
		///pts[2] = contours_poly[largest_i][2];
		///pts[3] = contours_poly[largest_i][1];
		
		///0 1 2 4 <--no, just no
		///1 2 3 0  <-UPSIDE DOWN
		///3 0 1 2  <-RIGHT SIDE UP BUT FLIPPED
		///2 1 0 3 <-UPSIDE DOWN
		///0 3 2 1<---------------------THE ONE!

		///float widthA   = sqrt(pow((pts[2].x - pts[1].x), 2) + pow((pts[2].y - pts[1].y), 2));
		///float widthB   = sqrt(pow((pts[3].x - pts[0].x), 2) + pow((pts[3].y - pts[0].y), 2));
		///float maxWidth = max(widthA, widthB);

		///float heightA   = sqrt(pow((pts[3].x - pts[2].x), 2) + pow((pts[3].y - pts[2].y), 2));
		///float heightB   = sqrt(pow((pts[0].x - pts[1].x), 2) + pow((pts[0].y - pts[1].y), 2));
		///float maxHeight = max(heightA, heightB);

		

		//Step Five: WARP the image!
		///Because we actually want to warped image to be the same size as the bmp, we should adjust the
		///online resourse to do so
		///ALSO, YES it looks bad that we use bmp40 as it could be bmp80 but they are same size so its fine
		Point2f endpoint[4];

		endpoint[0] = Point2f(0, 0);
		endpoint[1] = Point2f(bmp40.cols - 1, 0);
		endpoint[2] = Point2f(bmp40.cols - 1, bmp40.rows - 1);
		endpoint[3] = Point2f(0, bmp40.rows - 1);

		Mat getP = getPerspectiveTransform(pts, endpoint); ///get the prespective difference...

		Mat warpedImg;
		warpPerspective(src_gray, warpedImg, getP, bmp40.size());  /// Transform the image to correctly fit the detection file, 
																   /// essentially unwarping it so you see the sign as if you 
																   /// were looking straight at it.

		imshow("Output", warpedImg);
		imshow(window_name, drawing);

		Mat comparer80;
		Mat comparer40;
		Mat normComparer;

		matchTemplate(bmp40, warpedImg, comparer40, TM_CCOEFF_NORMED); ///compare read in image with a 40km/hr sign
		cout << "40: " << comparer40 << "\n";

		matchTemplate(bmp80, warpedImg, comparer80, TM_CCOEFF_NORMED); ///compare read in image with a 80km/hr sign
		cout << "80: " << comparer80 << "\n";


		vector<float> vec40;
		comparer40.col(0).copyTo(vec40);

		vector<float> vec80;
		comparer80.col(0).copyTo(vec80);


		if ((vec40[0] + vec80[0]) < 0.8) { text = "uknown"; }
		else if (vec40[0] > vec80[0]) { text = "Speed 40"; }
		else { text = "Speed 80"; }

		putText(src, text, cvPoint(10, 130),
			FONT_HERSHEY_TRIPLEX, 1.5, cvScalar(153, 50, 204), 1, CV_AA);

		imshow("Final", src);
	}
	else {
		putText(src, text, cvPoint(10, 130),
			FONT_HERSHEY_TRIPLEX, 0.75, cvScalar(0, 0, 0), 1, CV_AA);

		imshow("Final", src);

	}

}

int main(){
	namedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Smoothed Image", CV_WINDOW_AUTOSIZE);
	namedWindow("Output", CV_WINDOW_AUTOSIZE);
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	namedWindow("Final", CV_WINDOW_AUTOSIZE);
	cout << "[windows made]" << "\n";

	//Step Zero: Load an image from file
	///5 0r 3 0r stop4
	///src = imread("C:/Users/iamro/Desktop/stop4.jpg", 1);
	///src = imread("C:/Users/iamro/Desktop/speedsign3.jpg", 1);
	///src = imread("C:/Users/iamro/Desktop/speedsign4.jpg", 1);
	src = imread("speedlimit/speedsign5.jpg", 1);
	///src = imread("C:/Users/iamro/Desktop/speedsign12.jpg", 1);
	///src = imread("C:/Users/iamro/Desktop/speedsign13.jpg", 1);
	///src = imread("C:/Users/iamro/Desktop/speedsign14.jpg", 1);
	bmp40 = imread("speedlimit/speed_40.bmp", CV_LOAD_IMAGE_UNCHANGED);
	bmp80 = imread("speedlimit/speed_80.bmp", CV_LOAD_IMAGE_UNCHANGED);
	
	cout << "[Images read]" << "\n";
	
	//Step One: Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);
	cout << "[color converted]" << "\n";
	///show the loaded image
	imshow("Original Image", src);
	cout << "[display og done]" << "\n";

	//Step Two: blur the image
	char zBuffer[35];
	blur(src_gray, dst, Size(3, 3));
	cout << "[blur done]" << "\n";
	imshow("Smoothed Image", dst);
	cout << "[display blur done]" << "\n";
	///Make new dst

	/// Create a Trackbar for user to enter threshold
	CannyAndContoutIt(0, 0);
	waitKey(0);

	return 0;
}

