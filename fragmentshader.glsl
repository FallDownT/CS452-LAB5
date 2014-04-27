#version 130

in vec2 f_texcoord;

out vec4 out_color;

uniform sampler2D texture;

void main(){
  out_color = texture2D(texture, f_texcoord);
}
