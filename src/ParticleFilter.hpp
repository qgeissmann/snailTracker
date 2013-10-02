#ifndef PARTICLE_H
#include "defines.hpp"

class ParticleFilter{
    public:
        ParticleFilter(float w_expected, float ar_expected, float w_importance, float ar_importance, float threshold);
        int whichContourIsBest(const std::vector<std::vector<cv::Point> >& contours);
    protected:
        float calcTwoPointDist(const cv::Point P0,const cv::Point P1);
        float calculateAR(const std::vector<cv::Point>& contour);
        float calculateW(const std::vector<cv::Point>& contour);
        float calcScoreContour(const std::vector<cv::Point>& contour);
    private:
        float m_w_expected, m_ar_expected;
        float m_w_importance, m_ar_importance;
        float m_threshold;

};



#endif // PARTICLE_H


