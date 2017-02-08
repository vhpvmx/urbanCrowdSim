//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#ifndef CGLOBALS_H_
#define CGLOBALS_H_




class FramebufferObject;


// Mouse control variables
float			camAccel				= 25.0f;
bool			lMouseDown				= false;
bool			rMouseDown				= false;
bool			mMouseDown				= false;
bool			ctrlDown		        = false;
int				glut_mod				= 0;

typedef 		float			myScreenType;

// MPI variables.
int 				pid=-1, np=-1;				// stores process Id and number of process
bool 				loadedTexture 	= false;
myScreenType		**colorPixels 	= 0; 		// stores color pixels for each node
myScreenType		*depthPixels	= 0; 		// stores depth for each node
unsigned int		*screenTexIds 	= 0; 		// stores screen textures (color and depth)
unsigned int 		pixelSizeMsg 	= 0;		// stores the size of the color pixels MPI message
bool				running			= true;		// controls the glLoop function used by workers

FramebufferObject 	*fboMaster;
unsigned int		fboMasterColorTexId;
unsigned int		fboMasterDepthTexId;

#endif /* CGLOBALS_H_ */
