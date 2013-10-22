#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/video/video.hpp"

static float calcTwoPointDist(const cv::Point P0,const cv::Point P1){
        float Ax = P0.x-P1.x;
        float Ay = P0.y-P1.y;
        return sqrt( Ax*Ax + Ay*Ay);
}

#endif
