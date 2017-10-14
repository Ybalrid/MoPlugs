
# script to test deformation along a curve

# Author: Sergey Solohin (Neill3d), e-mail to: s@neill3d.com
#  www.neill3d.com
#
# Github repo - https://github.com/Neill3d/MoPlugs
# Licensed under BSD 3-clause
#  https://github.com/Neill3d/MoPlugs/blob/master/LICENSE
#

from pyfbsdk import *
import math

gLocalTransformAxis = 0
gNegativeEffect = True

def DeformMesh(curve, mdl, parent, creationPercent, modifyType):
    
    # calculate object width
    vmin = FBVector3d()
    vmax = FBVector3d()
    mat = FBMatrix()
    scl = FBVector3d()
    pos = FBVector3d()
    parentPos = FBVector3d()
    offsetPos = FBVector3d()
    
    mdl.GetMatrix(mat, FBModelTransformationType.kModelScaling)
    mdl.GetVector(pos)
    parent.GetVector(parentPos)
    
    offsetPos = FBVector3d(parentPos[0]-pos[0], parentPos[1]-pos[1], parentPos[2]-pos[2])
    
    # calculate curve length
    curveLen = curve.PathLength
    
    # calculate position of a mesh on a curve
    
    f = creationPercent
    
    
    pivotPos = curve.Total_GlobalPathEvaluate( f )
    der = curve.Total_GlobalPathEvaluateDerivative( f )
    
    angle = 90.0 + 180.0 / 3.1415 * math.atan2(der[0], der[2])
    
    centerTM = FBMatrix()
    FBTRSToMatrix( centerTM, pivotPos, FBVector3d(0.0, angle, 0.0), FBSVector(1.0, 1.0, 1.0) )
    
    #mdl.SetVector( FBVector3d(pivotPos[0], pivotPos[1], pivotPos[2]) )
    #mdl.SetVector( FBVector3d(0.0, angle, 0.0), FBModelTransformationType.kModelRotation )
    
    
    # Begin modify geometry 
    lGeometry = mdl.Geometry
    lGeometry.GeometryBegin()
    
    posList = lGeometry.GetPositionsArray()
    numberOfPoints = len(posList)
    print numberOfPoints
    
    lShape0Idx = 0
    
    if modifyType == 1:
    
        # Add shape 
        lShape0Idx = lGeometry.ShapeAdd("DeformByCurve")
        
        # Init Shape
        lGeometry.ShapeInit(lShape0Idx, numberOfPoints, False)
    
    mdlMatrix = FBMatrix()
    mdl.GetMatrix(mdlMatrix)
    invMatrix = FBMatrix(mdlMatrix)
    invMatrix.Inverse()
    
    # Set shape diff point
    for i in range(0, numberOfPoints):    
        
        vertex = FBVertex( lGeometry.VertexGet(i) )
        #vertex[0] = 0.0
        
        # apply only scaling, but still not translation and rotation
        transformedVertex = FBVertex()
        FBVertexMatrixMult( transformedVertex, mat, vertex )
    
        # this is when we have direction in -X
        vertexLen = transformedVertex[gLocalTransformAxis]
        if gNegativeEffect:
            vertexLen = vertexLen * -1.0
        vertexF = creationPercent + 100.0 * vertexLen / curveLen
        
        transformedVertex[gLocalTransformAxis] = 0.0
        
        transformedVertex[0] -= offsetPos[0]
        transformedVertex[1] -= offsetPos[1]
        transformedVertex[2] -= offsetPos[2]
        
        pos = curve.Total_GlobalPathEvaluate( vertexF )
        der = curve.Total_GlobalPathEvaluateDerivative( vertexF )
        
        angle = 90.0 + 180.0 / 3.1415 * math.atan2(der[0], der[2])
        
        curTM = FBMatrix()
        FBRotationToMatrix( curTM, FBVector3d(0.0, angle, 0.0) )
        
        rotatedVertex = FBVertex()
        FBVertexMatrixMult( rotatedVertex, curTM, transformedVertex )
        
        newVertexPos = FBVector4d( pos[0]+rotatedVertex[0], pos[1]+rotatedVertex[1], pos[2]+rotatedVertex[2], 1.0 )
        FBVectorMatrixMult( newVertexPos, invMatrix, newVertexPos )

        if modifyType == 1:        
            diff = FBVertex(newVertexPos[0]-vertex[0], newVertexPos[1]-vertex[1], newVertexPos[2]-vertex[2], 0.0)        
            lGeometry.ShapeSetDiffPoint(lShape0Idx, i, i, diff)
        elif modifyType == 2:
            lGeometry.VertexSet(newVertexPos[0], newVertexPos[1], newVertexPos[2], i)
    
    # End modify geometry, notify for synchronization
    lGeometry.GeometryEnd()
    
        
    return 1


###########################################

models = FBModelList()
FBGetSelectedModels(models)

curve = None
for mdl in models:
    if isinstance(mdl, FBModelPath3D):
        curve = mdl

if len(models) > 1 and curve != None:
    
    modifyType = FBMessageBox("Deform Mesh By Curve", "Please select type of modification?", "BlendShape", "Geometry", "Cancel")

    if modifyType < 3:
    
        for mdl in models:
            if mdl != curve and not isinstance(mdl, FBModelNull):
                
                parent = mdl.Parent
                if parent == None:
                    continue
                    
                percent = 0.0
                lprop = parent.PropertyList.Find("CreationPercent")
                if lprop:
                    percent = lprop.Data
                    print percent
                
                    DeformMesh(curve, mdl, parent, percent, modifyType)

