
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// file: ortexture_paramBlend_texture.cxx
//
//	Author Sergey Solokhin (Neill3d)
//
//	GitHub page - https://github.com/Neill3d/MoPlugs
//	Licensed under BSD 3-Clause - https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
//
///////////////////////////////////////////////////////////////////////////////////////////////////


//--- Class declaration
#include "ortexture_advanceBlend_texture.h"
#include <gl\glew.h>

#include "IO\FileUtils.h"

FBClassImplementation( ORTextureParamBlend );					        //Register class
FBStorableCustomTextureImplementation( ORTextureParamBlend, Texture );	//Register to the store/retrieve system
FBShadingElementClassImplementationWithLabel( ORTextureParamBlend, "Parametric Blend", "browsing/template_layeredtexture.png" );	//Register to the asset browser shading element category folder.

const char *gBlendVertexShaders[TOTAL_NUMBER_OF_SPRITE_SHADERS] = {
	"\\GLSL\\paramBlend.vsh",
	"\\GLSL\\paramBlend.vsh",
	"\\GLSL\\paramBlend.vsh"
};

const char *gBlendFragmentShaders[TOTAL_NUMBER_OF_SPRITE_SHADERS] = {
	"\\GLSL\\paramBlendVector.fsh",
	"\\GLSL\\paramBlendMatrix.fsh",
	"\\GLSL\\paramBlendMatrixBlend.fsh"
};

extern bool glewFirstRun;

/************************************************
 *	Constructor.
 ************************************************/
ORTextureParamBlend::ORTextureParamBlend( const char* pName, HIObject pObject ) : FBLayeredTexture( pName, pObject )
{
	FBClassInit;
	mSupported = true;

	for (int i=0; i<TOTAL_NUMBER_OF_SPRITE_SHADERS; ++i)
		mShaders[i] = nullptr;

	if (glewFirstRun)
	{
		glewInit();
		glewFirstRun = false;
	}
}

static void ORTextureParamBlend_SpriteOrderSet(HIObject pMbObject, ESpriteOrder pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
    pFbObject->SpriteOrder.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_CustomCompositionSet(HIObject pMbObject, bool pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
    pFbObject->CustomComposition.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_CountUSet(HIObject pMbObject, int pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
    pFbObject->CountU.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_CountVSet(HIObject pMbObject, int pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
    pFbObject->CountV.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_SpriteAnimationSet(HIObject pMbObject, bool pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
	pFbObject->SpriteAnimation.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_SpriteFPSSet(HIObject pMbObject, int pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
	pFbObject->SpriteFPS.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_SpriteFramesLimitSet(HIObject pMbObject, double pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
	pFbObject->SpriteFramesLimit.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_SpriteLocalPlaySet(HIObject pMbObject, bool pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
	pFbObject->SpriteLocalPlay.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

static void ORTextureParamBlend_SpriteLoopPlaySet(HIObject pMbObject, bool pValue)
{
    ORTextureParamBlend* pFbObject = FBCast<ORTextureParamBlend>( pMbObject );
	pFbObject->SpriteLoopPlay.SetPropertyValue(pValue);
    pFbObject->SetLayerConfigDirty();
}

/************************************************
 *	FiLMBOX Constructor.
 ************************************************/
bool ORTextureParamBlend::FBCreate()
{
	FBPropertyInitTextureConnectable(this, AuxLayer, "AuxLayer" );
    FBPropertyPublish(this, CustomComposition, "CustomComposition", NULL, ORTextureParamBlend_CustomCompositionSet);

	FBPropertyPublish(this, SpriteOrder, "Sprite Order", NULL, ORTextureParamBlend_SpriteOrderSet);

	FBPropertyPublish(this, CountU, "Count U", NULL, ORTextureParamBlend_CountUSet);
	FBPropertyPublish(this, CountV, "Count V", NULL, ORTextureParamBlend_CountVSet);
	FBPropertyPublish(this, ParamU, "Param U", nullptr, nullptr);
	FBPropertyPublish(this, ParamV, "Param V", nullptr, nullptr);

	FBPropertyPublish(this, SpriteAnimation, "Sprite Animation", NULL, ORTextureParamBlend_SpriteAnimationSet);
	FBPropertyPublish(this, SpriteFPS, "Sprite FPS", NULL, ORTextureParamBlend_SpriteFPSSet);
	FBPropertyPublish(this, SpriteFramesLimit, "Sprite Frames Limit", NULL, ORTextureParamBlend_SpriteFramesLimitSet);
	FBPropertyPublish(this, SpriteLocalPlay, "Sprite Local Play", NULL, ORTextureParamBlend_SpriteLocalPlaySet);
	FBPropertyPublish(this, SpriteLoopPlay, "Sprite Loop Play", NULL, ORTextureParamBlend_SpriteLoopPlaySet);

    AuxLayer = FBColorAndAlpha(1.0, 1.0, 1.0, 1.0);
    CustomComposition = true;

	SpriteOrder = eSpriteMatrix;

	CountU = 1;
	CountV = 1;

	CountU.SetMinMax(0.0, 128.0, true, true);
	CountV.SetMinMax(0.0, 128.0, true, true);

	ParamU = 0.0;
	ParamV = 0.0;

	ParamU.SetMinMax(0.0, 100.0, true, true);
	ParamV.SetMinMax(0.0, 100.0, true, true);

	SpriteAnimation = true;
	SpriteFPS = 12;
	SpriteFramesLimit = 100.0;
	SpriteLocalPlay = false;
	SpriteLoopPlay = true;

	mLastTime = FBTime::MinusInfinity;
	mSpriteParamU = 0.0;

	InitShaders();

    return true;
}

/************************************************
 *	FiLMBOX Destructor.
 ************************************************/
void ORTextureParamBlend::FBDestroy()
{
	ParentClass::FBDestroy();
}

bool ORTextureParamBlend::FbxStore(FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat)
{
    ParentClass::FbxStore(pFbxObject, pStoreWhat);

	return true;
}

bool ORTextureParamBlend::FbxRetrieve(FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat)
{
    ParentClass::FbxRetrieve(pFbxObject, pStoreWhat);

	return true;
}

bool ORTextureParamBlend::PlugDataNotify(FBConnectionAction pAction,FBPlug* pThis,void* pData,void* pDataOld,int pDataSize)
{
    if (pThis == &AuxLayer || pThis == &ParamU || pThis == &ParamV)
    {
        switch (pAction)
        {
        case kFBCandidated:
            {
                SetLayerConfigDirty();
            }
            break;
        default:
            break;
        }
    }

    return ParentClass::PlugDataNotify(pAction, pThis, pData, pDataOld, pDataSize);
}

bool ORTextureParamBlend::EvaluateAnimationNodes( FBEvaluateInfo* pEvaluateInfo )
{
    ParentClass::EvaluateAnimationNodes(pEvaluateInfo);

    if (CustomComposition)
	{

		if (SpriteAnimation && CountU > 0 && CountV > 0)
		{
			
			double paramOneFrame = 1.0 / (CountU * CountV);

			FBTime currTime = pEvaluateInfo->GetSystemTime();

			if (mLastTime == FBTime::MinusInfinity)
				mLastTime = currTime;

			double timeElapsed = currTime.GetSecondDouble() - mLastTime.GetSecondDouble();

			double framesElapsed = timeElapsed * SpriteFPS;
			mSpriteParamU += 100.0 * paramOneFrame * framesElapsed;

			while (mSpriteParamU > SpriteFramesLimit)
			{
				mSpriteParamU -= SpriteFramesLimit;
			}

			//
			mLastTime = currTime;

			// Trigger composition per frame.
			SetLayerConfigDirty();
		}

		if (AuxLayer.IsAnimated())
		{
    
			// Compute animatable property value in background evaluation thread.
			FBPropertyAnimatableColorAndAlpha::ValueType lTmpValue;
			AuxLayer.GetData(lTmpValue.mValue, sizeof(lTmpValue.mValue), pEvaluateInfo);

			// Trigger composition per frame.
			SetLayerConfigDirty();
		}

		if (ParamU.IsAnimated() )
		{
			double tempValue;
			ParamU.GetData(&tempValue, sizeof(double), pEvaluateInfo);

			// Trigger composition per frame.
			SetLayerConfigDirty();
		}

		if (ParamV.IsAnimated() )
		{
			double tempValue;
			ParamV.GetData(&tempValue, sizeof(double), pEvaluateInfo);

			// Trigger composition per frame.
			SetLayerConfigDirty();
		}
    }

    return true;
}

void ORTextureParamBlend::TextureLayerComposition(FBTime pTime,FBTime pTimeInCurrentTimeRef, int pWidth, int pHeight)
{
    // 
    // Here we simply demo the default and customized the texture layers blend operations. 
    //  Actually nothing is preventing user to do a more sophisticated real-time composition (keying and etc.,) 
    //  or even a full scene (or shadow map) RTT for advanced fancy tasks. 
    //
    if (!mSupported || !CustomComposition)
    {
        ParentClass::TextureLayerComposition(pTime, pTimeInCurrentTimeRef, pWidth, pHeight);
    }
	else if (mShaders[SpriteOrder] != nullptr && Layers.GetCount() > 0)
    {
        //
        //Render-To-Texture already setup. 
        //

        // Push GL states.
        glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);

        // Clear Buffers.

        // Default implementation use BackgroundColor property. Here AuxLayer is used for demo purpose. 
        // FBColorAndAlpha lBgColor = BackgroundColor;
        FBColorAndAlpha lBgColor = AuxLayer;
        glClearColor(lBgColor[0], lBgColor[1], lBgColor[2], lBgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0,0, pWidth, pHeight);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 1, 1, 0, -1, 1);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_FOG);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

		//
		// draw quad with a shader

		GLSLShader *pShader=mShaders[SpriteOrder];
		Locations &loc = mLocations[SpriteOrder];
		pShader->Bind();

		FBTexture *lTexture = Layers[0];
		lTexture->OGLInit();

		switch(SpriteOrder)
		{
		case eSpriteVector:
			pShader->bindTexture(GL_TEXTURE_2D, loc.tex, lTexture->TextureOGLId, 0);
		
			pShader->setUniformFloat( loc.countU, (float) CountU );
			pShader->setUniformFloat( loc.countV, (float) CountV );

			if (SpriteAnimation)
				pShader->setUniformFloat( loc.paramU, 0.01f * (float) mSpriteParamU );
			else
				pShader->setUniformFloat( loc.paramU, 0.01f * (float) ParamU );

			break;
		case eSpriteMatrix:
			pShader->bindTexture(GL_TEXTURE_2D, loc.tex, lTexture->TextureOGLId, 0);
		
			pShader->setUniformFloat( loc.countU, (float) CountU );
			pShader->setUniformFloat( loc.countV, (float) CountV );
			pShader->setUniformFloat( loc.paramU, 0.01f * (float) ParamU );
			pShader->setUniformFloat( loc.paramV, 0.01f * (float) ParamV );
			break;
		case eSpriteBlendedMatrix:
			pShader->bindTexture(GL_TEXTURE_2D, loc.tex, lTexture->TextureOGLId, 0);
		
			pShader->setUniformFloat( loc.countU, (float) CountU );
			pShader->setUniformFloat( loc.countV, (float) CountV );
			pShader->setUniformFloat( loc.paramU, 0.01f * (float) ParamU );
			pShader->setUniformFloat( loc.paramV, 0.01f * (float) ParamV );
			break;
		}

            // Draw quad for blending.
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 1.0);
            glVertex2f(0.0, 0.0);
            glTexCoord2f(0.0, 0.0);
            glVertex2f(0.0, 1.0);
            glTexCoord2f(1.0, 0.0);
            glVertex2f(1.0, 1.0);
            glTexCoord2f(1.0, 1.0);
            glVertex2f(1.0, 0.0);						
            glEnd();

		pShader->unbindTexture(GL_TEXTURE_2D, 0);
		pShader->UnBind();
		
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        // Pop GL states.
        glPopAttrib();
    }
}


bool ORTextureParamBlend::InitShaders()
{

	bool result = true;
	FBString effectPath, effectFullName;
	
	try
	{
		FreeShaders();

		if (false == FindEffectLocation( gBlendFragmentShaders[0], effectPath, effectFullName ) )
			throw std::exception( "Failed to locate shader files" );

		// most of shaders share the same simple vertex shader
		
		for (int i=0; i<TOTAL_NUMBER_OF_SPRITE_SHADERS; ++i)
		{
			mShaders[i] = new GLSLShader();

			if (false == mShaders[i]->LoadShaders( FBString(effectPath, gBlendVertexShaders[i]), FBString(effectPath, gBlendFragmentShaders[i]) ) )
				throw std::exception( "Failed to load shader" );
		
			//
			// find locations for all neede shader uniforms
		
			InitLocations(mShaders[i], mLocations[i]);
		}

	}
	catch ( const std::exception &e )
	{
		FBMessageBox( "Composite Master Tool", e.what(), "Ok" );
		result = false;

		FreeShaders();
	}

	return result;
}

bool ORTextureParamBlend::InitLocations(const GLSLShader *shader, Locations &locations)
{
	if (shader)
	{
		locations.tex = shader->findLocation( "tex" );
		locations.countU = shader->findLocation( "countU" );
		locations.countV = shader->findLocation( "countV" );
		locations.paramU = shader->findLocation( "paramU" );
		locations.paramV = shader->findLocation( "paramV" );
	}

	return true;
}

void ORTextureParamBlend::FreeShaders()
{
	for (int i=0; i<TOTAL_NUMBER_OF_SPRITE_SHADERS; ++i)
	{
		if (mShaders[i])
		{
			delete mShaders[i];
			mShaders[i] = nullptr;
		}
	}
}