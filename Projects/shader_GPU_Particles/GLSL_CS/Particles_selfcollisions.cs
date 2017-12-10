
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// file: Particles_selfcollisions.cs
//
//	Author Sergey Solokhin (Neill3d)
//
// GPU Particles self collision
//
//	GitHub page - https://github.com/Neill3d/MoPlugs
//	Licensed under BSD 3-Clause - https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#version 430

layout (local_size_x = 32, local_size_y = 1) in;

uniform int		gNumParticles;
uniform float	DeltaTimeSecs;

#define		ACCELERATION_LIMIT		5.0

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TYPES AND DATA BUFFERS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TParticle
{
	//vec4				OldPos;				// in w store Particle Type  
	vec4				Pos;				// in w hold lifetime from 0.0 to 1.0 (normalized)
	vec4				Vel;				// in w hold total lifetime
	vec4				Color;				// inherit color from the emitter surface, custom color simulation
	vec4				Rot;				// in w - float				AgeMillis;			// current Age
	vec4 				RotVel;				// in w - float				Index;				// individual assigned index from 0.0 to 1.0 (normalized)
};

layout (std430, binding = 0) buffer ParticleBuffer
{
	TParticle particles[];
} particleBuffer;

uint get_invocation()
{
   //uint work_group = gl_WorkGroupID.x * gl_NumWorkGroups.y * gl_NumWorkGroups.z + gl_WorkGroupID.y * gl_NumWorkGroups.z + gl_WorkGroupID.z;
   //return work_group * gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z + gl_LocalInvocationIndex;

   // uint work_group = gl_WorkGroupID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x + gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
   uint work_group = gl_GlobalInvocationID.y * (gl_NumWorkGroups.x * gl_WorkGroupSize.x) + gl_GlobalInvocationID.x;
   return work_group;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//shared vec4 tmp[gl_WorkGroupSize.x];
//shared vec4 tmpVel[gl_WorkGroupSize.x];
//shared float tmpIndex[gl_WorkGroupSize.x];

void main()
{
	// particle id in the array
	uint flattened_id = get_invocation();
	
	// ?! skip unused part of the array
	if (flattened_id >= gNumParticles)
		return;
	
	vec3 acceleration = vec3(0.0);

	// Read position and velocity
	vec4 pos = particleBuffer.particles[flattened_id].Pos;
	vec4 vel = particleBuffer.particles[flattened_id].Vel;
	
	float mass = 1.0;
	float radius1 = pos.w;

	if (vel.w > 0.0)
	{
		//int N = int(gl_NumWorkGroups.x*gl_WorkGroupSize.x);
	
		//
		// linear calculation
		
		for (int i=0; i<gNumParticles; ++i)
		{
			vec4 other = particleBuffer.particles[i].Pos;
			vec4 othervel = particleBuffer.particles[i].Vel;

			vec3 n = pos.xyz - other.xyz;
			float udiff = length(n);
			float radsum = radius1 + other.w;

			if ( othervel.w <= 0.0 && udiff >= radsum )
			{
				n = normalize(n);

				float a1 = dot(vel.xyz, n);
				float a2 = dot(othervel.xyz, n);

				float optimizedP = (2.0 * (a1 - a2)) / (mass + mass); 

				// calculate v1', the new movement vector of circle1
				//vel.xyz = vel.xyz - optimizedP * mass * n;
				acceleration = acceleration - optimizedP * mass * n;
			}
		}
	}

	/*
	int groupSize = int(gl_WorkGroupSize.x);
	
	for(int tile = 0; tile<N; tile+=groupSize) 
	{
		tmpIndex[gl_LocalInvocationIndex] = tile + int(gl_LocalInvocationIndex);
		if (tmpIndex[gl_LocalInvocationIndex] < gNumParticles)
		{
			tmp[gl_LocalInvocationIndex] = particleBuffer.particles[tile + int(gl_LocalInvocationIndex)].Pos;
			tmpVel[gl_LocalInvocationIndex] = particleBuffer.particles[tile + int(gl_LocalInvocationIndex)].Vel;
		}
		groupMemoryBarrier();
		barrier();
		for(int i=0; i<gl_WorkGroupSize.x; ++i) 
		{
			if (tmpIndex[i] >= gNumParticles)
				continue;
			
			vec4 othervel = tmpVel[i];
			
			if (othervel.w <= 0.0)
				continue;
			
			float radius2 = tmp[i].w;
			
			vec3 n = pos.xyz - tmp[i].xyz;
			float udiff = length(n);
			
			if (udiff >= (radius1 + radius2))
			{
				continue;
			}
			n = normalize(n);

			float a1 = dot(vel.xyz, n);
			float a2 = dot(othervel.xyz, n);

			float optimizedP = (2.0 * (a1 - a2)) / (mass + mass); 

			// calculate v1', the new movement vector of circle1
			//vel.xyz = vel.xyz - optimizedP * mass * n;
			acceleration -= optimizedP * mass * n;
		}
		groupMemoryBarrier();
        barrier();
	}
	*/
	/*
	for(int tile = 0;tile<N;tile+=int(gl_WorkGroupSize.x)) 
	{
		tmp[gl_LocalInvocationIndex] = particleBuffer.particles[tile + int(gl_LocalInvocationIndex)].Pos;
		groupMemoryBarrier();
		barrier();
		for(int i=0; i<gl_WorkGroupSize.x; ++i) 
		{
			if ()
			vec3 other = tmp[i].xyz;
			vec3 diff = pos.xyz - other;
			float invdist = 1.0/(length(diff)+0.001);
			acceleration -= diff*0.1*invdist*invdist*invdist;
		}
		groupMemoryBarrier();
        barrier();
	}
	*/
	/*
	for(int tile = 0;tile<N;tile+=int(gl_WorkGroupSize.x)) {\n"
        "       tmp[gl_LocalInvocationIndex] = positions[tile + int(gl_LocalInvocationIndex)];\n"
        "       groupMemoryBarrier();\n"
        "       barrier();\n"
        "       for(int i = 0;i<gl_WorkGroupSize.x;++i) {\n"
        "           vec3 other = tmp[i].xyz;\n"
        "           vec3 diff = position - other;\n"
        "           float invdist = 1.0/(length(diff)+0.001);\n"
        "           acceleration -= diff*0.1*invdist*invdist*invdist;\n"
        "       }\n"
        "       groupMemoryBarrier();\n"
        "       barrier();\n"
        "   }\n"
	*/
	
	float accLen = length(acceleration);
	if (accLen > ACCELERATION_LIMIT)
		acceleration = ACCELERATION_LIMIT * normalize(acceleration);
	

	particleBuffer.particles[flattened_id].Vel = vec4(vel.xyz + acceleration * DeltaTimeSecs, vel.w);	// in w we store lifetime
	//particleBuffer.particles[flattened_id].RotVel = vec4(acceleration.xyz, rotVel.w);
}