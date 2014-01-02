// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

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

/*
#define tex0 iChannel0
#define resolution iResolution
#define time iGlobalTime
#define mouse iMouse
*/

float projectV(float v, float mx, float b)
{
  return mx * v + b;
}

void main(void)
{
  float newTime = time * 2.0;

  //float squish = mouse.x / resolution.x;
  float squish = mouse.x;
  //squish = projectV(squish, 1.0, -0.5);

  vec2 q = gl_FragCoord.xy;
  //vec2 uv = 0.5 + (q-0.5)*(squish);
  //vec2 uv = 0.5 + (q-0.5)*(0.9 + 0.1*sin(0.2*time));

  vec2 uv = 0.5 + (q-0.5)*(1.0 + 0.5*sin(0.1*time));
  //uv += vec2(sin(squish), cos(squish));

  uv.s = q.s + (cos(newTime + (q.s*0.41*time)));
  uv.t = q.t + (sin(newTime + (q.t*0.2*time)));

  vec3 oricol = texture2DType(tex0, vec2(q) ).xyz;
  vec3 col;

  col.r = texture2DType(tex0, uv).r + 0.5;
  col.g = texture2DType(tex0, uv).g + 1.0;
  col.b = texture2DType(tex0, uv).b + squish;

  col = clamp(col*0.5+0.5*col*col*1.2,0.0,1.0);
  col *= 0.5 + 0.5*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y);
  col *= vec3(0.95,1.05,0.95);
  col *= 0.9+0.1*sin(10.0*time+uv.y*1000.0);
  col *= 0.99+0.01*sin(110.0*time);

  float comp = smoothstep( 0.2, 0.7, sin(time) );
  //col = mix( col, oricol, clamp(-2.0+2.0*q.x+3.0*comp,0.0,1.0) );
  col = mix(col, oricol, 1.0);

  gl_FragColor = vec4(col, 1.0);
}
