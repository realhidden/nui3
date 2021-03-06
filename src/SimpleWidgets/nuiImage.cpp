/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nglImage.h"
#include "nuiImage.h"
#include "nuiDrawContext.h"
#include "nglString.h"

nuiImage::nuiImage (nuiTexture* pTexture, bool AlreadyAcquired)
  : nuiWidget(), mColor(255, 255, 255, 255)
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = pTexture;
  if (!AlreadyAcquired && pTexture)
    pTexture->Acquire();

  mUseAlpha = true;
  //SetFixedAspectRatio(true);

  mBlendFunc = nuiBlendTransp;
  ResetTextureRect();
}

nuiImage::nuiImage (nglIStream* pInput, nglImageCodec* pCodec)
  : nuiWidget(), mColor(255, 255, 255, 255)
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(pInput, pCodec);
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  mBlendFunc = nuiBlendTransp;
  ResetTextureRect();
}

nuiImage::nuiImage (const nglPath& rPath, nglImageCodec* pCodec)
  : nuiWidget(), mColor(255, 255, 255, 255)
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(rPath, pCodec);
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"),rPath.GetPathName());
  ResetTextureRect();
}

nuiImage::nuiImage (nglImageInfo& rInfo, bool Clone)
  : nuiWidget(), mColor(255, 255, 255, 255)
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(rInfo, Clone);
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  mBlendFunc = nuiBlendTransp;
  ResetTextureRect();
}

nuiImage::nuiImage (const nglImage& rImage)
  : nuiWidget(), mColor(255, 255, 255, 255)
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(rImage);
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  mBlendFunc = nuiBlendTransp;
  ResetTextureRect();
}

nuiImage::nuiImage (nglImage* pImage, bool OwnImage)
  : nuiWidget(), mColor(255, 255, 255, 255)
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(pImage,OwnImage);
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  mBlendFunc = nuiBlendTransp;
  ResetTextureRect();
}


void nuiImage::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglPath&>
               (nglString(_T("Texture")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiImage::GetTexturePath), 
                nuiMakeDelegate(this, &nuiImage::SetTexturePath)));

  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("TextureRect")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiImage::GetTextureRect), 
                nuiMakeDelegate(this, &nuiImage::SetTextureRect)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("Color")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiImage::GetColor), 
                nuiMakeDelegate(this, &nuiImage::SetColor)));
  
}


const nglPath& nuiImage::GetTexturePath()
{
  return mTexturePath;
}

void nuiImage::SetTexturePath(const nglPath& rTexturePath)
{
  mTexturePath = rTexturePath;

  nuiTexture* pTexture = nuiTexture::GetTexture(mTexturePath, NULL);
  SetTexture(pTexture);
  pTexture->Release();
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  SetProperty(_T("Source"), mTexturePath.GetPathName());
}

void nuiImage::SetTexture(nuiTexture* pTex)
{  
  if (pTex)
    pTex->Acquire();
  if (mpTexture)
    mpTexture->Release();

  mpTexture = pTex;
  mUseAlpha = true;
  //SetFixedAspectRatio(true);
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"), _T("Memory Buffer"));
  ResetTextureRect();
  Invalidate();
}

nuiImage::~nuiImage()
{
  if (mpTexture)
    mpTexture->Release();
}

void nuiImage::SetTextureRect(const nuiRect& rRect)
{
  mTextureRect = rRect;
  InvalidateLayout();
}

const nuiRect& nuiImage::GetTextureRect() const
{
  return mTextureRect;
}

void nuiImage::ResetTextureRect()
{
  if (mpTexture)
    SetTextureRect(nuiRect(0, 0, mpTexture->GetWidth(), mpTexture->GetHeight()));
  else
    SetTextureRect(nuiRect());
}


void nuiImage::ForceReload()
{
  mpTexture->ForceReload();
  Invalidate();
}

bool nuiImage::Draw(nuiDrawContext* pContext)
{
//  int x=0,y=0;
  if (!mpTexture)
  {
    return false;
  }

  pContext->PushState();
  pContext->ResetState();
  pContext->SetTexture(mpTexture);
//  mpTexture->Apply(pContext);
  pContext->EnableTexturing(true);
  //nglImage* pImage = mpTexture->GetImage();

  float alpha = 1.0f;

  if (mUseAlpha)
  {
    pContext->EnableBlending(true);
    pContext->SetBlendFunc(mBlendFunc);
    alpha = GetMixedAlpha();
  }

  nuiRect rect = mRect.Size();

  nuiColor c(mColor);
  c.Multiply(alpha);
  
  pContext->SetFillColor(c);
  pContext->DrawImage(rect, mTextureRect);
  
  pContext->EnableBlending(false);
  pContext->EnableTexturing(false);

  pContext->PopState();
  
  return true;
}

void nuiImage::SetBlendFunc(nuiBlendFunc Func)
{
  mBlendFunc = Func;
  Invalidate();
}


nuiBlendFunc nuiImage::GetBlendFunc()
{
  return mBlendFunc;
}


nuiRect nuiImage::CalcIdealSize()
{
  mIdealRect = mTextureRect.Size();
  if (mHasUserWidth != mHasUserHeight && GetFixedAspectRatio())
  {
    // Give good ratio to keep things in proportions
    float w = mUserRect.GetWidth();
    float h = mUserRect.GetHeight();
    float tw = mpTexture->GetWidth();
    float th = mpTexture->GetHeight();
    float r = 1.0f;

    if (mHasUserWidth)
    {
      if (w < tw)
      {
        r = w / tw;
        h = th * r;
      }
    }
    else
    {
      if (h < th)
      {
        r = h / th;
        w = tw * r;
      }
    }
    mIdealRect.Set(0.0f, 0.0f, w, h);
  }
    
  return mIdealRect;
}


void nuiImage::SetUseAlpha(bool Alpha)
{
  mUseAlpha = Alpha;
  Invalidate();
}

bool nuiImage::GetUseAlpha()
{
  return mUseAlpha;
}

nglImage* nuiImage::GetImage()
{
  if (!mpTexture)
    return NULL;
  return mpTexture->GetImage();
}

nuiTexture* nuiImage::GetTexture()
{
  return mpTexture;
}

void nuiImage::SetColor(const nuiColor& rColor)
{
  if (mColor == rColor)
    return;
  mColor = rColor;
  Invalidate();
}

const nuiColor& nuiImage::GetColor() const
{
  return mColor;
}

