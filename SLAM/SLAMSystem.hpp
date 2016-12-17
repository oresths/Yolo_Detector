#ifndef SLAMSYSTEM_H
#define SLAMSYSTEM_H

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>

using namespace std;
// Input sensor
enum eSensor
{
    MONOCULAR=0,
    STEREO=1,
    RGBD=2
};

class SLAMSystem
{
	public: 

		/**
		* Initializes this SLAM subsystem. It is a virtual function overriden by any class that implements SLAMSystem.
		* It is called by the user only once, before the actual streaming of video frames.
		* <p>
		* Performs internally all memory and resource allocation, necessary for SLAM operation.
		*
		* @param arguments A list containing any algorithm-specific arguments/parameters in the form of strings. If they are numeric in nature, they have to be converted to numbers inside {@link #Init}.
		* @param sensor The type of video input (e.g., MONOCULAR, STEREO, RGBD)
		* @param bUseViewer A flag that determines whether on-the-fly algorithm visualization (if available) is enabled
		*/
		virtual void Init(const vector<string> &arguments, const eSensor sensor, const bool bUseViewer) = 0;


		/**
		* Feeds this SLAM subsystem with the next monocular video frame. It is a virtual function overriden by any class that implements SLAMSystem.
		* It is called by the user once for every new video frame, if the video input is MONOCULAR.
		*
		* @param im the next video frame 
		* @param timestamp the timestamp of the next video frame in KITTI Dataset format
		*/
		virtual cv::Mat TrackMonocular(const cv::Mat &im, const double &timestamp) = 0;

		/**
		* Feeds this SLAM subsystem with the next stereoscopic video frame. It is a virtual function overriden by any class that implements SLAMSystem.
		* It is called by the user once for every new video frame, if the video input is STEREO.
		*
		* @param imLeft the next left video frame 
		* @param imRight the next right video frame 
		* @param timestamp the timestamp of the next video frame in KITTI Dataset format
		*/
		virtual cv::Mat TrackStereo(const cv::Mat &imLeft, const cv::Mat &imRight, const double &timestamp) = 0;

		/**
		* Feeds this SLAM subsystem with the next RGB-D video frame. It is a virtual function overriden by any class that implements SLAMSystem.
		* It is called by the user once for every new video frame, if the video input is RGBD.
		*
		* @param im the next RGB video frame 
		* @param depthmap the next Depth video frame
		* @param timestamp the timestamp of the next video frame in KITTI Dataset format
		*/
		virtual cv::Mat TrackRGBD(const cv::Mat &im, const cv::Mat &depthmap, const double &timestamp) = 0;

	    /**
	     * Terminates this SLAM subsystem. This method must be called only once, after all video frames have been processed.
	     * It is a virtual function overriden by any class that implements SLAMSystem.
	     * 
	     * @param filename output filename where the history of estimated SLAM keyframe is to be written
	     */
		virtual void Shutdown(void) = 0;

	    /**
	     * Saves estimated keyframe poses in the TUM RGB-D dataset format. This method works for all sensor input. It must be called after {@link #Shutdown}.
	     * It is a virtual function overriden by any class that implements SLAMSystem.
	     * See format details at: <a href="http://vision.in.tum.de/data/datasets/rgbd-dataset">http://vision.in.tum.de/data/datasets/rgbd-dataset</a>
	     * 
	     * @param filename output filename where the history of estimated SLAM keyframe is to be written
	     */
    	virtual void SaveKeyFrameTrajectoryTUM(const string &filename) = 0;
	protected:
	    // Input sensor
	    eSensor mSensor;
};

#endif //SLAMSYSTEM_H