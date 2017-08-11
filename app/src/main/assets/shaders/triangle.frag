 precision mediump float;
 varying vec4 vColor;
 varying  vec2 v_Texcoord;
 uniform sampler2D u_tTex;
 uniform vec4 u_color;


 void main() {
        gl_FragColor = u_color;//vec4(0.0, 1.0, 0.0, 1.0);//texture2D(u_tTex, v_Texcoord);//

    }