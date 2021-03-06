#version 330

//---global uniforms
layout(std140) uniform GlobalMatrices
{
    mat4 cameraToClipMatrix;
    mat4 worldToCameraMatrix;
};

//---shader spec
layout(location=0) in vec4 TexCoord;
layout(location=1) in vec2 SpriteCenter;
layout(location=2) in float Heading;
layout(location=3) in float Alpha;
layout(location=4) in float Radius;
smooth out vec2 inTexCoord;
smooth out vec4 inColor;

vec2[4] corners=vec2[4](vec2(-1,-1),
        vec2(1,-1),
        vec2(-1,1),
        vec2(1,1));

void main()
{
    inColor=vec4( 1.0, 1.0, 1.0, Alpha );
    inTexCoord=vec2(TexCoord[int(floor(mod(gl_VertexID,2.0)))],TexCoord[2+int(floor(gl_VertexID/2.0))]);
    vec2 position=corners[gl_VertexID];
    mat2 ScaleMatrix=Radius*mat2(cos(Heading),sin(Heading),-sin(Heading),cos(Heading));
    gl_Position=cameraToClipMatrix*worldToCameraMatrix*vec4(ScaleMatrix*position+SpriteCenter,0,1);
}

