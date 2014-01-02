

#ifndef GL_ES
#define sampler2DType sampler2DRect
#define texture2DType texture2DRect
vec2 texCoord()
{
  return gl_FragCoord.xy;
}
#else
#define sampler2DType sampler2D
#define texture2DType texture2D
vec2 pos()
{
  return gl_FragCoord.xy;
}
#endif

uniform sampler2DType   tex0;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

float dist(vec2 p1, vec2 p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	return sqrt(dx * dx + dy * dy);
}

float dotp(vec2 p1, vec2 p2)
{
	return p1.x * p2.x + p1.y * p2.y;
}

void main(void)
{
  vec2 uv = gl_FragCoord.xy;
	float t = time;
	vec2 a = vec2(0.5,0.5);
	vec2 b = vec2(0.25,0.625);
	vec2 c = vec2(0.75,0.625);

	float f = mouse.x * 1.0;

	float d = cos(dist(uv,a)*f)+ cos(dist(uv,b)*f+t*0.2) + cos(dist(uv,c)*f+t*0.3);
	vec4 col = texture2DType(tex0, uv+d);
	vec4 origCol = texture2DType(tex0, uv);

	gl_FragColor = vec4(col.r, origCol.g, col.b, 1.0);
}

