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

		int openProject(char *filename_cy);
		int openVideo(QString fileName);

		std::vector<Transition> transitionList;

		Video *getVideo();
		Frame *getFrame();
		char  *getPath();
		char  *getFileName();
};
#endif
