
#include "Enums.hpp"

//#include <visualizer.hpp>

#include <iostream>
#include <cstdlib>
using namespace std;

//#include <unistd.h>

//#define DEBUG(x) do{ std::cout << #x << " = " << x << std::endl; }while(0)

#include "opencv2/opencv.hpp"
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

//================IMAGES=======================//
Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;



void getContours(Mat imgCanny, Mat img)
{
    vector<vector<Point>> contours; // { {1, 2, 3}, {4, 7, 2}, ...}
    vector<Vec4i> hierarchy;    //Contains information about image topology
    String objectType;  //Based on number of conPoly size we define type of object we found
    

    findContours(imgCanny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Find any contours you can in picture, put info in contours and hierarchy
    
    vector<vector<Point>> conPoly(contours.size()); //
    vector<Rect> boundRect(contours.size());    //

    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]); //get area of contour(something like its strength)

        //cout << area << endl;

        if (area > 1000)    //if the strength is low it means its noise and we dont care about it
        {
            float peri = arcLength(contours[i], true); //Find bounding box around objects/shapes, bool is used if object has closed shape or not
            approxPolyDP(contours[i], conPoly[i], 0.02*peri, true); //Find number of curves that this polygon has and insert in conPoly, epsilon 0.02*peri is used for better precision
            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2); //draw contours around edges of object we found

            cout << conPoly[i].size() <<endl;   //conPoly will have size that corresponds to how many curves it has

            boundRect[i] = boundingRect(conPoly[i]); //This just gets bounding box of those curves

            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 3); //Create reactangle around found object using bounding box of curves

            int objCor = (int)conPoly[i].size(); //See how many points does contour struct have (octagon 8, triangle 3...)

            if (objCor == 8)
            {
                objectType = "Stop sign";
            }
            else 
            if (objCor > 4 && objCor != 8)
            {
                objectType = "Circle";
            }
            if (objCor == 4)
            {
                objectType = "Rectangle";
            }
            else
            if (objCor == 3)
            {
                objectType = "Triangle";
            }

            putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 1); //Write type on boundRects coordinates
        }
    }
}

int main() {
Mat src = cv::imread("data/stop_sign.jpg");
	if(src.empty()){
		throw runtime_error("Cannot open image!");
	}
	
    //Preprocessing
    cvtColor(src, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
    Canny(imgBlur, imgCanny, 25, 75);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    dilate(imgCanny, imgDil, kernel);


    getContours(imgCanny, src);

    imshow("image", src);
    waitKey(0);

	return 0;
}
