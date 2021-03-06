/***************************************************************************
  qgsshadowrenderingframegraph.h
  --------------------------------------
  Date                 : August 2020
  Copyright            : (C) 2020 by Belgacem Nedjima
  Email                : gb underscore nedjima at esi dot dz
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSHADOWRENDERINGFRAMEGRAPH_H
#define QGSSHADOWRENDERINGFRAMEGRAPH_H

#include <QWindow>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QLayerFilter>
#include <Qt3DRender/QLayer>
#include <Qt3DRender/QRenderTargetSelector>
#include <Qt3DRender/QRenderTarget>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QFrustumCulling>
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QPolygonOffset>
#include <Qt3DRender/QRenderCapture>

#include "qgspointlightsettings.h"

class QgsDirectionalLightSettings;
class QgsCameraController;
class QgsRectangle;
class QgsPostprocessingEntity;
class QgsPreviewQuad;

#define SIP_NO_FILE

/**
 * \ingroup 3d
 * Container class that holds different objects related to shadow rendering
 *
 * \note Not available in Python bindings
 *
 * \since QGIS 3.16
 */
class QgsShadowRenderingFrameGraph : public Qt3DCore::QEntity
{
  public:
    //! Constructor
    QgsShadowRenderingFrameGraph( QWindow *window, Qt3DRender::QCamera *mainCamera, Qt3DCore::QEntity *root );

    //! Returns the root of the frame graph object
    Qt3DRender::QFrameGraphNode *getFrameGraphRoot() { return mRenderSurfaceSelector; }

    //! Returns the color texture of the forward rendering pass
    Qt3DRender::QTexture2D *forwardRenderColorTexture() { return mForwardColorTexture; }
    //! Returns the depth texture of the forward rendering pass
    Qt3DRender::QTexture2D *forwardRenderDepthTexture() { return mForwardDepthTexture; }
    //! Returns the shadow map (a depth texture for the shadow rendering pass)
    Qt3DRender::QTexture2D *shadowMapTexture() { return mShadowMapTexture; }

    //! Returns a layer object used to indicate that an entity is to be rendered during the post processing rendering pass
    Qt3DRender::QLayer *postprocessingPassLayer() { return mPostprocessPassLayer; }
    //! Returns a layer object used to indicate that an entity is to be rendered during the preview textures rendering pass
    Qt3DRender::QLayer *previewLayer() { return mPreviewLayer; }
    //! Returns a layer object used to indicate that an entity will cast shadows
    Qt3DRender::QLayer *castShadowsLayer() { return mCastShadowsLayer; }
    //! Returns a layer object used to indicate that an entity will be rendered during the forward rendering pass
    Qt3DRender::QLayer *forwardRenderLayer() { return mForwardRenderLayer; }

    //! Returns the main camera
    Qt3DRender::QCamera *mainCamera() { return mMainCamera; }
    //! Returns the light camera
    Qt3DRender::QCamera *lightCamera() { return mLightCamera; }
    //! Returns the postprocessing entity
    QgsPostprocessingEntity *postprocessingEntity() { return mPostprocessingEntity; }
    //! Returns the root entity of the entities related to the frame graph (like the post processing entity and preview entity)
    Qt3DCore::QEntity *rootEntity() { return mRootEntity; }

    //! Returns the render capture object used to take an image of the scene
    Qt3DRender::QRenderCapture *renderCapture() { return mRenderCapture; }

    //! Returns whether frustum culling is enabled
    bool frustumCullingEnabled() const { return mFrustumCullingEnabled; }
    //! Sets whether frustum culling is enabled
    void setFrustumCullingEnabled( bool enabled );

    //! Returns whether shadow rendering is enabled
    bool shadowRenderingEnabled() const { return mShadowRenderingEnabled; }
    //! Sets whether the shadow rendering is enabled
    void setShadowRenderingEnabled( bool enabled );

    //! Returns the shadow bias value
    float shadowBias() const { return mShadowBias; }
    //! Sets the shadow bias value
    void setShadowBias( float shadowBias );

    //! Returns the shadow map resolution
    int shadowMapResolution() const { return mShadowMapResolution; }
    //! Sets the resolution of the shadow map
    void setShadowMapResolution( int resolution );

    //! Sets the clear color of the scene (background color)
    void setClearColor( const QColor &clearColor );
    //! Adds an preview entity that shows a texture in real time for debugging purposes
    QgsPreviewQuad *addTexturePreviewOverlay( Qt3DRender::QTexture2D *texture, const QPointF &centerNDC, const QSizeF &size, QVector<Qt3DRender::QParameter *> additionalShaderParameters = QVector<Qt3DRender::QParameter *>() );
    //! Sets shadow rendering to use a directional light
    void setupDirectionalLight( const QgsDirectionalLightSettings &light, float maximumShadowRenderingDistance );
    //! Sets eye dome lighting shading related settings
    void setupEyeDomeLighting( bool enabled, double strength, int distance );
    //! Sets the shadow map debugging view port
    void setupShadowMapDebugging( bool enabled, Qt::Corner corner, double size );
    //! Sets the depth map debugging view port
    void setupDepthMapDebugging( bool enabled, Qt::Corner corner, double size );

  private:
    Qt3DRender::QRenderSurfaceSelector *mRenderSurfaceSelector = nullptr;
    Qt3DRender::QViewport *mMainViewPort = nullptr;
    Qt3DRender::QCameraSelector *mMainCameraSelector = nullptr;
    Qt3DRender::QLayerFilter *mForwardRenderLayerFilter = nullptr;
    Qt3DRender::QRenderTargetSelector *mForwardRenderTargetSelector = nullptr;
    Qt3DRender::QRenderTarget *mForwardRenderTarget = nullptr;
    Qt3DRender::QRenderTargetOutput *mForwardRenderTargetColorOutput = nullptr;
    Qt3DRender::QRenderTargetOutput *mForwardRenderTargetDepthOutput = nullptr;
    Qt3DRender::QTexture2D *mForwardColorTexture = nullptr;
    Qt3DRender::QTexture2D *mForwardDepthTexture = nullptr;
    Qt3DRender::QClearBuffers *mForwardClearBuffers = nullptr;
    Qt3DRender::QFrustumCulling *mFrustumCulling = nullptr;
    bool mFrustumCullingEnabled = true;

    Qt3DRender::QCamera *mMainCamera = nullptr;
    Qt3DRender::QLayerFilter *mPostprocessPassLayerFilter = nullptr;
    Qt3DRender::QClearBuffers *mPostprocessClearBuffers = nullptr;

    // texture preview
    Qt3DRender::QLayerFilter *mPreviewLayerFilter = nullptr;
    Qt3DRender::QRenderStateSet *mPreviewRenderStateSet = nullptr;
    Qt3DRender::QDepthTest *mPreviewDepthTest = nullptr;
    Qt3DRender::QCullFace *mPreviewCullFace = nullptr;

    // shadow rendering pass
    Qt3DRender::QRenderTargetSelector *mShadowRenderTargetSelector = nullptr;
    Qt3DRender::QRenderTarget *mShadowRenderTarget = nullptr;
    Qt3DRender::QRenderTargetOutput *mShadowRenderTargetOutput = nullptr;
    Qt3DRender::QTexture2D *mShadowMapTexture = nullptr;
    Qt3DRender::QClearBuffers *mShadowClearBuffers = nullptr;
    Qt3DRender::QCamera *mLightCamera = nullptr;
    Qt3DRender::QCameraSelector *mLightCameraSelector = nullptr;
    bool mShadowRenderingEnabled = false;
    float mShadowBias = 0.00001f;
    int mShadowMapResolution = 2048;

    bool mEyeDomeLightingEnabled = false;
    double mEyeDomeLightingStrength = 1000.0;
    int mEyeDomeLightingDistance = 1;

    QgsPreviewQuad *mDebugShadowMapPreviewQuad = nullptr;
    QgsPreviewQuad *mDebugDepthMapPreviewQuad = nullptr;

    Qt3DRender::QLayerFilter *mShadowSceneEntitiesFilter = nullptr;
    Qt3DRender::QRenderStateSet *mShadowRenderStateSet = nullptr;
    Qt3DRender::QCullFace *mShadowCullFace = nullptr;
    Qt3DRender::QDepthTest *mShadowDepthTest = nullptr;

    Qt3DRender::QRenderCapture *mRenderCapture = nullptr;

    QVector3D mLightDirection = QVector3D( 0.0, -1.0f, 0.0f );

    Qt3DCore::QEntity *mRootEntity = nullptr;

    Qt3DRender::QLayer *mPostprocessPassLayer = nullptr;
    Qt3DRender::QLayer *mPreviewLayer = nullptr;
    Qt3DRender::QLayer *mForwardRenderLayer = nullptr;
    Qt3DRender::QLayer *mCastShadowsLayer = nullptr;

    QgsPostprocessingEntity *mPostprocessingEntity = nullptr;

    QVector<QgsPreviewQuad *> mPreviewQuads;

    Qt3DRender::QFrameGraphNode *constructShadowRenderPass();
    Qt3DRender::QFrameGraphNode *constructForwardRenderPass();
    Qt3DRender::QFrameGraphNode *constructTexturesPreviewPass();
    Qt3DRender::QFrameGraphNode *constructPostprocessingPass();

    Q_DISABLE_COPY( QgsShadowRenderingFrameGraph )
};

#endif // QGSSHADOWRENDERINGFRAMEGRAPH_H
