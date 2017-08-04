 precision mediump float;
 varying vec4 vColor;
 varying  vec2 v_Texcoord;
 uniform sampler2D u_tTex;


 void main() {
        gl_FragColor = texture2D(u_tTex, v_Texcoord);//vec4(0.0, 1.0, 0.0, 1.0);

    }