/*
 Match file for SURF
 
 Copyright 2013: Edouard Oyallon, Julien Rabin
 
 Version for IPOL.
 */

#include "match_surf.h"
#include "lib_match_surf.h"

using namespace std;

// This executive computes the matches given 2 sets of descriptors
// d1.txt d2.txt output_matches.txt -orsa
int main(int argc, char** argv)
{

    if(argc!=4 && argc!=5)
    {
        cerr<<"Error: Missing arguments. Please refer to the README for usage."<<endl;
        return EXIT_FAILURE;
    }
    
    string keypoints1=argv[1];
    string keypoints2=argv[2];
    string outputmatch=argv[3];
    
    // ORSA ?
    bool isOrsa=false;
    if(argc>4)
        isOrsa=true;

    
    
    // Recover the list of descriptors
    listDescriptor* listDesc1=getDescriptorFromRawFile(keypoints1);
    listDescriptor* listDesc2=getDescriptorFromRawFile(keypoints2);
    
    // Matching
    std::vector<Match> match_coor=matchDescriptor(listDesc1,listDesc2);
    
    // Cleaning identical matches
    cleanMatch(&match_coor);
    
    // Cleaning via ORSA
    if(match_coor.size()>10 && isOrsa)
        match_coor=cleanMatchORSA(match_coor);
    
    // Save matches
    saveMatch(outputmatch.c_str(), match_coor);
    
    /*MemCheck*/
	for(int i=0;i<(int)listDesc1->size();i++)
		delete((*listDesc1)[i]);
	for(int i=0;i<(int)listDesc2->size();i++)
		delete((*listDesc2)[i]);
    
    delete listDesc1;
    delete listDesc2;

    return EXIT_SUCCESS;
}




