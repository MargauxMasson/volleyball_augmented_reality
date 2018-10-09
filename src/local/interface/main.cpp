
#include <QApplication>

#include "myWindow.hpp"
#include "../../lib/interface/application_qt.hpp"

#include <QCoreApplication>
//
//  main.cpp
//  detectionMAIN_versionmain
//
//  Created by Margaux on 15/05/2017.
//  Copyright © 2017 Margaux. All rights reserved.
//

#include <GL/glew.h>
#include <QMouseEvent>



#include "../../lib/opengl/glutils.hpp"
#include "../../lib/perlin/perlin.hpp"
#include "../../lib/interface/camera_matrices.hpp"
#include "../interface/myWidgetGL.hpp"
#include "../../lib/mesh/mesh_io.hpp"

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "iostream"
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "../../local/interface/scene.hpp"


using namespace std;



int main(int argc, const char * argv[]) {
    scene scene_3d;
    int ag = argc;
    CvCapture* capture = cvCreateCameraCapture( CV_CAP_ANY );
     if( !capture )
         return 1;
     if(!cvGrabFrame( capture ))
         return 2;
     int key;

    scene_3d.detection(ag,capture,key);


    return 0;

}







