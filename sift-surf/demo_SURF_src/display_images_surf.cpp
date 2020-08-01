/*
 Display images file for SURF
 
 Wrapper to display images and info from generated text files.
 
 Copyright 2013: Edouard Oyallon, Julien Rabin
 
 Version for IPOL.
 */


#include "display_images_surf.h"


using namespace std;

// Display pictures given keypoints, input images and matches
// img1 img2 d1.txt d2.txt matchs.txt output_desc.png  output_matches.png
int main(int argc, char** argv)
{
    
    if(argc!=8)
    {
        cerr<<"Error: not the correct inputs. Please refer to the README for usage."<<endl;
        return EXIT_FAILURE;
    }
    
    string keypoints1=argv[1];
    string keypoints2=argv[2];
    string outputmatch=argv[3];
    string nameImg1=argv[4];
    string nameImg2=argv[5];
    string output_desc=argv[6];
    string output_matches=argv[7];
    
    size_t nx,ny,nx2,ny2;
	unsigned char* data1;
    unsigned char* data2;

    std::vector<Match> match_coor;

    if(!loadMatch(outputmatch.c_str(), match_coor))
        std::cerr<<"Error: matches not loaded.";
    
    // Load descriptors
    listDescriptor* listDesc1=getDescriptorFromRawFile(keypoints1);
    listDescriptor* listDesc2=getDescriptorFromRawFile(keypoints2);
  
    // Images
	if (NULL == (data1 = read_png_u8_gray(nameImg1.c_str(), &nx, &ny)) || nx<2 || ny<2)
    {
        cerr<<"Error: Image 1 is not read. Use a valid 2x2 PNG image (at least) and relaunch the software."<<endl;
        return EXIT_FAILURE;
    }
    

	if (NULL == (data2 = read_png_u8_gray(nameImg2.c_str(), &nx2, &ny2)) || nx2<2 || ny2<2)
    {
        cerr<<"Error: Image 2 is not read. Use a valid 2x2 PNG image (at least) and relaunch the software."<<endl;
        return EXIT_FAILURE;
    }
    
    image* img1=new image((int) nx,(int) ny,data1);
    image* img2=new image((int) nx2,(int) ny2,data2);
    
    // Normalize the image by setting the minimum value of the image to 0 and its maximum value to 256. Several technics like histogram equalization, linearization could be used. Standardization can not be used since the algorithm works with non negative integers.
    img1->normalizeImage();
    img2->normalizeImage();
    
    /* Free check*/
    free(data1);
	free(data2);
    
    
    // Output descriptors on the two images
    image* img1_descriptor=showDescriptors(img1,listDesc1);
    image* img2_descriptor=showDescriptors(img2,listDesc2);
	
	// Stamp image for matches
	image* img=new image(((img1->getWidth()>img2->getWidth())?img1->getWidth():img2->getWidth()),img1->getHeight()+img2->getHeight());
	
	for(int i=0;i<img1->getWidth();i++)
		for(int j=0;j<img1->getHeight();j++)
			(*img)(i,j)=(*img1)(i,j);
	
	for(int i=0;i<img2->getWidth();i++)
		for(int j=0;j<img2->getHeight();j++)
			(*img)(i,j+img1->getHeight())=(*img2)(i,j);
	
	// Stamp image for descriptors
	image* img_descriptor=new image(((img1->getWidth()>img2->getWidth())?img1->getWidth():img2->getWidth()),img1->getHeight()+img2->getHeight());
	
	for(int i=0;i<img1->getWidth();i++)
		for(int j=0;j<img1->getHeight();j++)
			(*img_descriptor)(i,j)=(*img1_descriptor)(i,j);
	
	for(int i=0;i<img2->getWidth();i++)
		for(int j=0;j<img2->getHeight();j++)
			(*img_descriptor)(i,j+img1->getHeight())=(*img2_descriptor)(i,j);
   
    // Initiliazation of the stamp between matches and background
	image* img_descriptor_line=new image(((img1->getWidth()>img2->getWidth())?img1->getWidth():img2->getWidth()),img1->getHeight()+img2->getHeight());
	
    // Draw matches
	for(int i=0;i<(int)match_coor.size();i++)
        line(img_descriptor_line,match_coor[i].x1,match_coor[i].y1,match_coor[i].x2,match_coor[i].y2+img1->getHeight(),140);

	
    cout<<"Image 1: "<<listDesc1->size()<<" keypoints."<<endl;
    cout<<"Image 2: "<<listDesc2->size()<<" keypoints."<<endl;
	cout<<"Matches: "<<match_coor.size()<<" points."<<endl;

    
	//We print result.
	img->printImagePara((char*)output_desc.c_str(),img_descriptor);// We print the descriptors alone.
    img->printImagePara((char*)output_matches.c_str(),img_descriptor_line);// We show the matches between 2 pictures.
   
    /*MemCheck*/
	for(int i=0;i<(int)listDesc1->size();i++)
		delete((*listDesc1)[i]);
	for(int i=0;i<(int)listDesc2->size();i++)
		delete((*listDesc2)[i]);
    delete listDesc1;
    delete listDesc2;
    delete img1_descriptor;
    delete img2_descriptor;
    delete img;
    delete img1;
    delete img2;
    delete img_descriptor;
	delete img_descriptor_line;

    return EXIT_SUCCESS;
}



// Make a lign between {a,b} points for a given intensity.
void line(image *img,float xa,float ya,float xb,float yb,float intensite)
{
	xa=(int) xa;ya=(int)ya;xb=(int)xb;yb=(int)yb;
	if(xa>xb)
	{
		line(img,xb,yb,xa,ya,intensite);
	}
	
	else
	{
		if(xb==xa)
		{
			if(ya<yb)
				for(int i=0;i<=yb-ya;i++)
					(*img)(xa,ya+i)=intensite;
			else
				for(int i=0;i<=ya-yb;i++)
					(*img)(xa,yb+i)=intensite;
		}
		else
		{
			float a,b;
			a=(float)(yb-ya)/(xb-xa);
			b=ya;
			for(int i=0;i<=xb-xa+1;i++)
			{
                if(a>0)
                {
					for(int j=(int)((float)(a*i)+b);j<(int)((float)(a*(i+1))+b);j++)
						if ( (j>ya && j<yb) || (j>yb && j<ya))
                            
                            (*img)(xa+i,j)=intensite;
                }
                else
                    for(int j=(int)((float)(a*(i+1))+b);j<(int)((float)(a*i)+b);j++)
                        if ( (j>ya && j<yb) || (j>yb && j<ya))
                            (*img)(xa+i,j)=intensite;
			}
		}
	}
}


// Show descriptors by drawing a circle whose radius is proportional to the
// detected scale.
image* showDescriptors(image* img1,listDescriptor* listeDesc)
{
	image* img=new image(img1);
	
    for(int i=0;i<(int)listeDesc->size();i++)
	{
		float angle=((*listeDesc)[i]->kP)->orientation;
		
		float ech=((*listeDesc)[i]->kP)->scale;
		int x0=((*listeDesc)[i]->kP)->x;
		int y0=((*listeDesc)[i]->kP)->y;
		
        // Draw a circle
        for(int i=0;i<1000;i++)
        {
            int x=x0+cosf(i*2*pi/1000)*fround(2.5*ech);
            int y=y0+sinf(i*2*pi/1000)*fround(2.5*ech);
            if(x>=0 && y>=0 && x<img->getWidth() && y<img->getHeight())
                (*img)(x,y)=140;
        }
        
        for(int a=0;a<fround(2.5*ech)+1;a++)
        {
            int x=x0+a*cos(angle);
            int y=y0+a*sin(angle);
            if(x>=0 && y>=0 && x<img->getWidth() && y<img->getHeight())
                (*img)(x,y)=50;
            
        }
	}
	return img;		
}
