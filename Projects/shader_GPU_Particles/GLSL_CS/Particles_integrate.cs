
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// file: Particles_integrate.cs
//
//	Author Sergey Solokhin (Neill3d)
//
// GPU Particles n-bodies
//
//	GitHub page - https://github.com/Neill3d/MoPlugs
//	Licensed under BSD 3-Clause - https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#version 430

layout (local_size_x = 64, local_size_y = 1) in;

uniform float	DeltaTimeSecs;
uniform	int		gNumCollisions;

#define		USE_COLLISIONS		gNumCollisions > 0

#define		COLLISION_SPHERE		1.0
#define		COLLISION_TERRIAN		4.0

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

struct TCollision
{
	vec4			position;			// use w as collision type
	vec4			velocity;
	
	//vec4			terrainOffset;	// terrain offset stores in position
	vec4			terrainScale;
	vec4			terrainSize;		// texture dimentions
	
	float 			radius;
	float			friction;
	
	uvec2			terrainAddress;
};

layout (std430, binding = 0) buffer ParticleBuffer
{
	TParticle particles[];
} particleBuffer;

layout (std430, binding = 2) readonly buffer CollisionBuffer
{
	TCollision collisions[];
} collisionBuffer;

// terrain depth
layout(binding=0) uniform sampler2D 	TerrainSampler;

///////////////////////////////////////////

uint get_invocation()
{
   //uint work_group = gl_WorkGroupID.x * gl_NumWorkGroups.y * gl_NumWorkGroups.z + gl_WorkGroupID.y * gl_NumWorkGroups.z + gl_WorkGroupID.z;
   //return work_group * gl_WorkGroupSize.x * gl_WorkGroupSize.y * gl_WorkGroupSize.z + gl_LocalInvocationIndex;

   // uint work_group = gl_WorkGroupID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x + gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationIndex;
   uint work_group = gl_GlobalInvocationID.y * (gl_NumWorkGroups.x * gl_WorkGroupSize.x) + gl_GlobalInvocationID.x;
   return work_group;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTRAINT AND COLLIDE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// constrain particle to be outside volume of a sphere
void SphereConstraint(TCollision data, inout vec3 x)
{
	vec3 delta = x - data.position.xyz;
	float dist = length(delta);
	if (dist < data.radius) {
		x = data.position.xyz + delta*(data.radius / dist);
	}
}

// constrain particle to heightfield stored in texture
void TerrainConstraint(TCollision data, inout vec3 pos)
{
	vec3 scale = data.terrainScale.xyz;
	vec3 offset = data.position.xyz;
	
	
	if (pos.x < offset.x || pos.x > offset.x+scale.x || pos.z < offset.z || pos.z > offset.z+scale.z
		|| pos.y < offset.y || pos.y > data.terrainSize.z )
	{
		return;
	}
	
	vec2 uv = (pos.xz - offset.xz) / scale.xz;

	float h = texture(TerrainSampler, uv).x;
	
	if (pos.y < h) {
		pos.y = h;
	}
}

// constrain particle to be above floor
void FloorConstraint(inout vec3 x, float level)
{
	if (x.y < level) {
		x.y = level;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main()
{
	// particle id in the array
	uint flattened_id = get_invocation();
	
	// ?! skip unused part of the array
	//if (flattened_id >= gNumParticles)
	//	return;
	
	// Read position and velocity
	vec4 pos = particleBuffer.particles[flattened_id].Pos;
	vec4 vel = particleBuffer.particles[flattened_id].Vel;
	
	float lifetime = vel.w;
	
	if (lifetime <= 0.0)
		return;
	
	pos.xyz = pos.xyz + vel.xyz * DeltaTimeSecs;
	
	if (USE_COLLISIONS)
	{
		for(int i=0; i<gNumCollisions; ++i)
		{
			float type = collisionBuffer.collisions[i].position.w;
			
			if (type == COLLISION_SPHERE) 
			{
				SphereConstraint(collisionBuffer.collisions[i], pos.xyz);
			}
			else if (type == COLLISION_TERRIAN)
			{
				TerrainConstraint(collisionBuffer.collisions[i], pos.xyz);
			}
		}
	}
	/*
	if (USE_FLOOR > 0.0) 
		FloorConstraint(pos.xyz, FLOOR_LEVEL);
	*/
	particleBuffer.particles[flattened_id].Pos = pos;	// in w we store lifetime
}