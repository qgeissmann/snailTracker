#ifndef ROI_H
#include "defines.hpp"

class ROI{
    public:
        ROI(int ncol,int nrow):
            m_patternsize(ncol,nrow),m_corner_found(false){}
        bool apply(const cv::Mat& img, cv::Mat& out,cv::Mat& subimg);
        cv::Rect getRect(){return cv::Rect(m_rrect.boundingRect());}
        cv::Point2f getRelativeCoords(const cv::Point2f pt){
            float w = calcTwoPointDist(m_roi_verts[1],m_roi_verts[2]);
            return cv::Point2f(pt.x / w, 1 -  pt.y / w);

//            cv::Mat pt2;
//            cv::transform(cv::Mat(pt), pt2, m_transform_map);

//            double pt_data[3] = {pt.x,pt.y,1.0};
//            cv::Mat pt_mat(3,1,CV_64F, (void*) pt_data);

//            std::cout<<"___________________________"<<std::endl;
//            std::cout<<pt<<std::endl;
//            std::cout<<m_transform_map<<std::endl;
//            std::cout<<pt_mat<<std::endl;
//
//            std::cout<<m_transform_map * pt_mat<<std::endl;
//        w = calcTwoPointDist(m_roi_verts[1],m_roi_verts[2])
//
//        px

        }
    protected:
        bool findCorners(const cv::Mat& img, cv::Mat& out);
        void makeTranformMap();
    private:
        cv::Size m_patternsize;
        std::vector<cv::Point2f> m_corners;
        std::vector<cv::Point> m_roi_verts;
        bool m_corner_found;
        cv::RotatedRect m_rrect;
        cv::Point m_parent_img_dim;
        cv::Mat m_transform_map;
};


#endif // ROI_H
