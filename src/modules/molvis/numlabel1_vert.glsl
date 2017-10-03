// -*-Mode: C++;-*-
//
//  Number label vertex shader for OpenGL
//

@include "lib_common.glsl"

////////////////////
// Vertex attributes

attribute vec3 a_xyz;
attribute vec2 a_wh;
attribute vec3 a_nxyz;
//attribute float a_width;
//attribute float a_addr;

////////////////////
// Uniform variables

uniform vec2 u_winsz;

uniform float u_ppa;

////////////////////
// Varying

varying vec2 v_labpos;
varying float v_ilab;

void main (void)
{
  //int ind = gl_VertexID%4;

  // Eye-coordinate position of vertex, needed in various calculations
  vec4 ecPosition = gl_ModelViewMatrix * vec4(a_xyz, 1.0);
  vec2 dxy = a_wh;

  //dxy = mat2(0, -1, 1, 0)*dxy;
  if (u_ppa>0.0f) {
    ecPosition.xy += dxy/u_ppa;
    //ecPosition.y += a_wh.y/u_ppa;
  }

  // Do fixed functionality vertex transform
  gl_Position = gl_ProjectionMatrix * ecPosition;

  vec4 ec_dir = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(a_nxyz, 1.0);
  vec2 ecdir2 = normalize(ec_dir.xy);
  mat2 m2 = mat2(ecdir2.x, ecdir2.y, -ecdir2.y, ecdir2.x);
  dxy = m2 * dxy;

  if (u_ppa<0.0f) {
    gl_Position.xy += dxy/u_winsz;
    //gl_Position.y += a_wh.y/u_winsz.y;
  }

  gl_FrontColor=gl_Color;

  gl_FogFragCoord = ffog(ecPosition.z);

  v_labpos.x = a_wh.x;
  v_labpos.y = a_wh.y;

  //v_width = (a_width);
  //v_addr = (a_addr);
  v_ilab = float(gl_VertexID/4);
}

