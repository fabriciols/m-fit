class Project
{
	private:
		Video *vdo;
		Frame *frame;
		char *path_cy;
		char *filename_cy;

	public:
		Project(Video *vdo);
		Project(char *filename_cy);

		Video *getVideo();
		Frame *getFrame();
		char  *getPath();
		char  *getFileName();
};
