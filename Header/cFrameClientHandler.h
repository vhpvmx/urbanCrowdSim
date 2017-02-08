//
//										Jorge E. Ramirez Flores, PhD. 	jramfl@gmail.com
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#ifndef CFRAMECLIENTHANDLER_H_
#define CFRAMECLIENTHANDLER_H_





#endif /* CFRAMECLIENTHANDLER_H_ */


#define CLIENT_FRAME_EVENT	7		///This code is made up according to our needs,
									///is not specified in the RFB protocol

class cFrameClientHandler{

private:
	char * pc_frameBuffer;
	char * pc_depthBuffer;
	float * pf_frameBuffer;
	float * pf_depthBuffer;
	unsigned int ui_width;
	unsigned int ui_height;
	unsigned int ui_BffrSz;
	unsigned int ui_BffrSzBytes;


public:
	cFrameClientHandler(){
		pc_frameBuffer = pc_depthBuffer = nullptr;
		pf_frameBuffer = pf_depthBuffer = nullptr;
		ui_width=ui_height=ui_BffrSz=ui_BffrSzBytes=0;
	};

	cFrameClientHandler(const unsigned int width,const unsigned int height){setFramesSizes(width,height);}

	~cFrameClientHandler(){
		deleteData();
	};

	char * getFrameBuffer(){return pc_frameBuffer;}
	char * getDepthBuffer(){return pc_depthBuffer;}

	float * getFrameBufferMyScTyp(){return pf_frameBuffer;}
	float * getDepthBufferMyScTyp(){return pf_depthBuffer;}

	unsigned int getWidth(){return ui_width;}
	unsigned int getHeight(){return ui_height;}

	bool processDataBuffer(const std::stringstream * val){
		unsigned int tLenght = ui_BffrSzBytes+ui_BffrSzBytes+1;
		unsigned int aux = val->str().length();
		//char *data = new char[tLenght];

		//std::cout<<" Successful data buffer sended with a size of : "<<aux<<" bytes"<<std::endl;
		//std::cout<<" Estimated data size: "<<tLenght<<std::endl;
		if(aux==0 || aux!=(tLenght))
			return false;

		//data = val->str().c_str();
		//val->str().copy(data,tLenght,0);
		val->str().copy(pc_frameBuffer,ui_BffrSzBytes,1);
		val->str().copy(pc_depthBuffer,ui_BffrSzBytes,ui_BffrSzBytes+1);
		std::cout<<" First byte of information: "
				<<(unsigned int)((unsigned char)pc_frameBuffer[263656])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263657])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263658])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263659])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263660])<<","<<std::endl;

		std::cout<<"Congruence test, first byte of information is "
				<<(unsigned int)((unsigned char)pc_depthBuffer[263656])<<","
				<<(unsigned int)((unsigned char)pc_depthBuffer[263657])<<","
				<<(unsigned int)((unsigned char)pc_depthBuffer[263658])<<","
				<<(unsigned int)((unsigned char)pc_depthBuffer[263659])<<","
				<<(unsigned int)((unsigned char)pc_depthBuffer[263660])<<","<<std::endl;

		return true;
	}

	void normalizeData(){
		if(pf_frameBuffer==nullptr||pf_depthBuffer==nullptr||pc_frameBuffer==nullptr||pc_depthBuffer==nullptr)
			return;
		for(unsigned int x=0;x<ui_BffrSzBytes;++x){
			pf_frameBuffer[x]=(float)pc_frameBuffer[0]/255.0;
			pf_depthBuffer[x]=(float)pc_depthBuffer[0]/255.0;
		}
	}

	void setFramesSizes(const unsigned int width,const unsigned int height){
		deleteData();
		ui_width = width;
		ui_height = height;
		ui_BffrSz = width*height;
		ui_BffrSzBytes = ui_BffrSz*4;
		pc_frameBuffer = new char[ui_BffrSzBytes];
		pc_depthBuffer = new char [ui_BffrSzBytes];
		pf_frameBuffer = new float[ui_BffrSzBytes];
		pf_depthBuffer = new float[ui_BffrSzBytes];
	};

	void deleteData(){
		try{
			if(pc_frameBuffer!=nullptr)
				delete pc_frameBuffer;
			if(pf_frameBuffer!=nullptr)
				delete pc_frameBuffer;

			if(pc_depthBuffer != nullptr)
				delete pc_depthBuffer;
			if(pf_depthBuffer != nullptr)
				delete pc_depthBuffer;
			}
		catch(...){}
		ui_width=ui_height=ui_BffrSz=ui_BffrSzBytes=0;
	}


};
