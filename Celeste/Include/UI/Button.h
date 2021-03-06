#pragma once

#include "CelesteDllExport.h"
#include "Objects/Component.h"

#include <functional>


namespace Celeste
{
  class Path;
  class GameObject;

  namespace Input
  {
    class MouseInteractionHandler;
  }

  namespace Rendering
  {
    class SpriteRenderer;
  }

  namespace Audio
  {
    class AudioSource;
  }

  namespace Resources
  {
    class Texture2D;
    class Sound;
  }

  namespace UI
  {
    class Button : public Component
    {
      DECLARE_UNMANAGED_COMPONENT(Button, CelesteDllExport)

      public:
        using GameObjectClickCallback = std::function<void(GameObject&)>;

        enum class ButtonState
        {
          kIdle,
          kHighlighted,
          kClicked,
        };

        inline observer_ptr<Resources::Texture2D> getIdleTexture() const { return m_idleTexture; }
        CelesteDllExport void setIdleTexture(const Path& texturePath);

        inline observer_ptr<Resources::Texture2D> getHighlightedTexture() const { return m_highlightedTexture; }
        CelesteDllExport void setHighlightedTexture(const Path& texturePath);
        
        inline observer_ptr<Resources::Texture2D> getClickedTexture() const { return m_clickedTexture; }
        CelesteDllExport void setClickedTexture(const Path& texturePath);

        inline observer_ptr<Resources::Sound> getHighlightedSound() const { return m_highlightedSound; }
        CelesteDllExport void setHighlightedSound(const Path& texturePath);
        
        inline observer_ptr<Resources::Sound> getClickedSound() const { return m_clickedSound; }
        CelesteDllExport void setClickedSound(const Path& texturePath);

        template <typename ...Callbacks>
        void subscribeLeftClickCallback(const GameObjectClickCallback& callback, const Callbacks&... callbacks);
        CelesteDllExport void subscribeLeftClickCallback(const GameObjectClickCallback& callback);

      protected:
        inline ButtonState getButtonState() const { return m_state; }
        inline void setButtonState(ButtonState state) { m_state = state; }

      private:
        using Inherited = Component;

        void onEnter();
        void onLeave();
        void onLeftMouseButtonDown();
        void onLeftMouseButtonUp();

        ButtonState m_state = ButtonState::kIdle;

        observer_ptr<Resources::Texture2D> m_idleTexture = nullptr;
        observer_ptr<Resources::Texture2D> m_highlightedTexture = nullptr;
        observer_ptr<Resources::Texture2D> m_clickedTexture = nullptr;

        observer_ptr<Resources::Sound> m_highlightedSound = nullptr;
        observer_ptr<Resources::Sound> m_clickedSound = nullptr;

        observer_ptr<Rendering::SpriteRenderer> m_spriteRenderer = nullptr;
        observer_ptr<Input::MouseInteractionHandler> m_mouseInteraction = nullptr;
        observer_ptr<Audio::AudioSource> m_audio = nullptr;
    };

    //------------------------------------------------------------------------------------------------
    template <typename ...Callbacks>
    void Button::subscribeLeftClickCallback(const GameObjectClickCallback& callback, const Callbacks&... callbacks)
    {
      subscribeLeftClickCallback(callback);
      subscribeLeftClickCallback(callbacks...);
    }
  }
}