#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;

int main(){

    int hmin1,smin1,vmin1,hmax1,smax1,vmax1;
    int hmin2,smin2,vmin2,hmax2,smax2,vmax2;
    VideoCapture cap(-1);
    Mat img,frame,imgHSV,imgHSV2, imgtrack, imgCanny, imgDil, mask,mask2,kernel;
    kernel = getStructuringElement(MORPH_RECT, Size(3,1));
    string windowname;
    windowname = "Camera 1";

    //WARNA MERAH
    hmin1 = 0; smin1 = 0; vmin1 = 0;
    hmax1 = 180; smax1 = 255; vmax2 = 255;

    //WARNA HIJAU
    hmin2 = 0; smin2 = 0; vmin2 = 0;
    hmax2 = 180; smax2 = 255; vmax2 = 255;

    namedWindow(windowname);
    createTrackbar("Hue min merah", windowname, &hmin1, 180);
    createTrackbar("Hue min hijau", windowname, &hmin2, 180);
    createTrackbar("Sat min merah", windowname, &smin1, 255);
    createTrackbar("Sat min hijau", windowname, &smin2, 255);
    createTrackbar("Val min merah", windowname, &vmin1, 255);
    createTrackbar("Val min hijau", windowname, &vmin2, 255);

    while(true){
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        cvtColor(img, imgHSV2, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hmin1,smin1,vmin1), Scalar(hmax1,smax1,vmax1), mask);
        inRange(imgHSV2, Scalar(hmin2,smin2,vmin2), Scalar(hmax2,smax2,vmax2), mask2);

        vector <vector<Point>> contours;
            findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
            for (int i = 0; i < contours.size(); i++){
                int area = contourArea(contours[i]);
                vector <vector<Point>> conPoly(contours.size());
                Rect boundRect = boundingRect(contours[i]);
                /*if(boundRect.area()>350 && (boundRect.width < 70|| boundRect.height<70)){
                    rectangle(img, boundRect.tl(), boundRect.br(), (255,0,0), 3);
                }*/
                if(area > 1000 && boundRect.area()>350 && (boundRect.width < 70|| boundRect.height<70)){
                    rectangle(img, boundRect.tl(), boundRect.br(), (255,0,0), 3);
                }
            }

            vector <vector<Point>> contours2;
            findContours(mask2, contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
            for (int i = 0; i < contours2.size(); i++){
                int area = contourArea(contours2[i]);
                Rect boundRect = boundingRect(contours2[i]);
                /*if(boundRect.area()>350 && (boundRect.width < 70|| boundRect.height<70)){
                    rectangle(img, boundRect.tl(), boundRect.br(), (0,128,0), 3);
                }*/
                if(area > 1000 && boundRect.area()>350 && (boundRect.width < 70|| boundRect.height<70)){
                    rectangle(img, boundRect.tl(), boundRect.br(), (0,128,0), 3);        
                }
            }

        imshow(windowname, img);

        waitKey(1);

        cap >> frame;
        if(frame.empty()) break;
        if(waitKey(10) >= 0) break;
    }

    return 0;
}

