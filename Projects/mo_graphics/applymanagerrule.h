#ifndef MoGraphics_applymanagerrule_h__
#define MoGraphics_applymanagerrule_h__

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// file: applymanagerrule.h
//
//	Author Sergey Solokhin (Neill3d)
//
//	GitHub page - https://github.com/Neill3d/MoPlugs
//	Licensed under BSD 3-Clause - https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Registration define
#define MOGRAPHICSASSOCIATION__CLASSNAME	    MoGraphicsAssociation
#define MOGRAPHICSASSOCIATION__CLASSSTR		"MoGraphicsAssociation"

#include <fbsdk/fbapplymanagerrule.h>

/////////////////////////////////////////////
//

/**	KAMRManipulatorAssociation class.
*	Provides custom context menu rules for ORManip_Association and ORModelItem.
*/
class MoGraphicsAssociation : public FBApplyManagerRule
{
	FBApplyManagerRuleDeclare(MoGraphicsAssociation, FBApplyManagerRule);

public:
	/** Return true if pSrc is the type of custom object which this rule to be defined for. 
    *
    *    @warning you should only return true if pSrc is the type of custom object you defined.
    */
    virtual bool IsValidSrc( FBComponent* /*pSrc*/);

    /** Return true if pDst is the type of custom or MB object which your custom object 
    *    pSrc will be applied on. it's possible to return proper destination and return it by pDst. 
    *    For example, pSrc is RagDoll Property, pDst is a one of IK/FK bone, then you can find
    *    associated Character and replace.
    *
    *    \param    pSrc    the custom type of object you drag it to attach another object.
    *    \retval    pDst    one of the selected destinations, it could be modified and return proper destination.
    *    \retval    pAllowMultiple    return true if allow multiple objects of same type as pSrc to be connected
    *                   to pDst.
    *    \return            return true if connection between pSrc and pDst is allowed.
    */
    virtual bool IsValidConnection( FBComponent* pSrc, FBComponent*& pDst, bool& pAllowMultiple);

	/** Build context menu for KAMRManipulatorAssociation.
	*
	*	\param	pAMMenu	the menu to add menu option on.
	*	\param	pFocusedObject	the ORModelItem type of object you right-click on.
	*	\return	return true if menu item has been added.
	*/
	virtual bool MenuBuild( FBAMMenu* pAMMenu, FBComponent* pFocusedObject);

	/** Perform action if the added menu item is clicked.
	*
	*	\param	pMenuId	the menu ID right-click on.
	*	\param	pFocusedObject	the ORModelItem type of object you right-click on.
	*	\return	return true if action has been performed.
	*/
	virtual bool MenuAction( int pMenuId, FBComponent* pFocusedObject);

protected:
	int mEditManipulatorId;		//!< The menu itme id for edit by manipulator association.
	int mCloseManipulatorId;	//!< The menu itme id for close manipulator association.

	int mCopyTextureFilenameId;
	int mRevealTextureFilenameId;

	int mCopyVideoFilenameId;
	int mRevealVideoFilenameId;

	int mReplaceProjTexId;

	FBSystem mSystem;


	void	CopyVideoClipFilename(FBVideoClip *pVideoClip);
	void	RevealVideoClipFilename(FBVideoClip *pVideoClip);
};

#endif // BlendShapeToolkit_applymanagerrule_h__
