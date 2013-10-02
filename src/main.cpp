#include "defines.hpp"
#include "ROI.hpp"
#include "ParticleFilter.hpp"




int main(int argc, char **argv){

    const float ACCUM_ALPHA(0.5);
    const float CHESS_BOARD_DIM(11);
    const float EXPECTED_W(50.0);
    const float EXPECTED_ASPECT_RATIO(1.0);
    const float IMPORTANCE_OF_W(1.0);
    const float IMPORTANCE_OF_ASPECT_RATIO(4.0);
    const float SCORE_THRESHOLD(.5);
    const int N_MAX_ABSENT_FRAMES(10);
    const int THRESHOLD(25);



    cv::VideoCapture cap;

    ROI roi(CHESS_BOARD_DIM,CHESS_BOARD_DIM);
    ParticleFilter partFilt(EXPECTED_W,EXPECTED_ASPECT_RATIO,
                            IMPORTANCE_OF_W,IMPORTANCE_OF_ASPECT_RATIO,SCORE_THRESHOLD);

    assert(argc == 2);
    cap.open(std::string(argv[1]));
    assert(cap.isOpened());

    cv::Mat frame,grey,accum,subimg,result;
    bool last_frame(false);
    bool tracking_started(false);
    int absent_frames(0);
    int spotted(0);
    while(!last_frame){
        cap>>frame;
        if(frame.empty()|| (tracking_started && absent_frames > N_MAX_ABSENT_FRAMES))
            last_frame = true;
        else{
            cv::cvtColor(frame,grey,CV_BGR2GRAY);
            cv::medianBlur(grey,grey,3);
            if(roi.apply(grey,frame,subimg)){
                if(accum.empty())
                    subimg.convertTo(accum,CV_64FC3);
                else
                    cv::accumulateWeighted(subimg,accum,ACCUM_ALPHA);

                accum.convertTo(subimg,CV_8U);
                cv::erode(subimg,subimg,cv::Mat());
                cv::adaptiveThreshold(subimg,subimg,255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV,41, THRESHOLD);
                cv::imshow("test",subimg);
                std::vector<std::vector<cv::Point> > contours;
                cv::findContours(subimg,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
                if(contours.size() > 0){

                    int i = partFilt.whichContourIsBest(contours);
                    if( i >= 0){
                        const auto &good_contour = contours[i];
                        cv::Moments moms= moments( good_contour , false );
                        cv::Point center( moms.m10/moms.m00 , moms.m01/moms.m00 );
                        cv::drawContours(frame,contours,i,cv::Scalar(0,100,200),2,CV_AA,cv::noArray(),2,roi.getRect().tl());
                        std::cout<<cv::Point3f(center.x,center.y,cap.get(CV_CAP_PROP_POS_FRAMES))<<std::endl;

                        absent_frames = 0;
                        ++spotted;
                        if(spotted >10 && !tracking_started)
                            tracking_started = true;
                    }
                    else
                        ++absent_frames;
                }
                else
                    ++absent_frames;
            }
            cv::imshow(std::string(argv[1]),frame);
            cv::waitKey(10);
        }
    }


    return 1;
}
