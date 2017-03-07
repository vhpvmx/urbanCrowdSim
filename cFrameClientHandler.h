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
	bool	b_cmbndSrcImg;

public:

	cFrameClientHandler(){
		initData();
		}

	cFrameClientHandler(const unsigned int width,const unsigned int height){initData();setFramesSizes(width,height);}


	~cFrameClientHandler(){
		deleteData();
	}



	char * getFrameBuffer(){return pc_frameBuffer;}
	char * getDepthBuffer(){return pc_depthBuffer;}

	float * getFrameBufferMyScTyp(){return pf_frameBuffer;}
	float * getDepthBufferMyScTyp(){return pf_depthBuffer;}

	unsigned int getWidth(){return ui_width;}
	unsigned int getHeight(){return ui_height;}

	void printInfo(){
		std::cout<<" UnNormalized data: "
				<<(unsigned int)((unsigned char)pc_frameBuffer[263656])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263657])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263658])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263659])<<","
				<<(unsigned int)((unsigned char)pc_frameBuffer[263660])<<","<<std::endl;

				std::cout<<" Pointer: "<<pf_frameBuffer<<std::endl;
				std::cout<<" Normalized data: "
				<<pf_frameBuffer[263656]<<","
				<<pf_frameBuffer[263657]<<","
				<<pf_frameBuffer[263658]<<","
				<<pf_frameBuffer[263659]<<","
				<<pf_frameBuffer[263660]<<","<<std::endl;

				std::cout<<" Normalized data depth buffer: "
								<<pf_depthBuffer[0]<<std::endl;
	}

	bool processDataBuffer(std::stringstream * val){

		unsigned int aux = val->str().length();
		unsigned int tLenght = ui_BffrSzBytes + 1;

		if(aux==0 ||( aux!=(tLenght) && aux!=(tLenght=(tLenght+ui_BffrSz))))
			return false;

		val->str().copy(pc_frameBuffer,ui_BffrSzBytes,1);

		if(tLenght != ui_BffrSzBytes+1){
			b_cmbndSrcImg = false;
			val->str().copy(pc_depthBuffer,ui_BffrSz,ui_BffrSzBytes+1);
		}



		normalizeData();
		return true;
	}

	void normalizeData(){
		unsigned int x,conta=3;
		if(pf_frameBuffer==nullptr||pf_depthBuffer==nullptr||pc_frameBuffer==nullptr||pc_depthBuffer==nullptr)
			return;

		for(x=0;x<ui_BffrSzBytes;++x)
			pf_frameBuffer[x]=(float)((unsigned int)((unsigned char)pc_frameBuffer[x]))/255.0;

		if(!b_cmbndSrcImg){
			for(x=0;x<ui_BffrSz;++x){
				pf_depthBuffer[x]=(float)((unsigned int)((unsigned char)pc_depthBuffer[x]))/255.0;

				/////////////////Combining both buffers in one /////////////////
				pf_frameBuffer[conta]=pf_depthBuffer[x];
				conta+=4;
				/////////////////Combining both buffers in one /////////////////
			}
		}

		//printInfo();
	}

	void initData(){
		pc_frameBuffer = pc_depthBuffer = nullptr;
		pf_frameBuffer = pf_depthBuffer = nullptr;
		ui_width=ui_height=ui_BffrSz=ui_BffrSzBytes=0;
	}

	void setFramesSizes(const unsigned int width,const unsigned int height){
		deleteData();
		ui_width = width;
		ui_height = height;
		ui_BffrSz = width*height;
		ui_BffrSzBytes = ui_BffrSz*4;
		pc_frameBuffer = new char[ui_BffrSzBytes];
		pc_depthBuffer = new char [ui_BffrSz];
		pf_frameBuffer = new float[ui_BffrSzBytes];
		pf_depthBuffer = new float[ui_BffrSz];
		b_cmbndSrcImg = true;
	};

	void deleteData(){
		//std::cout<<"Destructor frmclinthndlr: "<<this<<std::endl;
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

	void setCmbndSrcImg(bool flg){b_cmbndSrcImg = flg;}
};
