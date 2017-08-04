attribute vec4 a_Position;
attribute vec2 a_TexCoord;
varying vec4 vColor;
varying  vec2 v_Texcoord;
uniform mediump mat4 uMVP;
void main() {
   gl_Position = uMVP*a_Position;
    vColor = a_Position + vec4(0.0, 0.2, 0.2, 0.2);
    v_Texcoord = a_TexCoord;
}