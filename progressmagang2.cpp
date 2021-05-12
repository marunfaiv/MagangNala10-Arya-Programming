#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool compareContourAreas(vector<Point> contour1, vector<Point> contour2){
    double i = fabs(contourArea(Mat(contour1)) );
    double j = fabs(contourArea(Mat(contour2)) );

    return (i < j);
}

int main(){

    int hmin, smin, vmin, dilSize, erodSize, max_value1, max_value2, max_value3;
    int hmin_purp, smin_purp, vmin_purp;
    int hmax_purp, smax_purp, vmax_purp;
    hmin = 0; smin = 0; vmin = 0;
    max_value1 = 180;
    max_value2 = 255;
    max_value3 = 10;
    dilSize = 0;
    erodSize = 0;
    float x, y, error;
    x = 640.0/2.0;
    y = 480.0/2.0;
    error = 0;

    VideoCapture cap(-1);
    Mat img, imgHSV, imgDil, imgErode, mask;
    string mainwindow, window2,window3,window4;
    mainwindow = "Main Camera";
    window2 = "Masked";
    window3 = "Eroded";
    window4 = "Dilated";

    namedWindow(mainwindow);
    createTrackbar("Hue",mainwindow, &hmin, max_value1);
    createTrackbar("Saturation", mainwindow, &smin, max_value2);
    createTrackbar("Value", mainwindow, &vmin, max_value2);
    createTrackbar("Erode", mainwindow, &erodSize, max_value3);
    createTrackbar("Dilate", mainwindow, &dilSize, max_value3);

    while(true){
        
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hmin,smin,vmin), Scalar(max_value1,max_value2,max_value2), mask);

        Mat kernel = getStructuringElement(MORPH_RECT, Size(erodSize+1, erodSize+1), Point(-1,-1));
        Mat kernel2 = getStructuringElement(MORPH_RECT, Size(dilSize+1,dilSize+1), Point(-1,-1));

        //erode dilate blue color
        erode(mask, imgErode, kernel, Point(-1,-1),1);
        dilate(imgErode, imgDil, kernel2, Point(-1,-1),1);

        vector<vector<Point>> contours;
        findContours(imgDil, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if(contours.size()!= 0){
                sort(contours.begin(), contours.end(), compareContourAreas);
                Rect br = boundingRect(contours[contours.size()-1]);
            if(br.area() > 1000){
                rectangle(img, br.tl(), br.br(), Scalar(0,0,255), 3);
                    if(br.x < x && br.x + br.width > x){
                        if(br.y < y && br.y + br.height > y){
                        error = 0;
                    }
                        }else{
                    float midx = (2.0*br.x + br.width) / 2.0;
                    float midy = (2.0*br.y + br.height) / 2.0;
                    error = sqrt(pow((midx-x), 2)+pow((midy-y),2));
                }
            char str[200];
            char str2[200];
            sprintf(str, "error = %.2f", error);
            sprintf(str2, "x, y = (%.2f, %.2f)", (float)br.x, (float)br.y);
            putText(img, str, Point2f(100,50), FONT_HERSHEY_TRIPLEX, 1, Scalar(0,0,255));
            putText(img, str2, Point2f(100,100), FONT_HERSHEY_TRIPLEX, 1, Scalar(0,0,0));
            }
        }
        imshow(mainwindow, img);
        imshow(window2, mask);
        imshow(window3, imgDil);
        imshow(window4, imgErode);

        cap >> img;
        if(img.empty()) break;
        if(waitKey(10) == 27) break;
    }

    return 0;
}
