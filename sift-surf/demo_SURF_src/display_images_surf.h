/*
 Display images header for SURF
 
 Wrapper to display images and info from generated text files.
 
 Copyright 2013: Edouard Oyallon, Julien Rabin
 
 Version for IPOL.
 */


#include <ctime>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "descriptor.h"
#include "image.h"
#include "extern_files_used/io_png.h"
#include "extern_files_used/libMatch/match.h"
#include "match_surf.h"

#ifndef SURF_DISPLAY
#define SURF_DISPLAY

// Show descriptors represented by a circle whose radius is proportional to the scale.
image*	 showDescriptors(image* img1,listDescriptor* listeDesc);

// Draw a line
void line(image *img,float xa,float ya,float xb,float yb,float intensite);

#endif
