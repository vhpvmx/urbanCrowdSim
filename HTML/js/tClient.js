var output;
var wsUri	= "ws://localhost:9005/";
var reader	= new FileReader();
var ctx;
var stop 	= false;

var gl;
var rttFrameBuffer;
var rttTexture;
var renderbuffer;
var data=[0,1,2];
var tmplin=[0,1,2];
var canvas2;
var contextCanvas2;
var canvas3;
var contextCanvas3;
var canvas4;
var contextCanvas4;
var ImageData;
var ImageData2;
var ImageData3;
var websocket;

function init(){
	if (window.WebSocket) {
		connectAndCallbacks();
	}
	else
	{
		console.log ("This browser does not support Websocket.");
	}

	// adding mouse events
	canvas2 = document.getElementById('canvas2');

	canvas2.addEventListener( "contextmenu", preventDefaultHandler,	true );
	canvas2.addEventListener( "mousedown", 	mouseDownHandler,	true );
    canvas2.addEventListener( "mouseup",	mouseUpHandler, 	true );
    canvas2.addEventListener( "mousemove", 	mouseMoveHandler, 	true );
    canvas2.addEventListener( "mousewheel", 	mouseWheelHandler, 	true );

    document.addEventListener("keydown", 	keyDownHandler, 	false );
    document.addEventListener("keyup", 	keyUpHandler, 		false );

	contextCanvas2 	= canvas2.getContext('2d');
	ImageData = contextCanvas2.getImageData(0,0,canvas2.width,canvas2.height);
}

function connectAndCallbacks ()
{
	websocket			= new WebSocket(wsUri);
	websocket.onopen 	= function(evt) { onOpen	(evt)	};
	websocket.onclose 	= function(evt) { onClose	(evt)	};
	websocket.onmessage	= function(evt) { onMessage	(evt)	};
	websocket.onerror 	= function(evt) { onError	(evt)	};

	// sets websocket's binary messages as ArrayBuffer type
	//websocket.binaryType = "arraybuffer";
	// sets websocket's binary messages as Blob type by default is Blob type
	//websocket.binaryType = "blob";

	reader.onload		= function(evt) { readBlob		(evt) };
	reader.onloadend	= function(evt) { nextBlob		(evt) };
	reader.onerror		= function(evt) { fileReaderError 	(evt) };

	peripherals		= new Peripherals ();
	peripherals.init(websocket);
}

function onOpen(evt)
{

}

function fileReaderError (e)
{
	console.error("FileReader error. Code " + event.target.error.code);
}

// this method is launched when FileReader ends loading the blob
function nextBlob (e)
{
	if (!stop)
		websocket.send ("GiveMeMore");
	//console.log (stop);
}

/// this method is launched when the user is having interaction with the xml3d element
function send3DElement (){
	if(!stop)
		websocket.send(ImageData3);
		//websocket.send("SendingFrameBuffer");
}

// readBlob is called when reader.readAsBinaryString(blob); from onMessage method occurs
function readBlob (e)
{
	//console.log (canvas.width, canvas.height );
	var i,j=0;

    // if receiving float images
	//img = new Float32Array(reader.result);

	// if receiving unsigned byte images
	img = new Uint8Array(reader.result);

//console.log (imgdata.data.length);

// TODO: send data as  picture format to avoid this:

	//console.log ("result: ", + reader.target.result);
//	console.log("Blob size ", + reader.result.length + " ", + reader.result[10]);
//	console.log("Blob size ", + img.length + " ", + img[10]);


	// if img is float
/*
    for(i=0;i<imgdata.data.length;i+=4)
	{
		imgdata.data[i]		= img[j  ] * 255; //.charCodeAt(j);
		imgdata.data[i+1] 	= img[j+1] * 255; //reader.result.charCodeAt(j+1);
		imgdata.data[i+2] 	= img[j+2] * 255; //reader.result.charCodeAt(j+2);
		imgdata.data[i+3] 	= 255;
		j+=3;
	}
*/

 	 for(i=0;i<ImageData.data.length;i+=4)
	{
		ImageData.data[i]		= img[j  ] ; //.charCodeAt(j);
		ImageData.data[i+1] 	= img[j+1] ; //reader.result.charCodeAt(j+1);
		ImageData.data[i+2] 	= img[j+2] ; //reader.result.charCodeAt(j+2);
		ImageData.data[i+3] 	= 255;
		j+=3;
	}

	contextCanvas2.putImageData(ImageData,0,0);

}

// message from the server
function onMessage(e)
{
	if (typeof e.data == "string")
	{
		console.log ("String msg: ", e, e.data);
	}
	else if (e.data instanceof Blob)
	{
		var blob = e.data;

		//console.log ("Blob msg: ", blob);

		//reading data from Blob
		//reader.readAsBinaryString(blob);
		reader.readAsArrayBuffer(blob);

	}
	else if (e.data instanceof ArrayBuffer)
	{
		var img = new Uint8Array(e.data);

		console.log("Array msg:", + img[63] );


	}


	//websocket.close ();
}


// is there is an error
function onError (e)
{
	console.log("Websocket Error: ", e);
	// Custom functions for handling errors
	// handleErrors (e);
}

// when closing
function onClose (e)
{
	console.log ("Connection closed", e);

}

function startingConnection()
{
	alert('Streaming ON...  display size: '+canvas2.width+' x '+canvas2.height);

	websocket.send ("STSIM");
	stop = false;

}

function closingConnection()
{
    	alert('Streaming OFF...');
	websocket.send ("END  ");
	stop = true;



	//websocket.close ();
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/




function initGL(){
    //var xml3dElement = document.getElementById("myXml3dElement");
    //var renderInterface = xml3dElement.getRenderInterface();
    //canvas2 = document.getElementById("canvas2");

    canvas3 = document.getElementById("canvas3");
		canvas3.width=512;
		canvas3.height=512;
		contextCanvas3 = canvas3.getContext("2d");
		ImageData2 = contextCanvas3.createImageData(canvas3.width,canvas3.height);
    ImageData3 = new Uint8Array(canvas3.width*canvas3.height*8 + 1);
    ImageData3[0] = 7;

		canvas4 = document.getElementById("canvas4");
		canvas4.width=512;
		canvas4.height=512;
		contextCanvas4 = canvas4.getContext("2d");


    gl = null;


    try {
	// Tratar de tomar el contexto estandar. Si falla, retornar al experimental.

	var map = L.map('map');

        var layer = Tangram.leafletLayer({
            scene: 'scene.yaml',
            attribution: '<a href="https://mapzen.com/tangram" target="_blank">Tangram</a> | &copy; OSM contributors | <a href="https://mapzen.com/" target="_blank">Mapzen</a>',
            //logLevel: 'debug',
            //preUpdate: myPreUpdateFunction,
            postUpdate: function(didRender) {
              if (didRender) {
                gl = layer.scene.gl;
								createImageFromGLObject(gl,canvas3.width,canvas3.height);
             }
          }
        });


        layer.addTo(map);

        map.setView([41.3893997, 2.1139643], 17);


    }
    catch(e) {}
}

//function createImageFromTexture(gl,texture,width,height,framebuffer){
function createImageFromGLObject(gl,width,height){
    ////read the contents of the framebuffer

    if(data.length!=width*height*4)
		data = new Uint8Array(width*height*4);

    gl.readPixels(0,0,width,height,gl.RGBA,gl.UNSIGNED_BYTE,data);

    var cnt4 = 0;
    var cnt3=width*height*4 + 1;

    for(var i=0;i<data.length;++i){
    	ImageData3[i+1]=data[i];
    	ImageData3[i+cnt3]=data[i];
    	/*
    	if(data[i]!=0&&cnt4<5){
    		console.log(data[i]+","+i);
    		++cnt4;
    		}
    		*/
    }


   	ImageData2.data.set(data);
    contextCanvas3.putImageData(ImageData2,0,0);

   console.log(ImageData3[263657]+","+ImageData3[263658]+","+ImageData3[263659]+","+ImageData3[263660]+","+ImageData3[263661]);

   send3DElement();

//    return img;

}

window.onload = initGL;
window.addEventListener("load", init, false);
