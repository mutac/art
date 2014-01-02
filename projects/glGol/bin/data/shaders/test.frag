
#ifndef GL_ES
#define sampler2DType sampler2DRect
#define texture2DType texture2DRect
vec2 texCoord()
{
  return gl_TexCoord[0].st;
}
#else
#define sampler2DType sampler2D
#define texture2DType texture2D
vec2 pos()
{
  return gl_FragCoord.xy / resolution.xy;
}
#endif

uniform sampler2DType lastFrame;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

void main()
{
  vec2 pos = gl_FragCoord.xy;
	vec4 last = texture2DType(lastFrame, pos);
	//last.r += 0.52;
	if (last.g >= 1.0)
  {
    last.g = 0.0;
  }
  else
  {
   last.g += 0.002;
  }

	last.a = 1.0;
	//last.r += mouse.x;
	//last.g += mouse.y;
	//last.a = 1.0;

	gl_FragColor = last;
}
