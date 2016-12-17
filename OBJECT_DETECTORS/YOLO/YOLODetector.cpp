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
    predictionsFilename = "objectPredictions.txt";
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

    //Save current video frame to disk and feed it to DARKNET YOLO external program
    cv::imwrite("./OBJECT_DETECTORS/YOLO/currentFrame.jpg", im);
    string command = "./OBJECT_DETECTORS/YOLO/darknet detect " + configurationFileRelPath + ' ' + weightsFileRelPath + " ./OBJECT_DETECTORS/YOLO/currentFrame.jpg";
    system(command.c_str());

    //Read the YOLO predictions from disk and prepare return variables
    vector<cv::Rect>* detectedROIs = new vector<cv::Rect>;
    std::ifstream predictionsFile;
    predictionsFile.open(predictionsFilename.c_str());
    string label;
    float probability;
    int left, top, right, bottom;
    while(predictionsFile.is_open() && (!predictionsFile.eof()))
    {
        predictionsFile >> label;
        labels.push_back(label);
        predictionsFile >> probability;
        probs.push_back(probability);
        predictionsFile >> left >> top >> right >> bottom;
        cv::Point tl(left, top);
        cv::Point br(right, bottom);
        cv::Rect ROI(tl, br);
        detectedROIs->push_back(ROI);
    }
    predictionsFile.close();

    return detectedROIs;
}
