#ifndef PROJECT_INCLUDE
#define PROJECT_INCLUDE
class Project
{
	private:
		Video *vdo;
		Frame *frame;
		char path_cy[256];
		char filename_cy[256];

	public:
		Project();

		int openProject(QString);
		int openVideo(QString);

		std::vector<Transition> transitionList;

		Video *getVideo();
		Frame *getFrame();
		char  *getPath();
		char  *getFileName();
};
#endif
