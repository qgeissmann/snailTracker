#include "ParticleFilter.hpp"


ParticleFilter::ParticleFilter(float w_expected, float ar_expected, float w_importance, float ar_importance, float threshold) :
m_w_expected(w_expected),
m_ar_expected(ar_expected),
m_w_importance(w_importance),
m_ar_importance(ar_importance),
m_threshold(threshold){
}

float ParticleFilter::calcTwoPointDist(const cv::Point P0,const cv::Point P1){
        float Ax,Ay;
        Ax=P0.x-P1.x;
        Ax *= Ax;
        Ay=P0.y-P1.y;
        Ay *= Ay;
        return sqrt(Ax+Ay);
}

float ParticleFilter::calculateAR(const std::vector<cv::Point>& contour){
    cv::Point2f rRect[4];
    cv::minAreaRect(contour).points(rRect);
    float A,B;
    A = calcTwoPointDist(rRect[0],rRect[1])+1;
    B = calcTwoPointDist(rRect[1],rRect[2])+1;
    return std::max(A,B)/std::min(A,B);
}



float ParticleFilter::calculateW(const std::vector<cv::Point>& contour){
    cv::Point2f rRect[4];
    cv::minAreaRect(contour).points(rRect);
    float A,B;
    A = calcTwoPointDist(rRect[0],rRect[1])+1;
    B = calcTwoPointDist(rRect[1],rRect[2])+1;
    return  std::max(A,B);
}

float ParticleFilter::calcScoreContour(const std::vector<cv::Point>& contour){
    float ar = calculateAR(contour);
    float w = calculateW(contour);
    float w_err = (1-w/m_w_expected);
    float ar_err = (1-ar/m_ar_expected);
    return  (w_err * w_err * m_w_importance + ar_err * ar_err * m_ar_importance) / (m_ar_importance+m_w_importance);
}
int ParticleFilter::whichContourIsBest(const std::vector<std::vector<cv::Point> >& contours){
    float best_score(INT_MAX);
    int best_idx(-1);
    for(unsigned int i = 0; i < contours.size();++i){
            float score = calcScoreContour(contours[i]);

            if(score < best_score){
              best_idx = i;
              best_score = score;
            }
    }
    if(best_score < m_threshold)
        return best_idx;
    else
        return -1;
}
