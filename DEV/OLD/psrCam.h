


class psrCam
{
   public:
     psrCam(int argc, char** argv);
     ~psrCam();
     int psrCamDemo(int argc, char** argv);
     CvCapture* psrCaptureFromAvi(int argc, char* argv[]);  
     void psrVisualRithm(int argc, char** argv);  
     void psrGetDiagonal(IplImage* frame, IplImage* imvr, int f);           
     
};

