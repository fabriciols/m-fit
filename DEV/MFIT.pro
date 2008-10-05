######################################################################
# Automatically generated by qmake (2.01a) qui 18. set 14:25:43 2008
######################################################################

DEVCPP_PATH = "C:/Dev-Cpp"
OPENCV_PATH = "C:/Program Files/OpenCV"
TEMPLATE = app
TARGET = MFIT
DEPENDPATH += . bin include source
INCLUDEPATH += .
QT += qt3support

FORMS   = Interface/mfit.ui

LIBS =  -L$$OPENCV_PATH/bin  \
		  -L$$OPENCV_PATH/lib  \
		  $$OPENCV_PATH/lib/ml.lib \
		  $$OPENCV_PATH/lib/cv.lib \
		  $$OPENCV_PATH/lib/cvaux.lib \
		  $$OPENCV_PATH/lib/cvcam.lib \
		  $$OPENCV_PATH/lib/cxcore.lib\
		  $$OPENCV_PATH/lib/highgui.lib 

INCPATH += $$DEVCPP_PATH/lib/gcc/mingw32/3.4.2/include \
		  $$DEVCPP_PATH/include/c++/3.4.2/backward \
		  $$DEVCPP_PATH/include/c++/3.4.2/mingw32  \
		  $$DEVCPP_PATH/include/c++/3.4.2  \
		  $$DEVCPP_PATH/include        \
		  $$OPENCV_PATH/cv/include     \
		  $$OPENCV_PATH/cvaux/include  \
		  $$OPENCV_PATH/cxcore/include \ 
		  $$OPENCV_PATH/otherlibs/highgui


# Input
HEADERS += include/Brightness.h \
           include/Color.h \
           include/Contrast.h \
           include/Cut.h \
           include/DetectTransitions.h \
           include/Dissolve.h \
           include/Effect.h \
           include/Fade.h \
           include/Fadein.h \
           include/Fadeout.h \
           include/Filters.h \
           include/Frame.h \
           include/Histogram.h \
           include/Interface.h \
           include/Log.h \
           include/Morphology.h \
           include/Take.h \
           include/Time.h \
           include/Transition.h \
           include/Video.h \
           include/VisualRythim.h \
           include/Project.h \
           include/VideoPlayer.h \
           include/QWidgetTimeline.h 

SOURCES += source/main.cpp \
           source/Brightness.cpp \
           source/Color.cpp \
           source/Contrast.cpp \
           source/Cut.cpp \
           source/DetectTransitions.cpp \
           source/Dissolve.cpp \
           source/Effect.cpp \
           source/Fade.cpp \
           source/Fadein.cpp \
           source/Fadeout.cpp \
           source/Filters.cpp \
           source/Frame.cpp \
           source/Histogram.cpp \
           source/Interface.cpp \
           source/Log.cpp \
           source/Morphology.cpp \
           source/Take.cpp \
           source/Time.cpp \
           source/Transition.cpp \
           source/Video.cpp \
           source/VisualRythim.cpp \
           source/Project.cpp \
           source/VideoPlayer.cpp \
           source/QWidgetTimeline.cpp

CONFIG -= incremental_off
