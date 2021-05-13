#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool compareContourAreas(vector<Point> contour1, vector<Point> contour2){
    double i = fabs(contourArea(Mat(contour1)));
    double j = fabs(contourArea(Mat(contour2)));

    return (i < j);
}

int main(){

    int hmin_blue, smin_blue, vmin_blue;
    int hmax_blue, smax_blue, vmax_blue;
    int hmin_purp, smin_purp, vmin_purp;
    int hmax_purp, smax_purp, vmax_purp;
    int erodSize = 0, dilSize = 0;
    int ox1,ox2,oy1,oy2; //x and y outer
    int ix1,ix2,iy1,iy2; // x and y inside
    int errhorizon, errverti;
    int left,right,top,down;
    int xp = 0,yp = 0; //x pusat and y pusat

    ox1=0; ox2=0; oy1=0; oy2=0;
    ix1=0; ix2 = 0; iy1 = 0; iy2 = 0;
    errhorizon= 0; errverti = 0;
    left = 0; right = 0; top = 0; down = 0;

    //blue
    hmin_blue = 0; smin_blue = 0; vmin_blue = 0;
    hmax_blue = 180; smax_blue = 255; vmax_blue = 255;
    //purple
    hmin_purp = 0; smin_purp = 0; vmin_purp = 0;
    hmax_purp = 180; smax_purp = 255; vmax_purp = 255;

    int x = 640.0/2.0;
    int y = 480.0/2.0;
    int error = 0;

    VideoCapture cap(-1);
    Mat img, imgHSV, mask, imgDil, imgErode;
    Mat imgHSV2, mask2, imgDil2, imgErode2;
    string windowname, windowname2, windowname3, windowtrack, windowtrack2;
    windowname = "Main Camera";
    windowname2 = "Mask Purp";
    windowname3 = "Mask Blue";
    windowtrack = "Purple Trackbar";
    windowtrack2 = "Blue Trackbar";
    
    //trackbar untuk erode and dilate untuk warna ungu
    namedWindow(windowtrack);
    createTrackbar("Hue", windowtrack, &hmin_purp, hmax_purp);
    createTrackbar("Saturation", windowtrack, &smin_purp, smax_purp);
    createTrackbar("Value", windowtrack, &vmin_purp, vmax_purp);
    createTrackbar("Erode", windowtrack, &erodSize, 10);
    createTrackbar("Dilate", windowtrack, &dilSize, 10);

    //trackbar untuk erode and dilate warna biru
    namedWindow(windowtrack2);
    createTrackbar("Hue", windowtrack2, &hmin_blue, hmax_blue);
    createTrackbar("Saturation", windowtrack2, &smin_blue, smax_blue);
    createTrackbar("Value", windowtrack2, &vmin_blue, vmax_blue);
    createTrackbar("Erode", windowtrack2, &erodSize, 10);
    createTrackbar("Dilate", windowtrack2, &dilSize, 10);

    while(true){

        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        cvtColor(img, imgHSV2, COLOR_BGR2HSV);

        inRange(imgHSV, Scalar(hmin_purp,smin_purp,vmin_purp), Scalar(hmax_purp,smax_blue,vmax_purp), mask);
        inRange(imgHSV2, Scalar(hmin_blue, smin_blue, vmin_blue), Scalar(hmax_blue, smax_blue, vmax_blue), mask2);
        
        Mat kernel1 = getStructuringElement(MORPH_RECT, Size(erodSize+1, erodSize+1), Point(-1,-1));
        Mat kernel2 = getStructuringElement(MORPH_RECT, Size(dilSize+1,dilSize+1), Point(-1,-1));
        Mat kernel3 = getStructuringElement(MORPH_RECT, Size(erodSize+1, erodSize+1), Point(-1,-1));
        Mat kernel4 = getStructuringElement(MORPH_RECT, Size(dilSize+1,dilSize+1), Point(-1,-1));

        //Dilate and Erode untuk warna ungu
        erode(mask, imgErode, kernel1, Point(-1,-1), 1);
        dilate(imgErode, imgDil, kernel2, Point(-1,-1), 1);

        //Dilate and Erode untuk warna biru
        erode(mask2, imgErode2, kernel3, Point(-1,-1), 1);
        dilate(imgErode2, imgDil2, kernel4, Point(-1,-1), 1);
        
        //contour for purple color
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if(contours.size() != 0){
            sort(contours.begin(), contours.end(), compareContourAreas);
            Rect br = boundingRect(contours[contours.size()-1]);
            if(br.area() > 1000){
                rectangle(img, br.tl(), br.br(), Scalar(255,0,0), 3);
                ox1 = br.x;
                ox2 = br.x + br.width;
                oy1 = br.y;
                oy2 = br.y + br.height;
            }
        }

        //contour for blue color
        vector<vector<Point>> contours2;
        vector<Vec4i> hierarchy2;
        findContours(imgDil2, contours2, hierarchy2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if(contours2.size() != 0){
            sort(contours2.begin(), contours2.end(), compareContourAreas);
            Rect br2 = boundingRect(contours2[contours2.size()-1]);
            if(br2.area() > 1000){
                rectangle(img, br2.tl(), br2.br(), Scalar(120,0,120), 3);
                ix1 = br2.x;
                ix2 = br2.x + br2.width;
                iy1 = br2.y;
                iy2 = br2.y + br2.height;
                if((ix1 > ox1 && ix2 < ox2) && (iy1 > oy1 && iy2 < oy2)){
                    rectangle(img, br2.tl(), br2.br(), Scalar(255,0,0), 3);
                    xp = (ix1 + ix2) / 2;
                    yp = (iy1 +iy2) / 2;
                }
                if((ix1 < x && ix2 > x) && (iy1 < y && iy2 > y)){
                    top = 0;
                    down = 0;
                    left = 0;
                    right = 0;
                }
                else{
                    if(xp < x){
                        left = xp-x;
                        if(y > iy1 && y < iy2){
                            top = 0;
                            down = 0;
                        }
                        right = 0;
                    }
                    if(xp > x){
                    right = xp-x;
                    if(y > iy1 && y < iy2){
                        top = 0;
                        down = 0;
                    }
                    left = 0;
                    }
                    if(yp > y){
                        down  = yp - y;
                        if(x > ix1 && x < ix2){
                            down = 0;
                            right = 0;
                        }
                        top = 0;
                    }
                    if(yp < y){
                    top = yp-y;
                    if(x > ix1 && x < ix2){
                        left = 0;
                        right = 0;
                    }
                    down = 0;
                }
                }
            }
        }

            char Upper[200];
            char Bottom[200];
            char Left[200];
            char Right[200];
            char kor[200];
            sprintf(Upper,"Upper     = %d", (top));
            sprintf(Bottom,"Bottom  = %d", (down));
            sprintf(Left,"Left   = %d", (left));
            sprintf(Right,"Right  = %d", (right));
            sprintf(kor,"xp, yp  = %d, %d", xp, yp);
            putText(img, Upper, Point2f(15,50), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
            putText(img, Bottom, Point2f(15,100), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
            putText(img, Left, Point2f(15,150), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
            putText(img, Right, Point2f(15,200), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
            putText(img, kor, Point2f(15,250), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));

        imshow(windowname, img);
        imshow(windowname2, imgDil);
        imshow(windowname3, imgDil2);

        cap >> img;
        if(img.empty()) break;
        if(waitKey(10) == 27) break;
    }

    return 0;
}
