// GLSL fragment shader for rendering a slice
#version 110

uniform sampler3D vol_tex;

uniform vec3 texel_size; // voxel size in texture space
uniform vec3 voxel_size; // voxel size in world space

uniform bool enable_lighting;
uniform bool enable_fog;

varying vec4 diffuse, ambient_global, ambient; // Light components.
varying vec3 light_dir; // Light direction
varying vec3 half_vector; // Half vector between eye and light source. 
                                       // In the case of head light, it's the same as eye vector.
varying float dist; // Distance to light source.

float compute_fog_factor();

vec4 compute_lighting( vec3 normal )
{
  vec3 half_v;
  float n_dot_l, n_dot_hv;
  vec4 color = ambient_global;
  
  n_dot_l = max( dot ( normal, normalize( light_dir ) ), 0.0 );

  if ( n_dot_l > 0.0 ) 
  {
    color += ( diffuse * n_dot_l + ambient );
    
    half_v = normalize(half_vector);
    n_dot_hv = max( dot ( normal, half_v ), 0.0 );
    color += gl_FrontMaterial.specular * gl_LightSource[0].specular * 
            pow( n_dot_hv, gl_FrontMaterial.shininess );
  }

  return color;
}

void main()
{
  vec4 voxel_color( texture3D( vol_tex, gl_TexCoord[0].stp ).a );

  if ( enable_lighting )
  {
    vec3 gradient;
    gradient.x = ( texture3D( vol_tex, gl_TexCoord[0].stp + vec3( texel_size.x, 0.0, 0.0 ) ).a -
      texture3D( vol_tex, gl_TexCoord[0].stp - vec3( texel_size.x, 0.0, 0.0 ) ).a ) / ( 2.0 * voxel_size.x );
    gradient.y = ( texture3D( vol_tex, gl_TexCoord[0].stp + vec3( 0.0, texel_size.y, 0.0 ) ).a -
      texture3D( vol_tex, gl_TexCoord[0].stp - vec3( 0.0, texel_size.y, 0.0 ) ).a ) / ( 2.0 * voxel_size.y );
    gradient.z = ( texture3D( vol_tex, gl_TexCoord[0].stp + vec3( 0.0, 0.0, texel_size.z ) ).a -
      texture3D( vol_tex, gl_TexCoord[0].stp - vec3( 0.0, 0.0, texel_size.z ) ).a ) / ( 2.0 * voxel_size.z );
    gradient = normalize( gradient );
    voxel_color.rgb = ( slice_color * compute_lighting( gradient ) ).rgb;
  }

  if ( enable_fog )
  {
    voxel_color = mix( gl_Fog.color, voxel_color, compute_fog_factor() );
  }

  gl_FragColor = slice_color;
}
