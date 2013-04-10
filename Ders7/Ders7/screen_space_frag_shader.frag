#version 120

// vertex shaderindan gelen veriler
varying vec3 normal;
varying float pos;

// fragment shader main metodu
void main(void)
{
    // gl_FragColor ekrana basilacak rengi secer
    gl_FragColor = vec4(pos*0.1,pos*0.1,pos*0.1,1.0);
}
