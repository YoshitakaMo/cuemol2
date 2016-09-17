// -*-Mode: C++;-*-
//
//  Tube2Renderer vertex shader for OpenGL
//

//#version 120
#version 140
#extension GL_ARB_compatibility : enable

#extension GL_EXT_gpu_shader4 : enable 


//precision mediump float;

////////////////////
// Uniform variables
//uniform sampler1D coordTex;
//uniform sampler2D coordTex;
uniform samplerBuffer coefTex;

uniform samplerBuffer binormTex;

uniform int u_npoints;

////////////////////
// Vertex attributes

// spline rho param
attribute vec2 a_rho;

// color
attribute vec4 a_color;

////////////////////

float ffog(in float ecDistance)
{
  return(abs(ecDistance));
}

void getCoefs(in samplerBuffer tex, in int ind, out vec3 vc0, out vec3 vc1, out vec3 vc2, out vec3 vc3)
{
/*
  ivec2 iv;
  iv.x = ind%1024;
  //iv.y = ind;
  iv.y = ind/1024;
  //iv.x = 1;
  return ( texelFetch2D(coordTex, iv, 0).xyz );
*/
  
  vc0.x = texelFetch(tex, ind*12+0).r;
  vc0.y = texelFetch(tex, ind*12+1).r;
  vc0.z = texelFetch(tex, ind*12+2).r;

  vc1.x = texelFetch(tex, ind*12+3).r;
  vc1.y = texelFetch(tex, ind*12+4).r;
  vc1.z = texelFetch(tex, ind*12+5).r;

  vc2.x = texelFetch(tex, ind*12+6).r;
  vc2.y = texelFetch(tex, ind*12+7).r;
  vc2.z = texelFetch(tex, ind*12+8).r;

  vc3.x = texelFetch(tex, ind*12+9).r;
  vc3.y = texelFetch(tex, ind*12+10).r;
  vc3.z = texelFetch(tex, ind*12+11).r;

  // return texelFetch1D(coordTex, ind, 0).xyz;
}

vec3 interpolate(in samplerBuffer tex, in float rho)
{
  vec3 coef0, coef1, coef2, coef3;

  int ncoeff = int(floor(rho));
  ncoeff = clamp(ncoeff, 0, u_npoints-2);

  getCoefs(tex, ncoeff, coef0, coef1, coef2, coef3);

  float f = rho - float(ncoeff);

  vec3 rval;
  rval = coef3*f + coef2;
  rval = rval*f + coef1;
  rval = rval*f + coef0;

  return rval;
}

void interpolate2(in samplerBuffer tex, in float rho,
                  out vec3 rval, out vec3 drval)
{
  vec3 coef0, coef1, coef2, coef3;

  int ncoeff = int(floor(rho));
  ncoeff = clamp(ncoeff, 0, u_npoints-2);

  getCoefs(tex, ncoeff, coef0, coef1, coef2, coef3);

  float f = rho - float(ncoeff);

  rval = coef3*f + coef2;
  rval = rval*f + coef1;
  rval = rval*f + coef0;

  drval = coef3*(3.0*f) + coef2*2.0;
  drval = drval*f + coef1;
}

vec4 getSectTab(in int ind)
{
  return vec4( texelFetch(sectTex, ind*4+0).r,
               texelFetch(sectTex, ind*4+1).r,
               texelFetch(sectTex, ind*4+2).r,
               texelFetch(sectTex, ind*4+3).r );
}

// local variables for lighting calc
vec4 Ambient;
vec4 Diffuse;
vec4 Specular;

void DirectionalLight(in int i, in vec3 normal)
{
  float nDotVP;         // normal . light direction
  float nDotHV;         // normal . light half vector
  float pf;             // power factor
  
  nDotVP = max(0.0, dot(normal,
                        normalize(vec3(gl_LightSource[i].position))));
  nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));
  
  if (nDotVP == 0.0)
    pf = 0.0;
  else
    pf = pow(nDotHV, gl_FrontMaterial.shininess);
  
  Ambient  += gl_LightSource[i].ambient;
  Diffuse  += gl_LightSource[i].diffuse * nDotVP;
  Specular += gl_LightSource[i].specular * pf;
}

vec4 flight(in vec4 acolor, in vec3 normal, in vec4 ecPosition)
{
  vec4 color;
  vec3 ecPosition3;
  vec3 eye;

  ecPosition3 = (vec3 (ecPosition)) / ecPosition.w;
  eye = vec3 (0.0, 0.0, 1.0);

  // Clear the light intensity accumulators
  Ambient  = vec4 (0.0);
  Diffuse  = vec4 (0.0);
  Specular = vec4 (0.0);

  DirectionalLight(0, normal);

  color = gl_LightModel.ambient * acolor;
  color += Ambient  * acolor;
  color += Diffuse  * acolor;
  color += Specular * gl_FrontMaterial.specular;
  color = clamp( color, 0.0, 1.0 );
  return color;
}

void main (void)
{
  //float xx = float(a_ind12.x)/2.0;

  float par = a_rho.x;

  vec3 cpos, bpos, v0;
  interpolate2(coefTex, par, cpos, v0);
  bpos = interpolate(binormTex, par);
  
  vec3 e0 = normalize(v0);

  vec3 binorm = bpos - cpos;
  vec v2 = binorm - e0*(dot(e0,binorm));
  vec e2 = normalize(v2);
  vec e1 = cross(e2,e0);

  int j = int(a_rho.y);

  vec4 stab = getSectTab(j);

  vec3 pos = e1*stab.x + e2*stab.y;
  vec3 norm = e1*stab.z + e2*stab.w;

  // Eye-coordinate position of vertex, needed in various calculations
  vec4 ecPosition = gl_ModelViewMatrix * vec4(pos1, 1.0);
  //gEcPosition = ecPosition;

  // Do fixed functionality vertex transform
  gl_Position = gl_ProjectionMatrix * ecPosition;

  //gl_FrontColor=vec4(xx, xx, xx, 1.0);
  //gl_FrontColor=a_color;

  gl_FrontColor = flight(a_color, norm, ecPosition);


  gl_FogFragCoord = ffog(ecPosition.z);
}

