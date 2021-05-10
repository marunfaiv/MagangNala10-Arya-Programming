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

    int hmin1,smin1,vmin1,hmax1,smax1,vmax1;
    int hmin2,smin2,vmin2,hmax2,smax2,vmax2;
    int dilSize = 0, erodSize = 0;
    int dilVal1,dilVal2,erodVal1,erodVal2;
    int maxDil = 255, maxErod = 255;
    float x = 640.0/2.0;
    float y = 480.0/2.0;
    float error = 0;

    VideoCapture cap(-1);
    Mat img,frame,imgHSV,imgHSV2, imgtrack, imgCanny, imgDil, mask,mask2,imgErode;
    string windowname, videoname1, videoname2, videoname3, videoname4;
    windowname = "Camera 1";
    videoname1 = "Red Mask";
    videoname2 = "Green Mask";
    videoname3 = "Dilated";
    videoname4 = "Eroded";

    //WARNA MERAH
    hmin1 = 0; smin1 = 0; vmin1 = 0;
    hmax1 = 180; smax1 = 255; vmax1 = 255;
    //WARNA HIJAU
    hmin2 = 0; smin2 = 0; vmin2 = 0;
    hmax2 = 180; smax2 = 255; vmax2 = 255;

    namedWindow(windowname);
    createTrackbar("Hue min merah", windowname, &hmin1, hmax1);
    createTrackbar("Sat min merah", windowname, &smin1, smax1);
    createTrackbar("Val min merah", windowname, &vmin1, vmax1);
    createTrackbar("Erode value", windowname, &erodSize, maxErod);
    createTrackbar("Dilate value", windowname, &dilSize, maxDil);


        /*if(erodSize == 0 && dilSize == 0){
             erodVal1 = 1; erodVal2 = 1;
             dilVal1 = 1; dilVal2 = 1;
            Mat kernel = getStructuringElement(MORPH_RECT, Size(erodVal1,erodVal2), Point(-1,-1));
            Mat kernel2 = getStructuringElement(MORPH_RECT, Size(dilVal1,dilVal2), Point(-1,-1));
        }if(erodSize == 1 && dilSize == 1){
             erodVal1 = 5; erodVal2 = 5;
             dilVal1 = 5; dilVal2 = 5;
            Mat kernel = getStructuringElement(MORPH_RECT, Size(erodVal1,erodVal2), Point(-1,-1));
            Mat kernel2 = getStructuringElement(MORPH_RECT, Size(dilVal1,dilVal2), Point(-1,-1));
        }if(erodSize == 2 && dilSize ==2){
             erodVal1 = 10; erodVal2 = 10;
             dilVal1 = 10; dilVal2 = 10;
            Mat kernel = getStructuringElement(MORPH_RECT, Size(erodVal1,erodVal2), Point(-1,-1));
            Mat kernel2 = getStructuringElement(MORPH_RECT, Size(dilVal1,dilVal2), Point(-1,-1));
        }*/
    //createTrackbar("Hue min hijau", windowname, &hmin2, 180);
    //createTrackbar("Sat min hijau", windowname, &smin2, 255);
    //createTrackbar("Val min hijau", windowname, &vmin2, 255);

    while(true){
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        cvtColor(img, imgHSV2, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hmin1,smin1,vmin1), Scalar(hmax1,smax1,vmax1), mask);
        //inRange(imgHSV2, Scalar(hmin2,smin2,vmin2), Scalar(hmax2,smax2,vmax2), mask2);
        Mat kernel = getStructuringElement(MORPH_RECT, Size(erodSize+1, erodSize+1), Point(-1,-1));
        Mat kernel2 = getStructuringElement(MORPH_RECT, Size(dilSize+1,dilSize+1), Point(-1,-1));
        erode(mask, imgErode, kernel, Point(-1,-1),1);
        dilate(imgErode, imgDil, kernel2, Point(-1,-1),1);

        vector <vector<Point>> contours;
            findContours(imgDil, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if(contours.size()!= 0){
                sort(contours.begin(), contours.end(), compareContourAreas);
                Rect boundRect = boundingRect(contours[contours.size()-1]);
            if(boundRect.area() > 3000){
                rectangle(img, boundRect.tl(), boundRect.br(), Scalar(0,0,255), 3);
                if(boundRect.x < x && boundRect.x + boundRect.width > x){
                    if(boundRect.y < y && boundRect.y + boundRect.height > y){
                        error = 0;
                    }
                }else{
                    float midx = (2.0*boundRect.x + boundRect.width) / 2.0;
                    float midy = (2.0*boundRect.y + boundRect.height) / 2.0;
                    error = sqrt(pow((midx-x), 2)+pow((midy-y),2));
                }
            char str[200];
            char str2[200];
            sprintf(str, "error = %.2f", error);
            sprintf(str2, "x, y = (%.2f, %.2f)", (float)boundRect.x, (float)boundRect.y);
            putText(img, str, Point2f(100,50), FONT_HERSHEY_TRIPLEX, 2, Scalar(0,0,0));
            putText(img, str2, Point2f(100,100), FONT_HERSHEY_TRIPLEX, 2, Scalar(0,0,0));
            }
        }
    
        imshow(windowname, img);
        imshow(videoname1, mask);
        imshow(videoname3, imgDil);
        imshow(videoname4, imgErode);

        waitKey(1);


        cap >> frame;
        if(frame.empty()) break;
        if(waitKey(10) == 27) break;
        contours.clear();
    }

    return 0;
}
