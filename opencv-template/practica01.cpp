
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;
int main(int, char **)
{
    Mat frame;
    VideoCapture cap("http://97.68.104.34/mjpg/video.mjpg", cv::CAP_ANY);

    Mat gray_scale;
    Mat binarizada;
    Mat rgbchannel[3];
    Mat green_chanel;
    vector<Mat> channels;
    Mat merge_image;

    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    while (true)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        resize(frame, frame, Size(640, 480), INTER_LINEAR);
        split(frame, rgbchannel);
        cvtColor(frame, gray_scale, COLOR_RGB2GRAY);
        threshold(gray_scale, binarizada, 125, 255, THRESH_BINARY);

        rgbchannel[1].copyTo(green_chanel);
        for (int i = 0; i < rgbchannel[1].rows; i++)
            for (int j = 0; j < rgbchannel[1].cols; j++)
                rgbchannel[1].at<uchar>(i, j) = 255 - rgbchannel[1].at<uchar>(i, j);

        channels.push_back(rgbchannel[0]);
        channels.push_back(rgbchannel[1]);
        channels.push_back(rgbchannel[2]);
        merge(channels, merge_image);
        channels.clear();

        imshow("Redimensionado", frame);
        imshow("Escala de grises", gray_scale);
        imshow("Binarizada", binarizada);
        imshow("Canal Verde", green_chanel);
        imshow("Canal Verde invertido", rgbchannel[1]);
        imshow("Nueva imagen", merge_image);

        if (waitKey(5) >= 0)
            break;
    }
    cap.release();
    destroyAllWindows();
    return 0;
}