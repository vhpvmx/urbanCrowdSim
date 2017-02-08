
Description:

This example renders  sets of characters within a virtual environment that can be viewed from a web browser.  
Each node from a GPU cluster renders a crowd, then the framebuffer is captured and the resultant image with 
depth is sent to the master node. In the master node sort-last composition is performed. Then, the final 
composite is sent to a websocket client where the user can interact with the simulation using a web browser. 

--------------------

This project needs the following libraries previously installed:

-ASSIMP version 3. You might need compile ASSIMP's source code in your system in order to get version 3.

Try to get the last version of these libraries:

-GLEW
-X11
-Freeglut
-GLU
-BOOST (boost_regex, boost_system)
-glm
-OpenMPI or any other compatible with MPI standard
-pthreads
-Any image transport library (e.g. virtualGL) installed in your cluster.



Your GPU card must support OpenGL 4.2

--------------------

Inside the crowdVizSimpleMPIWebsockets directory you will find the next directories:

Debug/        	- contains binaries and makefiles
Header/       	- contains the header files
Source/			- contains the source files. The file main.cpp is the entry point of the project
media/			- contains 3D models
shaders/		- contains shaders in GLSL 4.2 
HTML/			- contains the websocket client (client.html)

--------------------

You can compile the project using Eclipse. The project was developed using Nsight Eclipse (distribuited in CUDA 5.5 or 6.0) a modified 
version of Eclipse IDE by Nvidia. 
In addition you will find the makefiles of the project in the  Debug directory, you can change it according to your needs, however you 
might need to change *.mk files inside the Debug/Source directory.

Websockets library is included in the project files. However in order to compile this example, you will need your g++ compiler supports
c++11. Please check this link:

https://gcc.gnu.org/projects/cxx0x.html


--------------------

To run the app, in the terminal:

1. Run the websocket server:

mpi -np <N> <path/name of the executable>

where N is the number of nodes, it has been tested for N=1 to 9 nodes.


2. Run the websocket client:

Once the demo is running, go to HTML directory and open the file client.html, you will see in the console  a message similar to this:

" [2014-07-28 17:25:14] [connect] WebSocket Connection [::ffff:127.0.0.1]:53126 v13 "Mozilla/5.0 (X11; Linux x86_64) 
AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.153 Safari/537.36" / 101 "

This messages acknowledges the acceptance of websocket connection between the visualizer and the web browser. Click the play button 
and you can see the simulation running. You can interact with the mouse and WASD keys.

IMPORTANT:

You will notice that the visualization is done using windowless rendering. This version does not implement a  mechanism to close the application 
from the web browser yet. Thus, you likely need to close it  by pressing ctrl+C in the terminal, using the 'kill' 
command or wait until it is closed by the job queue system in the case of the cluster.  

--------------------



