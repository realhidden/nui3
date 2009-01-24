/*
 *  nuiAudioDecoder.cpp
 *  nui3
 *
 *  Created by mat on 12/18/08.
 *  Copyright 2008 Mxp4. All rights reserved.
 *
 */
#include "nui.h"
#include "nuiAudioDecoder.h"
#include "nuiAudioConvert.h"

nuiAudioDecoder::nuiAudioDecoder(nglIStream& rStream) :
  nuiSampleReader(rStream),
  mpPrivate(NULL)
{
}

nuiAudioDecoder::nuiAudioDecoder(const nuiAudioDecoder& rDecoder, nglIStream& rStream) :
nuiSampleReader(rDecoder, rStream),
mpPrivate(NULL)
{
}

nuiAudioDecoder::~nuiAudioDecoder()
{
  Clear();
}

nuiSampleReader* nuiAudioDecoder::Clone(nglIStream& rStream) const
{
  nuiAudioDecoder* pDecoder = new nuiAudioDecoder(*this, rStream);
  pDecoder->SetPosition(0);
  return pDecoder;
}


void nuiAudioDecoder::SetPosition(uint32 pos)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return;
 
  if (Seek(pos))
  {
    mPosition = pos;
  }
}


uint32 nuiAudioDecoder::ReadIN(void* pBuffer, uint32 sampleframes, nuiSampleBitFormat format)
{
  //don't increment mPosition: it's already done in ReadDE
  uint32 channels = mInfo.GetChannels();
  std::vector<float*> temp(channels);
  std::vector<void*> tempVoid(channels);
  for (uint32 c= 0; c < channels; c++)
  {
    temp[c] = new float[sampleframes];
    tempVoid[c] = (void*)(temp[c]);
  }
  
  uint32 sampleFramesRead = ReadDE(tempVoid, sampleframes, eSampleFloat32);
  if (format == eSampleFloat32)
  {
    float* pFloatBuffer = (float*)pBuffer;
    //just interleave samples
    for (uint32 c = 0; c < channels; c++)
    {
      for (uint32 s = 0; s < sampleFramesRead; s++)
      {
        pFloatBuffer[s * channels + c] = temp[c][s];
      }
    }
  }
  else
  {
    //16 bits int are required, so interleave samples and convert them into float
    int16* pInt16Buffer = (int16*)pBuffer;
    for (uint32 c = 0; c < channels; c++)
    {
      nuiAudioConvert_DEfloatToINint16(temp[c], pInt16Buffer, c, channels, sampleFramesRead);
    }
    
  }
  
  for (uint32 c= 0; c < channels; c++)
  {
    delete[] temp[c];
  }
  
  return sampleFramesRead;
}


