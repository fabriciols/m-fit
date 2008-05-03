#include "cv.h"
#include "highgui.h"
#include <stdio.h>

int histogram()
{
	int max_value;
	int hist_size = 64;
	int bin_w;

	cvCalcHist( &dst_image, hist, 0, NULL );
	cvZero( dst_image );
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
	cvScale( hist->bins, hist->bins, ((double)hist_image->height)/max_value, 0 );
	/*cvNormalizeHist( hist, 1000 );*/

	cvSet( hist_image, cvScalarAll(255), 0 );
	bin_w = cvRound((double)hist_image->width/hist_size);

	for( i = 0; i < hist_size; i++ )
		cvRectangle( hist_image, cvPoint(i*bin_w, hist_image->height),
				cvPoint((i+1)*bin_w, hist_image->height - cvRound(cvGetReal1D(hist->bins,i))),
				cvScalarAll(0), -1, 8, 0 );

	cvShowImage( "ImagemHistogram", hist_image );
}	

int main( int argc, char** argv )
{
	char c;
	CvCapture* capture = 0;
	IplImage* frame = 0;
	IplImage* framegray = 0;
	IplImage* framecanny = 0;
	IplImage* framesmooth = 0;
	IplImage* framedilate = 0;
	CvHistogram* framehist = 0;

	printf("Opening %s\n",argv[1]);

	capture = cvCaptureFromAVI(argv[1]); 

	cvNamedWindow("Imagem", 1);
	cvNamedWindow("ImagemGray", 1);
	cvNamedWindow("ImagemCanny", 1);
	cvNamedWindow("ImagemSmooth", 1);
	cvNamedWindow("ImagemDilate", 1);
	cvNamedWindow("ImagemHistogram", 1);

	if (!capture)
	{
		printf("File not found\n");
		return -1;
	}

	while(true)
	{
		frame = cvQueryFrame(capture);

		if (!frame)
			break;

		// Todas imagens: 8 bit por pixel, 1 canal ( RGB tem 3 ! )
		if (!framegray)
			framegray = cvCreateImage(cvGetSize(frame),8,1);

		if (!framesmooth)
			framesmooth = cvCreateImage(cvGetSize(frame),8,1);

		if (!framecanny)
			framecanny = cvCreateImage(cvGetSize(frame),8,1);

		if (!framedilate)
			framedilate = cvCreateImage(cvGetSize(frame),8,1);

		if (!framehist)
			framehist = cvCreateImage(cvGetSize(frame),8,1);

		framegray->origin = frame->origin;
		framecanny->origin = frame->origin;
		framesmooth->origin = frame->origin;
		framedilate->origin = frame->origin;
		framehist->origin = frame->origin;

		cvCvtColor(frame, framegray, CV_RGB2GRAY);

		cvCanny(framegray, framecanny, 30, 80, 3);

		cvSmooth(framegray, framesmooth, CV_GAUSSIAN, 15, 0, 0, 0);

		cvDilate(framecanny, framedilate, 0, 1);

		histogram();

		cvShowImage("Imagem", frame);
		cvShowImage("ImagemGray", framegray);
		cvShowImage("ImagemCanny", framecanny);
		cvShowImage("ImagemSmooth", framesmooth);
		cvShowImage("ImagemDilate", framedilate);
		cvShowImage("ImagemHistogram", framehist);

		c = cvWaitKey(30);
		printf("Key -> %c | %d\n",c,c);

		// 27 = ESC
		if ( (char) c == 27 )
			break;

	}

	cvWaitKey(10);

	cvDestroyWindow("Imagem");
	cvDestroyWindow("ImagemGray");
	cvDestroyWindow("ImagemCanny");
	cvDestroyWindow("ImagemSmooth");
	cvDestroyWindow("ImagemDilate");
	cvDestroyWindow("ImagemHistogram");

	cvReleaseImage(&frame);
	cvReleaseImage(&framegray);
	cvReleaseImage(&framecanny);
	cvReleaseImage(&framesmooth);
	cvReleaseCapture(&capture);

	/* Destroy the window */

	return -1;

}

/*
	IplImage* img;

	img = cvLoadImage(argv[1], 1);

	cvNamedWindow("Imagem", 1);
	cvShowImage("Imagem", img);
	cvWaitKey(0);

	return -1;
	*/
