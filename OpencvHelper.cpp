#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
class OpenCVClass {
public:
    // 读取图像
    Mat readImage(string filename) {
        Mat image = imread(filename, IMREAD_COLOR);
        return image;
    }
    // 显示图像
    void showImage(Mat image, string windowName) {
        namedWindow(windowName, WINDOW_AUTOSIZE);
        imshow(windowName, image);
    }
    // 保存图像
    void saveImage(Mat image, string filename) {
        imwrite(filename, image);
    }
