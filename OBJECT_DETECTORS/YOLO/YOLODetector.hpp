#ifndef YOLODETECTOR_H
#define YOLODETECTOR_H

#include <string>
#include <vector>
#include <thread>
#include <opencv2/core/core.hpp>
#include <boost/filesystem.hpp>
#include "../ObjectDetector.hpp"
#include "src/darknet.h"

class YOLODetector: public ObjectDetector
{
public:

    void Init(const vector<string> &arguments, const eSensor sensor);
    vector<cv::Rect>* DetectMonocular(const cv::Mat &im, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output = NULL);
    vector<cv::Rect>* DetectStereo(const cv::Mat &imLeft, const cv::Mat &imRight, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output = NULL);
    vector<cv::Rect>* DetectRGBD(const cv::Mat &im, const cv::Mat &depthmap, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output = NULL);
    void Shutdown(void);

protected:
    string configurationFileRelPath;
    string weightsFileRelPath;
    vector<string> targetLabels;
};

#endif // YOLODETECTOR_H
