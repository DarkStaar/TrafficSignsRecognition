
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

//Global variables
Mat imgCrop;
Scalar color = (0, 0, 0);

int hmin = 0;
int smin = 0;
int vmin = 0;
int hmax = 255;
int smax = 255;
int vmax = 255;

bool isRound = false; //Needed because using peri in approxPolyDP wont be enough we need 0.75

void getContours(Mat imgDil, Mat img)
{
    vector<vector<Point>> contours; // { {1, 2, 3}, {4, 7, 2}, ...}
    vector<Vec4i> hierarchy;    //Contains information about image topology
    String objectType;  //Based on number of conPoly size we define type of object we found
    

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Find any contours you can in picture, put info in contours and hierarchy
    
    vector<vector<Point>> conPoly(contours.size()); //
    vector<Rect> boundRect(contours.size());    //

    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]); //get area of contour(something like its strength)

        //cout << area << endl;
        //0.02*peri
        if (area > 1000)    //if the strength is low it means its noise and we dont care about it
        {
            float peri = arcLength(contours[i], true); //Find bounding box around objects/shapes, bool is used if object has closed shape or not
            if (isRound)
            {
                approxPolyDP(contours[i], conPoly[i], 0.75, true);
            }
            else
            {
                approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true); //Find number of curves that this polygon has and insert in conPoly, epsilon 0.02*peri is used for better precision
            }
            drawContours(img, conPoly, i, color, FILLED); //draw contours around edges of object we found

            cout << conPoly[i].size() <<endl;   //conPoly will have size that corresponds to how many curves it has

            boundRect[i] = boundingRect(conPoly[i]); //This just gets bounding box of those curves

            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 3); //Create reactangle around found object using bounding box of curves

            int objCor = (int)conPoly[i].size(); //See how many points does contour struct have (octagon 8, triangle 3...)

            Rect cropped(boundRect[i].tl(), boundRect[i].br());
            
            imgCrop = img(cropped);

            if (objCor == 8)
            {
                objectType = "Stop";
            }
            else 
            if (objCor > 4 && objCor != 8)
            {
                objectType = "Command";
            }
            if (objCor == 4)
            {
                objectType = "Informative";
            }
            else
            if (objCor == 3)
            {
                objectType = "Danger";
            }

            putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 1); //Write type on boundRects coordinates
        }
    }
}

// SET HSV VALUES BASED ON IMAGE
void findHSV(string filepath)
{
    if (filepath == "data/stop_sign.jpg")
    {
        hmin = 111;
        hmax = 179;
        smin = 68;
        smax = 255;
        vmin = 78;
        vmax = 255;

        color = (255, 0, 0);
    }

    if (filepath == "data/crosswalk.jpg")
    {
        hmin = 79;
        hmax = 179;
        smin = 64;
        smax = 255;
        vmin = 83;
        vmax = 255;

        color = (0, 255, 0);
    }

    if (filepath == "data/danger.jpg")
    {
        hmin = 0;
        hmax = 179;
        smin = 226;
        smax = 255;
        vmin = 50;
        vmax = 255;

        color = (0, 0, 255);
    }


    if (filepath == "data/highway_entry.jpg")
    {
        hmin = 45;
        hmax = 96;
        smin = 208;
        smax = 255;
        vmin = 127;
        vmax = 178;

        color = (125, 23, 190);
    }


    if (filepath == "data/highway_exit.jpg")
    {
        hmin = 0;
        hmax = 179;
        smin = 153;
        smax = 255;
        vmin = 113;
        vmax = 255;

        color = (13, 24, 255);
    }

    if (filepath == "data/no_parking_1.jpg")
    {
        hmin = 129;
        hmax = 179;
        smin = 52;
        smax = 253;
        vmin = 85;
        vmax = 255;

        isRound = true;

        color = (2, 15, 160);
    }

    if (filepath == "data/no_priority.jpg")
    {
        hmin = 0;
        hmax = 179;
        smin = 189;
        smax = 255;
        vmin = 85;
        vmax = 255;

        color = (120, 255, 66);
    }

    if (filepath == "data/one_way_try.jpg")
    {
        hmin = 0;
        hmax = 179;
        smin = 160;
        smax = 255;
        vmin = 101;
        vmax = 255;

        isRound = true;

        color = (110, 244, 110);
    }

    if (filepath == "data/parking.jpg")
    {
        hmin = 95;
        hmax = 179;
        smin = 219;
        smax = 255;
        vmin = 56;
        vmax = 255;

        color = (150, 124, 210);
    }

    if (filepath == "data/pedestrian_zone.jpg")
    {
        hmin = 95;
        hmax = 172;
        smin = 0;
        smax = 16;
        vmin = 146;
        vmax = 182;

        color = (161, 111, 245);
    }


    if (filepath == "data/priority_final.jpg")
    {
        hmin = 0;
        hmax = 179;
        smin = 40;
        smax = 137;
        vmin = 62;
        vmax = 142;

        color = (120, 224, 165);
    }


    if (filepath == "data/straight_ahead.jpg")
    {
        hmin = 41;
        hmax = 179;
        smin = 69;
        smax = 255;
        vmin = 139;
        vmax = 255;

        isRound = true;

        color = (79, 199, 210);
    }
}

//================IMAGES=======================//
Mat imgHSV, imgBlur, imgCanny, imgDil, mask;

int main() {
    string filepath = "data/highway_entry.jpg";
    Mat src = cv::imread(filepath);
	if(src.empty()){
		throw runtime_error("Cannot open image!");
	}
    
    
    findHSV(filepath);
    
    //Preprocessing
    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    /* //Uncomment only for checking HSV values
    namedWindow("Trackbars", (640, 200));
    createTrackbar("Hue Min", "Trackbars", &hmin, 179); //For hue its 179
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255); //For others is 255
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);
    */

    //while (true) //Uncomment this part only for checking HSV values in trackbar
    //{
        Scalar minimum(hmin, smin, vmin);
        Scalar maximum(hmax, smax, vmax);

        inRange(imgHSV, minimum, maximum, mask); //create Mask image that will have HSV values in given range
        Canny(mask, imgCanny, 25, 75);  //Perform Canny on that Mask

        Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

        dilate(imgCanny, imgDil, kernel); //Dilate cannied img with set kernel


        getContours(imgDil, src);   
        //imshow("hsv", imgHSV);
        //imshow("mask", mask);
        imshow("original", imgCrop);
        //waitKey(1);   //use with while
        waitKey(0);
    //}
    return 0;
}
