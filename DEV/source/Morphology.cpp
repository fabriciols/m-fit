#include "../include/Morphology.h"

Frame* Morphology::dilate(Frame* frame)
{
	IplImage *img_dst;

	img_dst = cvCreateImage(cvGetSize(frame->data),frame->data->depth,frame->data->nChannels);

	cvDilate(frame->data, im_dst, 0, 1);

	return (new Frame(img_dst));
}
