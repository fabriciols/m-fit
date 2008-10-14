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

	public:
		Project();

		int openProject(QString);
		int saveProject(QString);
		int openVideo(QString);

		std::vector<Transition> transitionList;

		Video *getVideo();
		Frame *getFrame();
		char  *getPath();
		char  *getFileName();

		void setUserThreshold(int threshold);
		int getUserThreshold();

};
#endif
