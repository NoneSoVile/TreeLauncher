  "attribute vec4 vPosition;\n"
    "varying vec4 vColor;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "  vColor = vPosition + vec4(0.0, 0.2, 0.2, 0.2);\n"

    "}\n