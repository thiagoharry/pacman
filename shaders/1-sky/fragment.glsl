#version 100

#if GL_FRAGMENT_PRECISION_HIGH == 1
  precision highp float;
  precision highp int;
#else
  precision mediump float;
  precision mediump int;
#endif
  precision lowp sampler2D;
  precision lowp samplerCube;

uniform vec4 object_color;

uniform float time;
uniform vec2 object_size;
uniform int integer;

uniform sampler2D texture1;
varying mediump vec2 texture_coordinate;

const float cloudscale = 1.1;
const float speed = 0.03;
const float clouddark = 0.5;
const float cloudlight = 0.3;
const float cloudcover = 0.5;
const float cloudalpha = 8.0;
const float skytint = 0.5;
const vec3 skycolour1 = vec3(0.2, 0.4, 0.6);
const vec3 skycolour2 = vec3(0.4, 0.7, 1.0);

const mat2 m = mat2(1.6, 1.2, -1.2, 1.6);

vec2 hash(vec2 p){
  p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
  return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise(in vec2 p){
  const float K1 = 0.366025404; // (sqrt(3)-1)/2;
  const float K2 = 0.211324865; // (3-sqrt(3))/6;
  vec2 i = floor(p + (p.x+p.y)*K1);	
  vec2 a = p - i + (i.x+i.y)*K2;
  vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
  vec2 b = a - o + K2;
  vec2 c = a - 1.0 + 2.0*K2;
  vec3 h = max(0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
  vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
  return dot(n, vec3(70.0));	
}

float fbm(vec2 n){
  float total = 0.0, amplitude = 0.1;
  for (int i = 0; i < 7; i++) {
    total += noise(n) * amplitude;
    n = m * n;
    amplitude *= 0.4;
  }
  return total;
}

void main(){
  float time2;
  vec2 p = texture_coordinate;
  vec2 uv = p*vec2(object_size.x/object_size.y,1.0);
  float q = fbm(uv * cloudscale * 0.5);
  if(time < 150.0)
    time2 = time * speed;
  else
    time2 = (300.0 - time) * speed;
  //ridged noise shape
  float r = 0.0;
  uv *= cloudscale;
  uv -= q - time2;
  float weight = 0.8;
  for (int i=0; i<8; i++){
    r += abs(weight*noise(uv));
    uv = m*uv + time2;
    weight *= 0.7;
  }
  //noise shape
  float f = 0.0;
  uv = p*vec2(object_size.x/object_size.y,1.0);
  uv *= cloudscale;
  uv -= q - time2;
  weight = 0.7;
  for (int i=0; i<8; i++){
    f += weight*noise( uv );
    uv = m*uv + time2;
    weight *= 0.6;
  }
  f *= r + f;
  //noise colour
  float c = 0.0;
  if(time < 150.0)
    time2 = time * speed * 2.0;
  else
    time2 = (300.0 - time) * speed * 2.0;
  uv = p*vec2(object_size.x/object_size.y,1.0);
  uv *= cloudscale*2.0;
  uv -= q - time2;
  weight = 0.4;
  for (int i=0; i<7; i++){
    c += weight*noise( uv );
    uv = m*uv + time2;
    weight *= 0.6;
  }
  //noise ridge colour
  float c1 = 0.0;
  if(time < 150.0)
    time2 = time * speed * 3.0;
  else
    time2 = (300.0 - time) * speed * 3.0;
  uv = p*vec2(object_size.x/object_size.y,1.0);
  uv *= cloudscale*3.0;
  uv -= q - time2;
  weight = 0.4;
  for (int i=0; i<7; i++){
    c1 += abs(weight*noise( uv ));
    uv = m*uv + time2;
    weight *= 0.6;
  }
  c += c1;
  vec3 skycolour = mix(skycolour2, skycolour1, p.y);
  vec3 cloudcolour = vec3(1.1, 1.1, 0.9) *
    clamp((clouddark + cloudlight*c), 0.0, 1.0);
  f = cloudcover + cloudalpha*f*r;
  vec3 result = mix(skycolour, clamp(skytint * skycolour + cloudcolour,
                                     0.0, 1.0), clamp(f + c, 0.0, 1.0));
  gl_FragData[0] = vec4( result, 1.0 );
  //gl_FragData[0] = vec4(0.0, 0.0, 1.0, 1.0);
}
