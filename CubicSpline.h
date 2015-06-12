#pragma once

#include <vector>
#include <opencv2/core.hpp>

class CubicSpline
{
public:
    CubicSpline() {}
    explicit CubicSpline(const std::vector<cv::Point2d> &points) { this->addPoints(points); }
    ~CubicSpline() { mPoints.clear(); }

    bool isEmpty() const { return mPoints.empty(); }
    bool isValid() const { return mPoints.size() > 0; }
    bool isPoint() const { return mPoints.size() == 1; }
    bool isLine() const { return mPoints.size() == 2; }
    bool isSpline() const { return mPoints.size() > 2; }

    bool isInRange(double x);

    void clear() { mPoints.clear(); }

    void addPoint(const cv::Point2d &p);
    void addPoints(const std::vector<cv::Point2d> &points);

    cv::Point2d getPoint(double x) { return cv::Point2d(x, this->getY(x)); }
    double getY(double x);

    const std::vector<cv::Point2d> &points() const { return mPoints; }

private:
    void sort();
    void solve();

    std::vector<double> mS;
    std::vector<cv::Point2d> mPoints;
};
