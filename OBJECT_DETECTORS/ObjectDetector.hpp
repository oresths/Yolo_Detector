#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include "../SLAM/SLAMSystem.hpp"

using namespace std;

class ObjectDetector
{
	public: 

	    virtual void Init(const vector<string> &arguments, const eSensor sensor) = 0;
	    virtual vector<cv::Rect>* DetectMonocular(const cv::Mat &im, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output = NULL) = 0;
	    virtual vector<cv::Rect>* DetectStereo(const cv::Mat &imLeft, const cv::Mat &imRight, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output = NULL) = 0;
	    virtual vector<cv::Rect>* DetectRGBD(const cv::Mat &im, const cv::Mat &depthmap, const double &timestamp, vector<string> &labels, vector<float> &probs, cv::Mat* output = NULL) = 0;
	    virtual void Shutdown(void) = 0;

	protected:
	    // Input sensor
	    eSensor mSensor;
};

#endif //OBJECTDETECTOR_H