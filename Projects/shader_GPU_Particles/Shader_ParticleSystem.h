
#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// file: Shader_ParticleSystem.h
//
//	Author Sergey Solokhin (Neill3d)
//
//	GitHub page - https://github.com/Neill3d/MoPlugs
//	Licensed under BSD 3-Clause - https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>

//-------------- Effect system
#include "FxParser.h"

#include "algorithm\nv_math.h"
#include "Delegate.h"

#include "ParticleSystem_types.h"

namespace ParticlesSystem
{

	enum ETechEmitType
	{
		eTechEmitVolume,
		eTechEmitSurface,
		eTechEmitVertices,
		eTechEmitPreGenerated,
		eTechEmitCount
	};

/*
const GLchar* Varyings[5];    
    Varyings[0] = "OldPosition1";
    Varyings[1] = "Position1";
    Varyings[2] = "Velocity1";    
    Varyings[3] = "Age1";
	Varyings[4] = "Index1";
    
	bind ();
	glTransformFeedbackVaryings(getProgram(), 5, Varyings, GL_INTERLEAVED_ATTRIBS);
	bool linking = relink ();
	unbind ();

    if (false == linking)
	{
		return false;
	}
	/*
	char Name[64];
	memset(Name, 0, 64);
	GLsizei Length(0);
	GLsizei Size(0);
	GLenum Type(0);

	glGetTransformFeedbackVarying(
		getProgram(),
		0,
		64,
		&Length,
		&Size,
		&Type,
		Name);
		*/
	

// forward declaration
class Shader;

////////////////////////////////////////////////////////
// Global functions to get a shader call
Shader	*QueryShader();
void	FreeShader();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global GLSLFX Shader class - all techniques to populate, evaluate, sort and render particle system

class Shader
{
public:

	//! a constructor
	Shader();
	//! a destructor
	~Shader();

	bool IsInitialized();

	// effectLocation - full path to the effect
	// return false if failed to load effect from file
	static bool SetShaderEffectLocation(const char *effectLocation, const int effectStrLen);
	static bool SetComputeShaderLocation(const char *shaderLocation, const int shaderStrLen);
	static bool SetComputeSelfCollisionsShaderLocation(const char *shaderLocation, const int shaderStrLen);
	static bool SetComputeIntegrateLocation(const char *shaderLocation, const int shaderStrLen);
	bool Initialize();

	// very important function to recreate resources on context change
	void ChangeContext();

	Gallant::Delegate1<const char*>		OnMessageEvent;

	void	BindEmit(const ETechEmitType	type);
	void	UnBindEmit();

	void	BindSimulation(const bool updatePosition);
	void	DispatchSimulation(const float dt, const float gTime, const int size, const int group_x, const int group_y, const int group_z);
	void	UnBindSimulation();

	void	BindSelfCollisions();
	void	DispatchSelfCollisions(const float dt, const int size, const int group_x, const int group_y, const int group_z);
	void	UnBindSelfCollisions();

	void	BindIntegrate();
	void	DispatchIntegrate(const float dt, const int size, const int group_x, const int group_y, const int group_z);
	void	UnBindIntegrate();

	void	BindRenderPoints();
	void	UnBindRenderPoints();

	void	BindTerrainDepth();
	void	UnBindTerrainDepth();
	void	BindTerrainPreview();
	void	UnBindTerrainPreview();

	void	UploadEvaluateDataBlock(const evaluateBlock &_evaluateBlock);
	void	UploadRenderDataBlock(const renderBlock	&_renderBlock);
	void	UploadTerrainDataBlock(const terrainBlock &blockData);

	void	SetTime(const float value);
	void	SetDeltaTime(const float value);

	void	UpdateTerrainModelTM(const mat4 &tm);
	void	SetUniformTerrainTextureAddress(const GLuint64 address);
	
	const GLuint	GetEmitGeometryProgramId(const ETechEmitType	type) const
	{
		return emitGeometryProgram[type];
	}

	const GLuint	GetEmitMeshLocation(const ETechEmitType	type) const
	{
		return locEmitMesh[type];
	}
	/*
	const GLuint	GetEmitMeshTextureLocation() const
	{
		return locEmitTexture;
	}
	*/
	const bool IsBindlessTexturesSupported() const
	{
		return bindlessTexturesSupported;
	}

protected:

	bool				bindlessTexturesSupported;

	//
	// stuff effect, tech and pass interfaces
	//
	nvFX::IContainer*   fx_Effect;
    
	
	nvFX::ITechnique	*fx_TechEmit[eTechEmitCount];

	nvFX::IPass			*fx_passEmitCurrent;

	/*
	nvFX::ITechnique*   fx_TechSimulate;
	nvFX::IPass*        fx_passSimulate;
	*/
	nvFX::ITechnique*   fx_TechRenderPoints;
	nvFX::IPass*        fx_passRender;
	
	nvFX::ITechnique	*fx_TechTerrainPrep;
	nvFX::IPass			*fx_passTerrainPrep;
	nvFX::ITechnique	*fx_TechTerrainPreview;
	nvFX::IPass			*fx_passTerrainPreview;

	nvFX::ICstBuffer	*fx_evaluateBlock;
	nvFX::ICstBuffer	*fx_renderBlock;
	nvFX::ICstBuffer	*fx_terrainBlock;

	nvFX::IUniform		*fx_gTime;
	nvFX::IUniform		*fx_gDeltaTime;

	nvFX::IUniform		*fx_terrainModelTM;

	// locations for evaluate GS uniforms
	GLuint				locPositionBuffer;
	GLuint				locNormalBuffer;

	GLuint				locTerrainData;	// terrain 2d sampler

	GLuint				locCollisions;	// pointer uniform
	GLuint				locForces;		// pointer uniform
	/*
	GLuint				techEmitGeometry;
	GLuint				locEmitMesh;
	GLuint				locEmitTexture;
	*/
	GLuint				emitGeometryProgram[eTechEmitCount];
	GLuint				locEmitMesh[eTechEmitCount];

	//
	//
	//GLuint				shaderCompute;
	GLuint				programCompute;

	GLuint				locComputeDeltaTime;
	GLuint				locComputeTime;

	GLuint				locComputeDynamic;
	GLuint				locComputeTurbulence;
	GLuint				locComputeGravity;
	GLuint				locComputeFloor;
	GLuint				locComputeNumForces;
	GLuint				locComputeNumCollisions;
	GLuint				locComputeUseSizeAtten;
	GLuint				locComputeUpdatePosition;	// do we have to run integrate shader after ?!

	//
	// self collisions compute shader
	//GLuint				shaderSelfCollisions;
	GLuint				programSelfCollisions;

	GLuint				locSelfCollisionsDeltaTime;

	//
	// euler integration compute shader
	GLuint				programIntegrate;

	GLuint				locIntegrateDeltaTime;
	GLuint				locIntegrateNumCollisions;

	// terrain uniforms

	GLuint				techTerrainPreviewFragment;
	GLuint				locTerrainTextureAddress;


	bool loadEffect(const char *effectFileName );
	void clearResources();


	static void checkCompileStatus(GLuint shader, const char *shadername);
    static void checkLinkStatus(GLuint program, const char * programName);

	static GLuint loadComputeShader(const char* computeShaderName);
};

};