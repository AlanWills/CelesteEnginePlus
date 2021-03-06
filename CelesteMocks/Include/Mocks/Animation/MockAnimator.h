#pragma once

#include "StaticLibExport.h"
#include "Animation/Animator.h"


namespace CelesteMocks
{
  class MockAnimator : public Celeste::Animation::Animator
  {
    DECLARE_UNMANAGED_COMPONENT(MockAnimator, StaticLibExport)

    public:      
      size_t getCurrentFrame_Public() const { return getCurrentFrame(); }
      void setCurrentFrame_Public(size_t newFrame) { setCurrentFrame(newFrame); }

      void setPlaying_Public(bool isPlaying) { setPlaying(isPlaying); }

      float getCurrentSecondsPerFrame_Public() const { return getCurrentSecondsPerFrame(); }
      void setCurrentSecondsPerFrame_Public(float currentScondsPerFrame) { setCurrentSecondsPerFrame(currentScondsPerFrame); }

    private:
      using Inherited = Celeste::Animation::Animator;
  };
}