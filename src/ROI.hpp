#ifndef ROI_H
#include "defines.hpp"

class ROI{
    public:
        ROI(int ncol,int nrow):
            m_patternsize(ncol,nrow),m_corner_found(false){}
        bool apply(const cv::Mat& img, cv::Mat& out,cv::Mat& subimg);
        cv::Rect getRect(){return cv::Rect(m_rrect.boundingRect());}

    protected:
        bool findCorners(const cv::Mat& img, cv::Mat& out);
    private:
      cv::Size m_patternsize;
      std::vector<cv::Point2f> m_corners;
      bool m_corner_found;
      cv::RotatedRect m_rrect;
};


#endif // ROI_H
