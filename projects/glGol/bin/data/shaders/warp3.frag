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

uniform sampler2DType tex0;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;
uniform vec2 dMouse;

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void main( void )
{
  float t = time * 0.2;
  vec2 oldPos = gl_FragCoord.xy;

  // Zoom scale
  float scale = map(mouse.x, 0.0, 1.0, 0.2, 1.0);
  vec2 distortion = vec2(0.06, 0.4);

  // Scale in/out
  vec2 newPos = oldPos;
  newPos = (newPos * scale);
  newPos = newPos + (vec2(resolution.x / 2.0, resolution.y / 2.0) * (1.0 - scale));

  // Wobble frame around a little
  vec2 temp = newPos;
  newPos.x = temp.x + 5.0*sin(2.0+t*8.0) + 0.05*cos(50.0 + t);
  newPos.y = (temp.y + 5.0*cos(5.0+t*8.0) + 0.2*sin(50.0 * t));

  // Wave distortion
  temp = newPos;
  newPos.x = temp.x + distortion.x*sin(temp.y + t * 10.0);
  newPos.y = temp.y + distortion.y*cos(temp.x + t * 10.0);

  //
  vec4 final = texture2DType(tex0, newPos);

  // Colorize
  final.r += 0.09;
  final.g += 0.04;
  final.b -= 0.4;
  gl_FragColor = vec4(final.rgb, 1.0);
}
