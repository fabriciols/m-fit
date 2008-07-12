/* &&&&&&&&&&&&&&&  GENERAL INFORMATION &&&&&&&&&&&&&&&&&&&&

&&&&&&&&&&&  General Author Information &&&&&&&&&&&&&&&&&
This program was done and has been continuouly updated 
by Prof. D.Sc. Paulo Sérgio Rodrigues from Computer Science
and Electrical Department of Technological Center of FEI, 
São Bernardo do Campo, São Paulo, Brazil, and is free for copy 
and modifications if you make the approrpiate citations. 
The author's home page, where you can find other projects 
and interest areas in http://www.fei.edu.br/~psergio and 
the his e-mail is psergio@fei.edu.br if you wish toutch him
for critical, sugestions or collaborations. 

&&&&&&&&&&& General System Information and Goal &&&&&&&&&&&&&&&&

&&&&&&&&&& Input-OutPut Parameters &&&&&&&&&&&&&&&&&&&&&&&&&&&&&

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/


#include <cstdlib>
#include <iostream>

// C++ and C Libraries
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

// OpenCV Libraries
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "ml.h"

// System includes
#include "psrCam.h"

using namespace std;

int main(int argc, char *argv[])
{
    
    psrCam CAM(argc,argv);
    
    return EXIT_SUCCESS;
}




