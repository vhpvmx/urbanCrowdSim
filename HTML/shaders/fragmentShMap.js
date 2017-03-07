//fragment shaders don't have a deffault precission so we need
//to pick one. mediump is a good default, it means 'medium precision'.

var fragShSource =
    "precision highp float; "+
    "varying vec2 vTextureCoord; "+
    "uniform sampler2D uSampler; "+
    
    "void main(void){ "+
    "float grayColor = texture2D(uSampler,vec2(vTextureCoord.s, vTextureCoord.t)).x; "+
    //"gl_FragColor = texture2D(uSampler,vec2(vTextureCoord.s, vTextureCoord.t)); "+
    "gl_FragColor = vec4(1.0,0.0,1.0,1.0); "+
    //"gl_FragColor = vec4(grayColor,grayColor,grayColor,1.0); "+
    "} ";
