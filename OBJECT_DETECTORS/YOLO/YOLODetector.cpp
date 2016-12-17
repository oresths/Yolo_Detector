#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "YOLODetector.hpp"
using namespace boost::filesystem;


void YOLODetector::Init(const vector<string> &arguments, const eSensor sensor)
{
    this->mSensor = sensor;
    configurationFileRelPath = arguments[0];
    weightsFileRelPath = arguments[1];
    for(int i=2; i<arguments.size(); i++)
        targetLabels.push_back(arguments[i]);
}

void YOLODetector::Shutdown(void){}

vector<cv::Rect>* YOLODetector::DetectStereo(const cv::Mat &imLeft, const cv::Mat &imRight, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output)
{
    if(this->mSensor!=STEREO)
    {
        cerr << "ERROR: you called DetectStereo but input sensor was not set to STEREO." << endl;
        exit(-1);
    }

    //YOLO is a MONOCULAR detector!
    DetectMonocular(imLeft, timestamp, labels, probs, output);
}

vector<cv::Rect>* YOLODetector::DetectRGBD(const cv::Mat &im, const cv::Mat &depthmap, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output)
{
    if(this->mSensor!=RGBD)
    {
        cerr << "ERROR: you called DetectRGBD but input sensor was not set to RGBD." << endl;
        exit(-1);
    }

    //YOLO is a MONOCULAR detector!
    DetectMonocular(im, timestamp, labels, probs, output);
}

vector<cv::Rect>* YOLODetector::DetectMonocular(const cv::Mat &im, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output)
{
    if(this->mSensor!=MONOCULAR)
    {
        cerr << "ERROR: you called DetectMonocular but input sensor was not set to Monocular." << endl;
        exit(-1);
    }

    IplImage iplim = im;

    int argc = 4;

    char *argv[argc];
    argv[0] = "./darknet";
    argv[1] = "detect";
    argv[2] = configurationFileRelPath.c_str();
    argv[3] = weightsFileRelPath.c_str();

    detection_t * detection;

    int num_of_targets = 1;
    const char *targets[num_of_targets];
    targets[0] = "person";
    int num_of_detect = darknet(argc, argv, &iplim, &detection, targets, num_of_targets);

    //Read the YOLO predictions from disk and prepare return variables
    vector<cv::Rect>* detectedROIs = new vector<cv::Rect>;
    string label;
    float probability;
    int left, top, right, bottom;

    for (int i = 0; i < num_of_detect; ++i) {
        labels.push_back(std::string(detection[i].targetclass));
//        wcout << detection[i].targetclass << std::endl;
        probs.push_back(detection[i].prob);
        cv::Point tl(detection[i].left, detection[i].top);
        cv::Point br(detection[i].right, detection[i].bottom);
        cv::Rect ROI(tl, br);
        detectedROIs->push_back(ROI);
	}

    free(detection);

    return detectedROIs;
}
