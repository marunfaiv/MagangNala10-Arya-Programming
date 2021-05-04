#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;


int main(){

    const int alpha_slider_max = 256;
    int hue;
    double alpha;
    double beta;
    VideoCapture cap(-1);
    Mat img,frame,imgHSV,imgtrack;
    string videoname1, videoname2, trackname;
    videoname1 = "Testing 1";
    videoname2 = "Testing 2";
    trackname = "trackbar1";

    while(true){
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        hue = 0;
        namedWindow("Linear Blend", WINDOW_AUTOSIZE);

        createTrackbar(trackname, "Linear Blend", &hue, alpha_slider_max, 0);
        imshow(videoname1, img);
        imshow(videoname2, imgHSV);

        waitKey(1);

        cap >> frame;
        if(frame.empty()) break;
        if(waitKey(10) >= 0) break;
    }

    return 0;
}