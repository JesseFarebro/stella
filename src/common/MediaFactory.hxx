//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2020 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef MEDIA_FACTORY_HXX
#define MEDIA_FACTORY_HXX

#include "bspf.hxx"
#include "OSystem.hxx"
#include "Settings.hxx"
#include "SerialPort.hxx"
#include "SoundNull.hxx"
#include "FrameBuffer.hxx"
#include "Sound.hxx"
#include "EventHandler.hxx"


class AudioSettings;

/**
  This class deals with the different framebuffer/sound/event
  implementations for the various ports of Stella, and always returns a
  valid object based on the specific port and restrictions on that port.

  As of SDL2, this code is greatly simplified.  However, it remains here
  in case we ever have multiple backend implementations again (should
  not be necessary since SDL2 covers this nicely).

  @author  Stephen Anthony
*/
class MediaFactory
{
  private:
    template<class Type, typename... Args>
    static unique_ptr<Type> defaultHandler(Args... args) {
      throw runtime_error("media factory: type not registered");
    }

    template<class TDerived, typename... Args>
    static unique_ptr<TDerived> createHandler(Args... args) {
      return make_unique<TDerived>(std::forward<Args>(args)...);
    }

  public:
    using OSystemCreator = std::function<unique_ptr<OSystem>()>;
    using SettingsCreator = std::function<unique_ptr<Settings>()>;
    using SerialPortCreator = std::function<unique_ptr<SerialPort>()>;
    using FrameBufferCreator = std::function<unique_ptr<FrameBuffer>(OSystem&)>;
    using SoundCreator = std::function<unique_ptr<Sound>(OSystem&, AudioSettings&)>;
    using EventHandlerCreator = std::function<unique_ptr<EventHandler>(OSystem&)>;

    static OSystemCreator createOSystem;
    static SettingsCreator createSettings;
    static SerialPortCreator createSerialPort;
    static FrameBufferCreator createVideo;
    static SoundCreator createAudio;
    static EventHandlerCreator createEventHandler;

    static string backendName() {
      return "";
    }

    template<class T>
    static typename std::enable_if<std::is_base_of<OSystem, T>::value>::type Register() {
      MediaFactory::createOSystem = std::move(MediaFactory::createHandler<T>);
    }
    template<class T>
    static typename std::enable_if<std::is_base_of<Settings, T>::value>::type Register() {
      MediaFactory::createSettings = std::move(MediaFactory::createHandler<T>);
    }
    template<class T>
    static typename std::enable_if<std::is_base_of<SerialPort, T>::value>::type Register() {
      MediaFactory::createSerialPort = std::move(MediaFactory::createHandler<T>);
    }
    template<class T>
    static typename std::enable_if<std::is_base_of<FrameBuffer, T>::value>::type Register() {
      MediaFactory::createVideo = std::move(MediaFactory::createHandler<T, OSystem&>);
    }
    template<class T>
    static typename std::enable_if<std::is_base_of<Sound, T>::value>::type Register() {
      MediaFactory::createAudio = std::move(MediaFactory::createHandler<T, OSystem&, AudioSettings&>);
    }
    template<class T>
    static typename std::enable_if<std::is_base_of<EventHandler, T>::value>::type Register() {
      MediaFactory::createEventHandler = std::move(MediaFactory::createHandler<T, OSystem&>);
    }

  private:
    // Following constructors and assignment operators not supported
    MediaFactory() = delete;
    MediaFactory(const MediaFactory&) = delete;
    MediaFactory(MediaFactory&&) = delete;
    MediaFactory& operator=(const MediaFactory&) = delete;
    MediaFactory& operator=(MediaFactory&&) = delete;
};

#endif
