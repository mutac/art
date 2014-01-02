/*
Any live cell with fewer than two live neighbours dies, as if caused by under-population.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by overcrowding.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

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

uniform sampler2DType lastFrame;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;
uniform vec2 dMouse;
uniform int frame;

/*
#define tex0 iChannel0
#define resolution iResolution
#define time iGlobalTime
#define mouse iMouse
*/

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float randBetween(vec2 co, vec2 range)
{
  return mod(rand(co), (range.y - range.x + 1.0)) + range.x;
}

// Seed the scene if mouse is moved
vec4 seed(vec2 co)
{
  if (dMouse.x > 2.0)
  {
    // Mouse velocity determines the dispersion (outward from the center
    // of the scene).  Only seed the current coord if it is within the
    // dispersion zone.
    float fudge = 8.0;
    float dispersion = clamp(dMouse.x * fudge, 0, resolution.x / 2.0);
    float d = distance(resolution / 2.0, co);
    if (d <= dispersion)
    {
      // randomly disperse.  Use coordinate and time to seed RNG
      float r = randBetween(co+time, vec2(0.0, 1.0));
      if (r >= 0.0 && r <= 0.05)
      {
        return vec4(1.0, 1.0, 1.0, 0.0);
      }
    }
  }

  // Otherwise, don't seed.  Pass the normal pel through
  return texture2DType(lastFrame, co);
}

void main()
{
    // retrieve the texture coordinate
    vec2 c = texCoord();

	  vec2 pixel;
	  pixel.x = 1.0;
	  pixel.y = 1.0;

    // and the current pixel
    //vec3 current = texture2DType(lastFrame, c).rgb;
    vec3 current = seed(c).rgb;

    // Slow down...
    if (mod(frame, 40.0) == 0.0)
    {
      // count the neighboring pixels with a value greater than zero
      vec3 neighbours = vec3(0.0);
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2(-1,-1)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2(-1, 0)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2(-1, 1)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2( 0,-1)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2( 0, 1)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2( 1,-1)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2( 1, 0)).rgb, vec3(0.0)));
      neighbours += vec3(greaterThan(texture2DType(lastFrame, c + pixel*vec2( 1, 1)).rgb, vec3(0.0)));

      // check if the current pixel is alive
      vec3 live = vec3(greaterThan(current, vec3(0.0)));

      // resurrect if we are not live, and have 3 live neighbors
      current += (1.0-live) * vec3(equal(neighbours, vec3(3.0)));

      // kill if we do not have either 3 or 2 neighbors
      current *= vec3(equal(neighbours, vec3(2.0))) + vec3(equal(neighbours, vec3(3.0)));

      // fade the current pixel as it ages
      current -= vec3(greaterThan(current, vec3(0.4)))*0.05;
    }

    // write out the pixel
    gl_FragColor = vec4(current, 1.0);
}
