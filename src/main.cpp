#include "defines.hpp"
#include "ROI.hpp"
//#include "ParticleFilter.hpp"






cv::Vec3f calculateARWH(const std::vector<cv::Point>& contour){
    cv::Point2f rRect[4];
    cv::minAreaRect(contour).points(rRect);
    float A = calcTwoPointDist(rRect[0],rRect[1])+1;
    float B = calcTwoPointDist(rRect[1],rRect[2])+1;
    cv::Vec3f out;
    out[0] = std::max(A,B)/std::min(A,B);
    out[1] = std::max(A,B);
    out[2] = std::min(A,B);
    return out;
}

void correctBrightness(cv::Mat& inout){
        cv::Mat conv;
        cv::medianBlur(inout,conv,51);
        inout=conv-inout;
}



int main(int argc, char **argv){

    const float ACCUM_ALPHA(0.75);
    const float CHESS_BOARD_DIM(11);
    const float EXPECTED_ASPECT_RATIO(1.0);
    const float IMPORTANCE_OF_ASPECT_RATIO(4.0);
    const float SCORE_THRESHOLD(.5);
    const float MAX_AR(1.9);
    const int MIN_LENGTH(5);
    const int MIN_AREA(10);
    const int MAX_AREA(100);
    const int N_MAX_ABSENT_FRAMES(12);
    const int THRESHOLD(18);
    const int MAX_CIRCU(2);


    bool hide(false);
    cv::VideoCapture cap;

    ROI roi(CHESS_BOARD_DIM,CHESS_BOARD_DIM);

    assert(argc > 1);
    assert(argc < 4);
    if(argc == 3)
        hide = true;
    cap.open(std::string(argv[1]));
    assert(cap.isOpened());

    cv::Mat frame,grey,accum,subimg;
    cv::Mat result(0, 6, CV_32F);
    bool last_frame(false);
    int tracking_wait_start(10);
    int absent_frames(0);
    bool spotted(true);


    while(!last_frame){
        cap>>frame;
        if(frame.empty()|| (absent_frames > N_MAX_ABSENT_FRAMES))
            last_frame = true;
        else{
            spotted = 0;
            cv::cvtColor(frame,grey,CV_BGR2GRAY);

            if(roi.apply(grey,frame,subimg)){
                if(accum.empty())
                    subimg.convertTo(accum,CV_64FC3);
                else
                    cv::accumulateWeighted(subimg,accum,ACCUM_ALPHA);

                accum.convertTo(subimg,CV_8U);
                cv::medianBlur(subimg,subimg,5);
                correctBrightness(subimg);
                cv::threshold(subimg, subimg, THRESHOLD, 255, CV_THRESH_BINARY);
                std::vector<std::vector<cv::Point> > contours, good_contours;
                cv::findContours(subimg,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

                for(auto& i : contours)
                    if( calculateARWH(i)[0] < MAX_AR  && i.size() > MIN_LENGTH){
                        double perim = cv::arcLength(i,true);
                        double area = cv::contourArea(i) ;
                        double circu = 0.08 * perim * perim/ area;
//                        std::cout<<circu<<std::endl;
//                        std::cout<<cv::Point2f(area,circu)<<std::endl;

                        if(circu < MAX_CIRCU && area > MIN_AREA && area < MAX_AREA )
                            good_contours.push_back(i);
                }

                int ncontours = good_contours.size();
                int tframe = cap.get(CV_CAP_PROP_POS_FRAMES);

                for(auto& i : good_contours){
                    std::vector<float> row(6);
                    cv::Vec3f arwh = calculateARWH(i);
                    cv::Moments moms= moments( i , false );
                    cv::Point2f center( moms.m10/moms.m00, moms.m01/moms.m00 );

                    center = roi.getRelativeCoords(center);
                    row[0] = tframe;
                    row[1] = center.x;
                    row[2] = center.y;
                    row[3] = ncontours;
                    row[4] = arwh[1];
                    row[5] = arwh[2];

                    cv::Mat result_row(row);
                    result.push_back(result_row.reshape(1,1));
                }

                if(tracking_wait_start && ncontours == 1)
                    --tracking_wait_start;

                if(ncontours > 0 || tracking_wait_start)
                    absent_frames = 0;
                else
                    ++absent_frames;
                cv::drawContours(frame,good_contours,-1,cv::Scalar(0,250,250),2,CV_AA,cv::noArray(),2,roi.getRect().tl());
                if(!hide){
                    cv::imshow(std::string(argv[1]),frame);
                    cv::waitKey(5);
                }
            }
        }
    }
    std::cout<<cv::format(result,"CSV")<<std::endl;
}
