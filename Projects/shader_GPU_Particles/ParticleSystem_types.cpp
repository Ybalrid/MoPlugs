
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// file: ParticleSystem_types.cpp
//
//	Author Sergey Solokhin (Neill3d)
//
//	GitHub page - https://github.com/Neill3d/MoPlugs
//	Licensed under BSD 3-Clause - https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "ParticleSystem_types.h"

using namespace ParticlesSystem;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void EvaluationExchange::SetDirection(evaluateBlock &data, const vec3 &dir, const vec3 &random, bool useNormals)
{
	data.gDirection[0] = dir[0];
	data.gDirection[1] = dir[1];
	data.gDirection[2] = dir[2];
	data.gDirection[3] = (useNormals) ? 1.0 : 0.0;
	data.gDirRandom = random;
}
void EvaluationExchange::SetVelocity(evaluateBlock &data, const vec3 &vel, const vec3 &random, const vec4 &emittervel)
{
	data.gVelocity = vel;
	data.gVelRandom = random;
	data.gEmitterVelocity = emittervel;
}
void EvaluationExchange::SetDynamicParameters(evaluateBlock &data, float mass, float damping)
{
	data.gDynamic[0] = mass;
	data.gDynamic[1] = damping;
}
void EvaluationExchange::SetGravity(evaluateBlock &data, const vec3 &gravityDir, bool useGravity)
{
	data.gGravity[0] = gravityDir[0];
	data.gGravity[1] = gravityDir[1];
	data.gGravity[2] = gravityDir[2];
	data.gGravity[3] = (useGravity) ? 1.0f : 0.0f;
}
void EvaluationExchange::SetFlags(evaluateBlock &data, bool useForces, bool useCollisions, int emitterType)
{
	data.gFlags[0] = (useForces) ? 1.0f : 0.0f;
	data.gFlags[1] = (useCollisions) ? 1.0f : 0.0f;
	data.gFlags[2] = (float) emitterType;
}
void EvaluationExchange::SetTurbulence(evaluateBlock &data, const bool useTurbulence, const float freq, const float speed, const float amp)
{
	data.gTurbulence[3] = (useTurbulence) ? 1.0f : 0.0f;
	data.gTurbulence[0] = freq;
	data.gTurbulence[1] = speed;
	data.gTurbulence[2] = amp;
}
void EvaluationExchange::SetFloorParamaters(evaluateBlock &data, bool useFloor, float friction, float level)
{
	data.gFloor[3] = (useFloor) ? 1.0f : 0.0f;
	data.gFloor[1] = friction;
	data.gFloor[2] = level;
}
void EvaluationExchange::SetBoundingBox(evaluateBlock &data, const vec3 &bmin, const vec3 &bmax)
{
	data.gMin = bmin;
	data.gMax = bmax;
}


////////////////////////////////////////////////////////////////////////////
//

void CollisionExchange::SetPosition(TCollision &data, const int type, const vec3 &pos)
{
	data.position[0]=pos[0];
	data.position[1]=pos[1];
	data.position[2]=pos[2];
	data.position[3]= (double) type;
}

void CollisionExchange::SetVelocity(TCollision &data, const vec4 &value)
{
	data.velocity = value;
}

void CollisionExchange::SetRadius(TCollision &data, const double value)
{
	data.radius = (float) value;
}

void CollisionExchange::SetFriction(TCollision &data, const double value)
{
	data.friction = (float) value;
}

void CollisionExchange::SetInvMatrix(TCollision &data, const double *value)
{
	for (int i=0; i<16; ++i)
		data.tm.mat_array[i] = (float) value[i];
}

void CollisionExchange::SetTerrainScale(TCollision &data, const vec4 &value)
{
	data.terrainScale = value;
}
void CollisionExchange::SetTerrainSize(TCollision &data, const vec4 &value)
{
	data.terrainSize = value;
}
void CollisionExchange::SetTerrainTextureAddress(TCollision &data, const GLuint64 address)
{
	data.terrainAddress = address;
}

////////////////////////////////////////////////////////////////////////////
//

void ForceExchange::SetPosition(TForce &data, int type, const vec3 &pos)
{
	data.position[0]=pos[0];
	data.position[1]=pos[1];
	data.position[2]=pos[2];
	data.position[3]= (float) type;
}

void ForceExchange::SetDirection(TForce &data, const vec3 &dir, const double w)
{
	data.direction[0]=dir[0];
	data.direction[1]=dir[1];
	data.direction[2]=dir[2];
	data.direction[3]= w;
}

void ForceExchange::SetMagnitude(TForce &data, const float value)
{
	data.magnitude = value;
}

void ForceExchange::SetRadius(TForce &data, const double value)
{
	data.radius = (float) value;
}

void ForceExchange::SetTurbulence(TForce &data, const bool usage, const double amplitude, const double frequency)
{
	data.turbulence[0] = amplitude;
	data.turbulence[1] = frequency;
	data.turbulence[2] = 0.0f;
	data.turbulence[3] = (usage) ? 1.0f : 0.0f;
}
void ForceExchange::SetTurbulence(TForce &data,  const vec3 &value, const double w)
{
	data.turbulence[0]=value[0];
	data.turbulence[1]=value[1];
	data.turbulence[2]=value[2];
	data.turbulence[3]= w;
}
void ForceExchange::SetWind1(TForce &data, const vec3 &value, const double w)
{
	data.wind1[0]=value[0];
	data.wind1[1]=value[1];
	data.wind1[2]=value[2];
	data.wind1[3]= w;
}
void ForceExchange::SetWind2(TForce &data, const vec3 &value, const double w)
{
	data.wind2[0]=value[0];
	data.wind2[1]=value[1];
	data.wind2[2]=value[2];
	data.wind2[3]= w;
}
void ForceExchange::SetNoise(TForce &data, const double _useNoise, const double _noiseFreq, const double _noiseSpeed)
{
	data.wind2[3]=_useNoise;
	data.noiseFreq = _noiseFreq;
	data.noiseSpeed = _noiseSpeed;
}