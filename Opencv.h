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
    // 使用 Sobel 算子进行边缘检测
    Mat edgeDetection(Mat image) {
        Mat gray, grad_x, grad_y, abs_grad_x, abs_grad_y, grad;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;

        // 转换为灰度图像
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // 计算 X 方向梯度
        Sobel(gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);

        // 计算 Y 方向梯度
        Sobel(gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
        convertScaleAbs(grad_y, abs_grad_y);

        // 合并梯度
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

        return grad;
    }
    // 使用 Canny 算子进行边缘检测
    Mat edgeDetection(Mat image) {
        Mat gray, edges;
        int lowThreshold = 50;
        int ratio = 3;
        int kernel_size = 3;

        // 转换为灰度图像
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // 使用 Canny 算子进行边缘检测
        Canny(gray, edges, lowThreshold, lowThreshold * ratio, kernel_size);

        return edges;
    }

    // 特征匹配
    void featureMatching(Mat img1, Mat img2) {
        // 转换为灰度图像
        Mat gray1, gray2;
        cvtColor(img1, gray1, COLOR_BGR2GRAY);
        cvtColor(img2, gray2, COLOR_BGR2GRAY);

        // 初始化 ORB 检测器
        Ptr<ORB> detector = ORB::create();

        // 检测关键点和描述符
        vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;
        detector->detectAndCompute(gray1, noArray(), keypoints1, descriptors1);
        detector->detectAndCompute(gray2, noArray(), keypoints2, descriptors2);

        // 匹配描述符
        BFMatcher matcher(NORM_HAMMING);
        vector<DMatch> matches;
        matcher.match(descriptors1, descriptors2, matches);

        // 绘制匹配结果
        Mat img_matches;
        drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);

        // 显示匹配结果
        showImage(img_matches, "Matches");
    }
    // 特征匹配
    void featureMatching(Mat img1, Mat img2) {
        // 转换为灰度图像
        Mat gray1, gray2;
        cvtColor(img1, gray1, COLOR_BGR2GRAY);
        cvtColor(img2, gray2, COLOR_BGR2GRAY);

        // 初始化 SIFT 检测器
        Ptr<SIFT> detector = SIFT::create();

        // 检测关键点和描述符
        vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;
        detector->detectAndCompute(gray1, noArray(), keypoints1, descriptors1);
        detector->detectAndCompute(gray2, noArray(), keypoints2, descriptors2);

        // 匹配描述符
        BFMatcher matcher(NORM_L2);
        vector<DMatch> matches;
        matcher.match(descriptors1, descriptors2, matches);

        // 绘制匹配结果
        Mat img_matches;
        drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);

        // 显示匹配结果
        showImage(img_matches, "Matches");
    }
    // 特征匹配
    void featureMatching(Mat img1, Mat img2) {
        // 转换为灰度图像
        Mat gray1, gray2;
        cvtColor(img1, gray1, COLOR_BGR2GRAY);
        cvtColor(img2, gray2, COLOR_BGR2GRAY);

        // 初始化 SURF 检测器
        Ptr<SURF> detector = SURF::create();

        // 检测关键点和描述符
        vector<KeyPoint> keypoints1, keypoints2;
        Mat descriptors1, descriptors2;
        detector->detectAndCompute(gray1, noArray(), keypoints1, descriptors1);
        detector->detectAndCompute(gray2, noArray(), keypoints2, descriptors2);

        // 匹配描述符
        BFMatcher matcher(NORM_L2);
        vector<DMatch> matches;
        matcher.match(descriptors1, descriptors2, matches);

        // 绘制匹配结果
        Mat img_matches;
        drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);

        // 显示匹配结果
        showImage(img_matches, "Matches");
    }

    // 图像标定
    void calibrateCamera(vector<string> imageList, Size boardSize) {
        vector<vector<Point3f>> objectPoints;
        vector<vector<Point2f>> imagePoints;

        // 计算棋盘格角点的三维坐标
        vector<Point3f> obj;
        for (int i = 0; i < boardSize.height; i++) {
            for (int j = 0; j < boardSize.width; j++) {
                obj.push_back(Point3f(j, i, 0));
            }
        }

        // 遍历所有图像，提取角点坐标
        for (int i = 0; i < imageList.size(); i++) {
            Mat image = readImage(imageList[i]);
            vector<Point2f> corners;
            bool found = findChessboardCorners(image, boardSize, corners);

            if (found) {
                objectPoints.push_back(obj);
                imagePoints.push_back(corners);
            }
        }

        // 计算相机内参和畸变系数
        Mat cameraMatrix, distCoeffs;
        vector<Mat> rvecs, tvecs;
        calibrateCamera(objectPoints, imagePoints, boardSize, cameraMatrix, distCoeffs, rvecs, tvecs);

        // 输出结果
        cout << "Camera Matrix: " << cameraMatrix << endl;
        cout << "Distortion Coefficients: " << distCoeffs << endl;
    }

    // 计算并绘制直方图
    Mat calcHist(Mat image) {
        // 转换为灰度图像
        Mat gray;
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // 计算直方图
        int histSize = 256;
        float range[] = { 0, 256 };
        const float* histRange = { range };
        Mat hist;
        calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange);

        // 绘制直方图
        int hist_w = 512;
        int hist_h = 400;
        int bin_w = cvRound((double)hist_w / histSize);
        Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
        normalize(hist, hist, 0, histImage.rows, NORM_MINMAX);

        for (int i = 1; i < histSize; i++) {
            line(histImage,
                Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                Point(bin_w * i, hist_h - cvRound(hist.at<float>(i))),
                Scalar(255, 0, 0), 2);
        }

        return histImage;
    }

    // 直线检测
    void lineDetection(Mat image) {
        // 转换为灰度图像
        Mat gray;
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // 使用 Canny 算子进行边缘检测
        Mat edges;
        Canny(gray, edges, 50, 150);

        // 使用霍夫变换进行直线检测
        vector<Vec2f> lines;
        HoughLines(edges, lines, 1, CV_PI / 180, 100);

        // 绘制直线
        for (size_t i = 0; i < lines.size(); i++) {
            float rho = lines[i][0];
            float theta = lines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a * rho, y0 = b * rho;
            pt1.x = cvRound(x0 + 1000 * (-b));
            pt1.y = cvRound(y0 + 1000 * a);
            pt2.x = cvRound(x0 - 1000 * (-b));
            pt2.y = cvRound(y0 - 1000 * a);
            line(image, pt1, pt2, Scalar(0, 0, 255), 3);
        }

        // 显示结果
        showImage(image, "Lines");
    }
    
};
