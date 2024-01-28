// This is a *very* minimal Audio Unit example. So minimal, in fact, that not only will it 
// fail auval, it will fail to do anything useful at all in any DAW.
// It is intended to demonstrate, with as little code/configuration as possible, how to
// build an AudioUnit at the command line with clang, and get it to actually be recognized
// by the system, and respond to the basic factory/initialization calls.

// I always have trouble finding the right Apple docs. This looks like the right area:
// https://developer.apple.com/documentation/audiotoolbox/audiocomponentplugininterface?language=objc

#include <AudioToolbox/AudioToolbox.h>
#include <stdio.h>

// We won't even get to the point of hitting this callback without more initialization code,
// but it seems nice to leave in here anyway.
// https://developer.apple.com/documentation/audiotoolbox/aurendercallback?language=objc
static OSStatus RenderCallback(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList *ioData) 
{
  for (UInt32 channel = 0; channel < ioData->mNumberBuffers; channel++) {
    Float32 *data = (Float32 *)ioData->mBuffers[channel].mData;
    for (UInt32 frame = 0; frame < inNumberFrames; frame++) {
      data[frame] *= 0.5f;
    }
  }
  return noErr;
}

// Initialize will be called before your plugin is asked to actually process audio.
static OSStatus Initialize(void* ptr) { return noErr; }
static OSStatus Uninitialize(void* ptr) { return noErr; }

typedef AudioComponentMethod Method;

// This is missing a lot of functions that you'll want to respond to in a real plugin.
static AudioComponentMethod Lookup(SInt16 selector) {
  switch (selector) {
    case kAudioUnitRenderSelect:       return (Method) RenderCallback;
    case kAudioUnitInitializeSelect:   return (Method) Initialize;
    case kAudioUnitUninitializeSelect: return (Method) Uninitialize;
    default:                           return NULL;
  }
}

// This should be called when you actually open the plugin in your DAW. Allocate the main 
// memory for your plugin here. (The factory function just allocates memory to hold a few
// function pointers.)
static OSStatus Open(void* ptr, AudioUnit compInstance) {
  return noErr;
}

// Free your main plugin memory here.
static OSStatus Close(void* ptr) {
  return noErr;
}

// Note: this can be called anything, but you have to put its name in the factoryFunction 
// key in Info.plist.
// Remember not to make this static! I think it's the single function that must be exported
// in the dynamic lib.
// It seems like the host will call free() on this returned pointer?
AudioComponentPlugInInterface* MinimalAUFactory(const AudioComponentDescription* desc) {
  AudioComponentPlugInInterface *acpi = (AudioComponentPlugInInterface*) malloc(sizeof(AudioComponentPlugInInterface)); 
  acpi->Open = Open;
  acpi->Close = Close;
  acpi->Lookup = Lookup;
  acpi->reserved = NULL;
  return acpi;
}
