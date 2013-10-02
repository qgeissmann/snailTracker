#include "ROI.hpp"

bool ROI::apply(const cv::Mat& img, cv::Mat& out,cv::Mat& subimg){
    if(!m_corner_found){
        findCorners(img,out);
        if (m_corner_found){
            m_rrect = cv::minAreaRect(m_corners);
            subimg = img;
        }
        return false;
    }
    else{
        cv::drawChessboardCorners(out, m_patternsize, cv::Mat(m_corners), true);

        cv::Point2f rect_points[4];
        m_rrect.points( rect_points );

        for( int j = 0; j < 4; j++ )
            cv::line( out, rect_points[j], rect_points[(j+1)%4], cv::Scalar(200,100,0), 2, CV_AA );
        }
        subimg = img(m_rrect.boundingRect());
        return true;

}


bool ROI::findCorners(const cv::Mat& img, cv::Mat& out){
    img.copyTo(out);
    cv::Mat grey = img;


    int flag = cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK + CV_CALIB_CB_ADAPTIVE_THRESH;
    //'cv::adaptiveThreshold(grey,grey,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,51,0);
    cv::imshow("test",grey);

    m_corner_found = cv::findChessboardCorners(grey, m_patternsize, m_corners,flag);
    //'cv::imshow("test",grey);
    if(m_corner_found){
      cv::cornerSubPix(grey, m_corners, m_patternsize, cv::Size(-1, -1),
        cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    }
}
