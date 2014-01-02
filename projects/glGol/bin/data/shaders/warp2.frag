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

//our texture reference
uniform sampler2DType   tex0;

//width and height that we are working with
uniform vec2 resolution;

//a changing value to work with
uniform float time;
uniform vec2 mouse;

const int numPoints = 2;
const float rotations = 3.0;

float rand( vec2 n ) {	return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453); }

struct Ray
{
	vec2 point;
	vec2 direction;
};

float noise(vec2 n) {
	const vec2 d = vec2(0.0, 1.0);
	vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
	return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

vec2 noise2(vec2 n){ return vec2(noise(vec2(n.x+0.2, n.y-0.6)), noise(vec2(n.y+3., n.x-4.)));}

Ray GetRay(float i)
{
	vec2 position = noise2(vec2(i*0.12+time*0.1, i*0.43+time*0.1));
	return Ray(
		position,
		normalize(noise2(vec2(i*7.+mouse.x*0.5*1.5*sin(time), i*6.))*2.0-1.0));
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy;
	vec2 kaleidoscopePoint = clamp(sin(time) * resolution, 0.25 * resolution, 0.75 * resolution);
	vec2 curPos = uv-kaleidoscopePoint;

	float angle=atan(curPos.y, curPos.x);
	float dist =length(curPos);

	angle = angle*rotations;
	curPos.x=cos(angle)*dist;
	curPos.y=sin(angle)*dist;
	curPos += kaleidoscopePoint;

	for(int i=0;i<numPoints;i++)
	{
		Ray ray=GetRay(float(i+1)*3.);
		float offset=dot(curPos-ray.point, ray.direction);
		if(offset<0.)
		{
			curPos -= ray.direction*offset*2.0;
		}
	}

	gl_FragColor.rgb = texture2DType( tex0, vec2(curPos.x,curPos.y) ).xyz;
	gl_FragColor.a = 1.0;
}
