#include "CubicSpline.h"

bool CubicSpline::isInRange(double x)
{
    // Check Line or Spline
    if(!(this->isLine() || this->isSpline()))
        return false;

    // Whether within the range
    return mPoints.front().x <= x && mPoints.back().x >= x;
}

void CubicSpline::addPoint(const cv::Point2d &p)
{
    // Add new point
    mPoints.push_back(p);

    // Sort the points
    this->sort();

    // Solve the spline
    this->solve();
}

void CubicSpline::addPoints(const std::vector<cv::Point2d> &points)
{
    // Add new points
    mPoints.insert(mPoints.end(), points.begin(), points.end());

    // Sort the points
    this->sort();

    // Solve the spline
    this->solve();
}

double CubicSpline::getY(double x)
{
    // Check whether within the range
    if(!this->isInRange(x))
        return 0.0;

    // Check whether straight
    if(this->isLine())
        return ((mPoints[1].y - mPoints[0].y) / (mPoints[1].x - mPoints[0].x)) * (x - mPoints[0].x) + mPoints[0].y;

    // Find the index
    auto itr = std::find_if(mPoints.begin(), mPoints.end(), [x](const cv::Point2d &p) { return p.x > x; });

    if(itr == mPoints.end())
        return mPoints.back().y;

    int index = std::distance(mPoints.begin(), itr) - 1;

    // Calculate the coordinate Y
    double a, b, c, d, h, k;

    h = mPoints[index + 1].x - mPoints[index].x;
    c = mS[index];

    a = mPoints[index].y;
    b = ((mPoints[index + 1].y - a) / h) - ((h * (mS[index + 1] + 2.0 * c)) / 3.0);
    d = (mS[index + 1] - c) / (3.0 * h);

    k = x - mPoints[index].x;

    return a + b * k + c * k * k + d * k * k * k;
}

void CubicSpline::sort()
{
    // Sort the points
    std::sort(mPoints.begin(), mPoints.end(),
              [](const cv::Point2d &lhs, const cv::Point2d &rhs) {
                    return lhs.x < rhs.x;
    });

    // Erase the duplicate points
    mPoints.erase(std::unique(mPoints.begin(), mPoints.end(),
                              [](const cv::Point2d &lhs, const cv::Point2d &rhs) {
        return lhs.x == rhs.x;
    }), mPoints.end());
}

void CubicSpline::solve()
{
    // Check whether spline
    if(!this->isSpline())
        return;

    // Solve the spline
    cv::Mat mat = cv::Mat::zeros(mPoints.size(), mPoints.size(), CV_64FC1);
    std::vector<double> rhs(mPoints.size(), 0);
    mS = std::vector<double>(mPoints.size(), 0);

    mat.at<double>(0, 0) = 1;
    mat.at<double>(mat.rows - 1, mat.cols - 1) = 1;

    for(int i = 1; i < mat.rows - 1; i++) {

        double h0 = mPoints[i].x - mPoints[i - 1].x;
        double h1 = mPoints[i + 1].x - mPoints[i].x;

        mat.at<double>(i, i - 1) = h0;
        mat.at<double>(i, i) = 2.0 * (h0 + h1);
        mat.at<double>(i, i + 1) = h1;

        rhs[i] = 3.0 * ((mPoints[i + 1].y - mPoints[i].y) / h1 - (mPoints[i].y - mPoints[i - 1].y) / h0);
    }

    cv::solve(mat, cv::Mat(rhs), cv::Mat(mS));
}
