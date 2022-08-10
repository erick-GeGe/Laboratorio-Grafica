#include "opencv2/objdetect/objdetect.hpp"

#include "opencv2/highgui/highgui.hpp"

#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;

using namespace std;

int main()

{

    Mat image;

    image = imread("C:/Users/erick/Documents/GitHub/Laboratorio-Grafica/opencv-template/imgs/test.png");

    if (!image.data) // Check for invalid input

    {

        cout << "Could not open or find the image" << std::endl;

        return -1;
    }

    resize(image, image, Size(640, 480), INTER_LINEAR);

    imshow("Original Image", image);

    Mat rgbchannel[3];

    // The actual splitting.

    split(image, rgbchannel);

    imshow("Red", rgbchannel[0]);

    imshow("Green", rgbchannel[1]);

    imshow("Blue", rgbchannel[2]);

    waitKey(0); // Wait for a keystroke in the window

    return 0;
}