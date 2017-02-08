//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================

#include <cBroadcastServer.h>
#include "cMacros.h"
#include "cMouseEventHandler.h"
#include "cKeyboardHandler.h"

unsigned char 	webSocketKey;
//extern bool		keyChangedFlag;




broadcast_server::broadcast_server()
{
	 m_server.init_asio();

	 // set up access channels to only log interesting things
	m_server.clear_access_channels(websocketpp::log::alevel::all);
	m_server.set_access_channels(websocketpp::log::alevel::connect);
	m_server.set_access_channels(websocketpp::log::alevel::disconnect);
	//m_server.set_access_channels(websocketpp::log::alevel::app);


	 m_server.set_open_handler(bind(&broadcast_server::on_open,this,::_1));
	 m_server.set_close_handler(bind(&broadcast_server::on_close,this,::_1));
	 m_server.set_message_handler(bind(&broadcast_server::on_message,this,::_1,::_2));

	 needMoreFrames = false;
	 stop 			= true;

	 mouseHandler		= 0;
	 keyboardHandler	= 0;
	 pc_frmClientHandler = nullptr;
	 setParamsFrmCltHndlr(512,512);////////// This is a temporal measure, the size of the frame data parameters
	 	 	 	 	 	 	 	   ////////// must not be static!!!!

}
//
//=======================================================================================
//
broadcast_server::~broadcast_server()
{
	// TODO: stop the server

}
//
//=======================================================================================
//
void broadcast_server::on_open(connection_hdl hdl)
{
       m_connections.insert(hdl);
}
//
//=======================================================================================
//
void broadcast_server::on_close(connection_hdl hdl)
{
    m_connections.erase(hdl);
}
//
//=======================================================================================
//
void broadcast_server::on_message(connection_hdl hdl, server::message_ptr msg)
{
	// TODO: Process Interaction msgs
	std::stringstream val;
	int type;
	int buttonMask;
	int xPosition;
	int yPosition;

	int keyMask;

    for (auto it : m_connections)
    {
    	try
    	{
    		val << msg->get_payload();

    		if (!stop)
    		{
				type = val.str().data()[0];

				if (type == MOUSE_EVENT)
				{

					buttonMask = val.str().data()[1];
					xPosition = (unsigned char)val.str().data()[2]*255 + (unsigned char)val.str().data()[3];
					yPosition = (unsigned char)val.str().data()[4]*255 + (unsigned char)val.str().data()[5];

					if (mouseHandler)
					{
						mouseHandler->setCoords	( xPosition, yPosition	);
						mouseHandler->setButton ( buttonMask			);
						mouseHandler->refresh	( true 					);

						//std::cout << "button: " << mouseHandler->getButton() << " state: " << mouseHandler->getState() << std::endl;
						//std::cout << "x: " << xPosition << " y: " << xPosition << std::endl;
					}
					else
					{
						std::cout << "Is Mouse handler initialized? \n";
					}

				}else if (type == KEY_EVENT)
				{
					keyMask = (unsigned char)val.str().data()[4]*255 + (unsigned char)val.str().data()[5]*255 + (unsigned char)val.str().data()[6]*255 + (unsigned char)val.str().data()[7];

					if (keyboardHandler)
					{
						keyboardHandler->setState 	( val.str().data()[1] 	);
						keyboardHandler->setKey   	( keyMask				);
						keyboardHandler->refresh	( true 					);


						//if (keyboardHandler->getState()==cKeyboardHandler::DOWN)
						//std::cout << "Key: " << keyboardHandler->getKey () << std::endl;
					}
					else
					{
						std::cout << "Is keyboard handler initialized? \n";
					}
					//std::cout << "rstate: " << val.str().data()[1] << "cstate: " << keyboardHandler->getState() << std::endl;
				}
				else if(type == CLIENT_FRAME_EVENT){
					std::cout<<"CLIENT_FRAME_EVENT first byte of data is "<< (unsigned int)((unsigned char)val.str().data()[0])<<std::endl;
						if(!(pc_frmClientHandler->processDataBuffer(&val)))
							std::cout<<"Error at sending the frameData from the client"<<std::endl;
				}
    		}
    		if (val.str().compare("GiveMeMore") == 0 || val.str().compare("STSIM")==0 )
    		{
    			needMoreFrames = true;
    			stop = false;
    		}
    		if(val.str().compare("SendingFrameBuffer") == 0){
    			std::cout << "Sendig the 3D background\n";
    		}
    		if ( val.str().compare("END  ") == 0 )
    		{
    			stop = true;
    			std::cout << "dedos";
    			needMoreFrames = false;
    		}
    	}
    	catch (const websocketpp::lib::error_code& e)
    	{
    		std::cout	<< "SEND failed because: " << e
    					<< "(" << e.message() << ")" << std::endl;
		}
    }

}
//
//=======================================================================================
//
void broadcast_server::run(uint16_t port)
{
    m_server.listen(port);
    m_server.start_accept();
    // Start the ASIO io_service run loop
    try
    {
    	m_server.run();
    }
    catch (const std::exception & e)
    {
    	std::cout << e.what() << std::endl;
    }
    catch (websocketpp::lib::error_code e)
    {
    	std::cout << e.message() << std::endl;
    }
    catch (...)
    {
    	std::cout << "other exception" << std::endl;
    }
}
//
//=======================================================================================
//
void broadcast_server::sendFrame (float *img)
{
	//setFrame (img);

	for (auto it : m_connections)
	{
		try
		{
			// when img is unsigned char
			//m_server.send(it, m_img, (size_t)width*height*3 , websocketpp::frame::opcode::BINARY);
			// when img is float
			m_server.send(it, img, (size_t)WIN_WIDTH*WIN_HEIGHT*3*sizeof(float) , websocketpp::frame::opcode::BINARY);


			needMoreFrames = false;
		}
		catch (const websocketpp::lib::error_code& e)
		{
			std::cout	<< "SEND failed because: " << e
					<< "(" << e.message() << ")" << std::endl;
		}
	}

}
//
//=======================================================================================
//
void broadcast_server::sendFrame (unsigned char *img)
{
	//setFrame (img);



	for (auto it : m_connections)
	{
		try
		{
			// when img is unsigned char
			m_server.send(it, img, (size_t)WIN_WIDTH*WIN_HEIGHT*3 , websocketpp::frame::opcode::BINARY);

			//std::cout << "m_server.send(it, img, (size_t)WIN_WIDTH*WIN_HEIGHT*3 , websocketpp::frame::opcode::BINARY);\n";
			//m_server.send(it, "END  ", websocketpp::frame::opcode::text);

			needMoreFrames = false;
		}
		catch (const websocketpp::lib::error_code& e)
		{
			std::cout	<< "SEND failed because: " << e
					<< "(" << e.message() << ")" << std::endl;
		}
	}

}
//
//=======================================================================================
//
void broadcast_server::setMouseHandler			( cMouseHandler	*mouseH	)
{
	mouseHandler = mouseH;
}
//
//=======================================================================================
//
void broadcast_server::setKeyboardHandler		( cKeyboardHandler	*keyboardH	)
{
	keyboardHandler = keyboardH;
}
//
//=======================================================================================
//
cFrameClientHandler* broadcast_server::getFrmCltHndlr( )
{
	return pc_frmClientHandler;
}
//
//=======================================================================================
//
void broadcast_server::setParamsFrmCltHndlr(const unsigned int& width,const unsigned int& height )
{
	if(pc_frmClientHandler == nullptr)
		pc_frmClientHandler = new cFrameClientHandler(width,height);
	else
		pc_frmClientHandler->setFramesSizes(width,height);
}
