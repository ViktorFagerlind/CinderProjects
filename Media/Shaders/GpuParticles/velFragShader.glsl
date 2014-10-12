#extension GL_EXT_gpu_shader4: enable

uniform sampler2D positions;
uniform sampler2D velocities;
uniform sampler2D information;
uniform sampler2D oVelocities;
uniform sampler2D oPositions;
uniform sampler2D noiseTex;

uniform vec2 emissionPositions[2];

varying vec4 texCoord;

float tStep = .02;

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    
	vec3 pos = texture2D (positions, texCoord.st).rgb;

	float mass = texture2D (positions, texCoord.st).a;

	vec3 vel = texture2D (velocities, texCoord.st).rgb;
	float decay = texture2D (velocities, texCoord.st).a;

	float age = texture2D (information, texCoord.st).r;
	float maxAge = texture2D (information, texCoord.st).g;


    vec3 noise   = texture2D (noiseTex, pos.xy).rgb;
	vec3 gravity = -0.0001 * pos / clamp (pow (length(pos), 3.0), 0.001, 1.0);
	vec3 acceleration = noise + gravity;


    
	vel += acceleration;
    
    pos.x += vel.x;
    pos.y += vel.y;
	
    age += tStep;
    
	if (age >= maxAge)
    {
        vec3 origVel = texture2D(oVelocities, texCoord.st).rgb;
        vec3 origPos = texture2D(oPositions, texCoord.st).rgb;

        age = 0.0;

		if (fract (pos.x * 100) < 0.5) // 50% 
	        pos = vec3(emissionPositions[0],0);
		else
	        pos = vec3(emissionPositions[1],0);
		
		pos += 0.1 * (origPos - vec3(rand (vec2 (3,6)), 
									  rand (vec2 (3,6)), 
								      0));

        vel = origVel;
    }
	
    //position + mass
	gl_FragData[0] = vec4(pos, mass);
    //velocity + decay
	gl_FragData[1] = vec4(vel, decay);
    //age information
	gl_FragData[2] = vec4(age, maxAge, 0.0, 1.0);
}
