#version 120

varying vec2 texCoord0;
uniform sampler2D tex;
uniform vec4 color;
varying vec4 position;

float fogStart = 0.62f;
float fogEnd = 1.2f;

uniform bool includeFog;

uniform bool portal;
uniform float time;

float getFogFactor(float fogCoord)
{
	if (!includeFog)
		return 0;

	float result = (fogEnd-fogCoord)/(fogEnd-fogStart);
	result *= result;
	result = 1.0f - clamp(result, 0, 1);

	return result;
}

void main()
{
	vec4 textureColor = texture2D(tex, texCoord0.xy) * color;

	if (portal)
	{
		float f = sin(2*time);
		f *= 0.2f*f;
		f += 0.8f;

		textureColor *= f;
	}

	if (textureColor.a < 0.4)
		discard;

	float fogCoord = abs(position.z / position.w);
	vec4 fogColor = vec4(0, 0, 0, 1);

	float fogFactor = getFogFactor(fogCoord);

	gl_FragColor = mix(textureColor, fogColor, fogFactor);
}
