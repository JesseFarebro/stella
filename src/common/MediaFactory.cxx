#include "MediaFactory.hxx"

MediaFactory::OSystemCreator MediaFactory::createOSystem = MediaFactory::defaultHandler<OSystem>;
MediaFactory::SettingsCreator MediaFactory::createSettings = MediaFactory::defaultHandler<Settings>;
MediaFactory::SerialPortCreator MediaFactory::createSerialPort = MediaFactory::defaultHandler<SerialPort>;
MediaFactory::FrameBufferCreator MediaFactory::createVideo = MediaFactory::defaultHandler<FrameBuffer, OSystem&>;
MediaFactory::SoundCreator MediaFactory::createAudio = MediaFactory::defaultHandler<Sound, OSystem&, AudioSettings&>;
MediaFactory::EventHandlerCreator MediaFactory::createEventHandler = MediaFactory::defaultHandler<EventHandler, OSystem&>;
