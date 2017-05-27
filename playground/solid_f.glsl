#version 330 core

uniform sampler2D matcap;
//varying highp vec4 qt_TexCoord0;

in vec3 Position_worldspace;
in vec4 screen_normals;

void main(void)
{
    vec4 n_normals = normalize(screen_normals);
    gl_FragColor = texture2D(matcap,vec2(n_normals.x*.5+.5,
                             n_normals.y*.5+.5)).bgga;
}
