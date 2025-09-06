used board configuration: [esp32-s3 n16r8](https://github.com/handledexception/platform-espressif32/blob/esp32-s3-devkitc-1-n16r8v/boards/esp32-s3-devkitc-1-n16r8v.json)   

### Sound
To convert audio file use ffmpeg, for example:

`ffmpeg -i sfx2.ogg -f u8 -ar 44100 -ac 1 sfx2.pcm`

to plays *.pcm file, use `ffplay`, for example:

`ffplay -f u8 -ar 44100 -i sfx1.pcm`