
#ifndef SRC_DARKNET_H_
#define SRC_DARKNET_H_

#include <string.h>

#ifdef OPENCV
#include "opencv2/imgproc/imgproc_c.h"
#endif

typedef struct
{
	char targetclass[32];
	float prob;
    int left;
    int top;
    int right;
    int bottom;

} detection_t;

int darknet(int argc, char **argv, IplImage * iplim, detection_t ** detection, char ** targets, int num_of_targets);

#endif /* SRC_DARKNET_H_ */
