//
// Created by Tobias Hieta on 25/03/15.
//

#include "PowerComponent.h"
#include "player/PlayerComponent.h"
#include "input/InputComponent.h"
#include "settings/SettingsComponent.h"

#ifdef Q_OS_MAC
#include "PowerComponentMac.h"
#elif KONVERGO_OPENELEC
#include "PowerComponentOE.h"
#elif USE_X11POWER
#include "PowerComponentX11.h"
#elif defined(Q_OS_WIN32)
#include "PowerComponentWin.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////
PowerComponent& PowerComponent::Get()
{
#ifdef Q_OS_MAC
  static PowerComponentMac instance;
  return instance;
#elif KONVERGO_OPENELEC
  static PowerComponentOE instance;
  return instance;
#elif USE_X11POWER
  static PowerComponentX11 instance;
  return instance;
#elif defined(Q_OS_WIN32)
  static PowerComponentWin instance;
  return instance;
#else
  QLOG_WARN() << "Could not find a power component matching this platform. OS screensaver control disabled.";

  static PowerComponent instance;
  return instance;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
bool PowerComponent::componentInitialize()
{
  PlayerComponent* player = &PlayerComponent::Get();

  connect(player, &PlayerComponent::playbackActive, this, &PowerComponent::playbackActive);

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PowerComponent::setFullscreenState(bool fullscreen)
{
  m_fullscreenState = fullscreen;
  redecideScreeensaverState();
}

/////////////////////////////////////////////////////////////////////////////////////////
void PowerComponent::redecideScreeensaverState()
{
  bool enableOsScreensaver = !m_videoPlaying;

  // by default we don't allow the fullscreen state affect sleep state, but we want to
  // have a hidden option to allow system sleep and screensaver when in fullscreen.
  //
  bool preventSystemScreensaver = SettingsComponent::Get().value(SETTINGS_SECTION_MAIN, "preventSystemScreensaver").toBool();
#ifdef KONVERGO_OPENELEC
  preventSystemScreensaver = true;
#endif
  if (preventSystemScreensaver)
    enableOsScreensaver &= !m_fullscreenState;

  if (m_currentScreensaverEnabled != enableOsScreensaver)
  {
    m_currentScreensaverEnabled = enableOsScreensaver;
    if (enableOsScreensaver)
    {
      QLOG_DEBUG() << "Enabling OS screensaver";
      doEnableScreensaver();
      emit screenSaverEnabled();
    }
    else
    {
      QLOG_DEBUG() << "Disabling OS screensaver";
      doDisableScreensaver();
      emit screenSaverDisabled();
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
void PowerComponent::playbackActive(bool active)
{
  m_videoPlaying = active;
  redecideScreeensaverState();
}

/////////////////////////////////////////////////////////////////////////////////////////
void PowerComponent::componentPostInitialize()
{
  InputComponent::Get().registerHostCommand("poweroff", this, "PowerOff");
  InputComponent::Get().registerHostCommand("reboot", this, "Reboot");
  InputComponent::Get().registerHostCommand("suspend", this, "Suspend");
}

/////////////////////////////////////////////////////////////////////////////////////////
bool PowerComponent::checkCap(PowerCapabilities capability)
{
  if (!SettingsComponent::Get().value(SETTINGS_SECTION_MAIN, "showPowerOptions").toBool())
    return false;

  return (getPowerCapabilities() & capability);
}


