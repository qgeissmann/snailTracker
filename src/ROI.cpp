#include "ROI.hpp"


void ROI::makeTranformMap(){
    cv::Point2f rect_points[4];
    m_rrect.points( rect_points );
    std::vector<cv::Point> image_verts;
    m_roi_verts = std::vector<cv::Point>();
    image_verts.push_back(cv::Point(0,1000));
    image_verts.push_back(cv::Point(0,0));
    image_verts.push_back(cv::Point(1000,0));

    for(auto i : image_verts){
        float closest_dist = INT_MAX;
        int closest_idx = -1;
        for( int j = 0; j < 4; j++ ){
            float dist = calcTwoPointDist(rect_points[j],i);
            if (dist < closest_dist){
                closest_idx = j;
                closest_dist = dist;
            }
         }
        m_roi_verts.push_back(rect_points[closest_idx]);
    }


//
    cv::Point2f img_fv[3], roi_fv[3];
    for (int i =0; i < 3; ++i){
        img_fv[i] = image_verts[i];
        roi_fv[i] = m_roi_verts[i];
    }

    m_transform_map = cv::getAffineTransform(img_fv,roi_fv);
//    cv::invertAffineTransform(m_transform_map,m_transform_map);

}

bool ROI::apply(const cv::Mat& img, cv::Mat& out,cv::Mat& subimg){
    if(!m_corner_found){
        findCorners(img,out);
        m_parent_img_dim = cv::Point(img.cols,img.rows);
        if (m_corner_found){
            m_rrect = cv::minAreaRect(m_corners);
            makeTranformMap();
            subimg = img;
        }
        return false;
    }
    else{
//        cv::drawChessboardCorners(out, m_patternsize, cv::Mat(m_corners), true);

        cv::Point2f rect_points[4];
        m_rrect.points( rect_points );

            cv::line( out, m_roi_verts[0], m_roi_verts[1], cv::Scalar(200,100,0), 2, CV_AA );
            cv::line( out, m_roi_verts[1], m_roi_verts[2], cv::Scalar(200,100,0), 2, CV_AA );
        }
        subimg = img(m_rrect.boundingRect());
        return true;

}


bool ROI::findCorners(const cv::Mat& img, cv::Mat& out){
    img.copyTo(out);
    cv::Mat grey = img;
    int flag = cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK + CV_CALIB_CB_ADAPTIVE_THRESH;
    m_corner_found = cv::findChessboardCorners(grey, m_patternsize, m_corners,flag);
    if(m_corner_found){
      cv::cornerSubPix(grey, m_corners, m_patternsize, cv::Size(-1, -1),
        cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    }
}
