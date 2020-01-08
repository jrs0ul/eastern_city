#ifdef GL_ES
//
precision mediump float;

#endif


uniform sampler2D uDarkness;
uniform sampler2D uLight;

varying vec2 vUvs;
varying vec4 vColor;

void main(void)
{
   vec4 texelsDark = texture2D(uDarkness, vUvs);
   vec4 texelsLight = texture2D(uLight, vUvs);
   gl_FragColor = vec4(0, 0,0, (texelsDark.a * vColor.a) - (texelsLight.a * vColor.a));
}
