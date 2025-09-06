#ifndef AUDIO_STATE_H
#define AUDIO_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum AudioState {
  Disabled,
  Enabled,
  Play,
  Pause,
  Stop,
} AudioState;

#ifdef __cplusplus
}
#endif

#endif  // AUDIO_STATE_H