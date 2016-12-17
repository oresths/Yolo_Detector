#include "main_test.hpp"

int main(int argc, const char** argv) {
	cv::VideoCapture cap(
			"/mnt/hgfs/Vision/Dataset UAV123/UAV123/data_seq/UAV123/bike1/%06d.jpg");
	// Check if video device can be opened with the given index
	if (!cap.isOpened())
		return 1;

	cv::Mat frame;

	cv::Rect init_ROI(707, 362, 40, 97);
	eSensor camera = MONOCULAR;
	vector<string> args;

	vector<string> labels;
	vector<float> probs;

	args.push_back("./cfg/yolo.cfg");
	args.push_back("../../../BACKUP/yolo.weights");
	args.push_back("person");

	YOLODetector detector;

	detector.Init(args, camera);

	while (1) {
		cap >> frame;

		// Check if grabbed frame is actually full with some content
		if (!frame.empty()) {
			detector.DetectMonocular(frame, 1.0, labels, probs);

			break;
		}

		usleep(10000);
	}

	return 0;
}


