#include <iostream>
#include "CubicSpline.h"

int main()
{
    std::vector<cv::Point2d> points;

    points.push_back(cv::Point2d(0, 0));
    points.push_back(cv::Point2d(200, 230));
    points.push_back(cv::Point2d(200, 100));
    points.push_back(cv::Point2d(50, 30));
    points.push_back(cv::Point2d(50, 100));
    points.push_back(cv::Point2d(255, 255));

    CubicSpline cp(points);

    std::cout << "========= Inputs =========" << std::endl;

    for(auto &p : cp.points()) {
        std::cout << p << std::endl;
    }

    std::cout << "========= Results =========" << std::endl;

    for(int i = 0; i < 256; i++) {
        std::cout << cp.getPoint((double)i) << std::endl;
    }

    return 0;
}

