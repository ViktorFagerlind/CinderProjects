//----------------------------------------------------------------------------------------------------------------------
uniform vec3      iResolution;     // viewport resolution (in pixels)
uniform float     iGlobalTime;     // shader playback time (in seconds)
uniform float     iChannelTime[4]; // channel playback time (in seconds)
uniform vec4      iMouse;          // mouse pixel coords. xy: current (if MLB down), zw: click
//uniform samplerXX iChannel0..3;    // input channel. XX = 2D/Cube
uniform vec4      iDate;           // (year, month, day, time in seconds)
//----------------------------------------------------------------------------------------------------------------------

#define M_PI 3.1415926535897932384626433832795

//----------------------------------------------------------------------------------------------------------------------

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

//----------------------------------------------------------------------------------------------------------------------

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

//----------------------------------------------------------------------------------------------------------------------

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

//----------------------------------------------------------------------------------------------------------------------

vec2 fade(vec2 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

//----------------------------------------------------------------------------------------------------------------------

// Classic Perlin noise
float cnoise(vec2 P)
{
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod289(Pi); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;

  vec4 i = permute(permute(ix) + iy);

  vec4 gx = fract(i * (1.0 / 41.0)) * 2.0 - 1.0 ;
  vec4 gy = abs(gx) - 0.5 ;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = taylorInvSqrt(vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11)));
  g00 *= norm.x;  
  g01 *= norm.y;  
  g10 *= norm.z;  
  g11 *= norm.w;  

  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));

  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

//----------------------------------------------------------------------------------------------------------------------

float fbm(vec2 P, int octaves, float lacunarity, float gain)
{
    float sum = 0.0;
    float amp = 1.0;
    vec2 pp = P;
     
    int i;
     
    for(i = 0; i < octaves; i+=1)
    {
        amp *= gain; 
        sum += amp * cnoise(pp);
        pp *= lacunarity;
    }

    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

float myFbm (in vec2 p)
{
  return fbm (p, 6, 2.0, 0.5) + 0.5;
}

//----------------------------------------------------------------------------------------------------------------------

float pattern_1 (in vec2 p)
{
    vec2 q = vec2 (myFbm (p + vec2(0.0,0.0)),
                   myFbm (p + vec2(5.2,1.3)));

    return myFbm (p + 4.0 * q);
}

//----------------------------------------------------------------------------------------------------------------------

float pattern_2 (in vec2 p, out vec2 q, out vec2 r)
{
  q.x = myFbm (p + vec2 (0.0, 0.2*sin (iGlobalTime/10.0)));
  q.y = myFbm (p + vec2 (15.2,1.3));

  r.x = myFbm (p + 9.54*q + vec2 (5.7 + sin (iGlobalTime/2.0),9.2));
  r.y = myFbm (p + 4.0*q + vec2 (8.83 + sin (iGlobalTime/3.0),2.8));

  return myFbm (p + 4.0*r);
}
 
//----------------------------------------------------------------------------------------------------------------------

void main()
{
  vec2 p = vec2 (gl_FragCoord.x/iResolution.x, gl_FragCoord.y/iResolution.y);

  float f;
  vec2  q, r;

  f = pattern_2 (p, q, r);

  gl_FragColor.rgb = mix (vec3 (q.y, 0, r.x), vec3 (r.x, r.y, f), f);

	gl_FragColor.a = 1.0;
}
