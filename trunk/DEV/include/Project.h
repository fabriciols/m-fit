#ifndef PROJECT_INCLUDE
#define PROJECT_INCLUDE
class Project
{
	private:

		Video *vdo;
		Frame *frame;
		char path_cy[256];
		char filename_cy[256];

		int userCutThreshold; // Este limiar ser� em %, o que representa a % da altura do frame.
		int userMinCanny;
		int userMaxCanny;

		void clearTransitionList();

	public:

		Project();

		int openProject(QString);
		int saveProject(QString);
		int openVideo(QString);

		std::vector<Transition> transitionList;  // Transicoes no video
		std::vector<Effect*>    effectList;      // Efeitos no video

		Video *getVideo();
		Frame *getFrame();
		char  *getPath();
		char  *getFileName();

		void setUserThreshold(int threshold);
		int getUserThreshold();

		void setUserMinCanny(int userMinCanny);
		int getUserMinCanny();

		void setUserMaxCanny(int userMaxCanny);
		int getUserMaxCanny();

		void sortTransitionList(void);

		long FrameToTimelinePos(long frame);
		long TimelinePosToFrame(long pos);

		void renderVideo(char *filename_cy);

		Frame* applyEffect(Frame *frame, long pos);


};
#endif
