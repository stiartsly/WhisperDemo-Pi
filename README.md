Whisper Demo Run on RaspberryPi
====================================

Whisper Demo is an easy-understand application to show what Whisper framework can do over whisper network. It shows you can use Apps on Android/iOS/Raspberry to control each other with p2p technology.

## Whisper network types

Currently, 2 types of whisper network supprted:

 * Managed whisper network
 * Decentralzied whisper network

## Feaures:

The items for remote control currently includes:

- Turn on/off torch (or light)
- Turn on/off ringtone
- Increase/Decrease ringtone volume
- Turn on/off camera video

## Build from source

Run follwing commands for compilation on MacOS or Linux:

```
$ git clone https://github.com/stiartsly/WhisperDemo-Pi.git wdemo
$ cd wdemo
$ mkdir build
$ cd build
$ cmake ..
$ make
```

or on RaspberryPi:

```
$ git clone https://github.com/stiartsly/WhisperDemo-Pi.git wdemo
$ cd wdemo
$ mkdir build
$ cd build
$ cmake -DENABLE_PI=ON ..
$ make
```

## Build dependencies

Before buiding whisper demo, you have to download and build the following dependencies:

* whisper framework
* libconfuse

and copy distributions to "deps" direcotry. 

The structure of "deps" derectory should looks as listed below:

```
|-- deps
   |-- include
       |-- whisper.h
       |-- whisper_session.h
       |-- confuse.h
   |-- libs
       |-- libwcommon.so
       |-- libwcore.so
       |-- libwsession.so
       |-- libconfuse.a
```

## Deploy && Run
TODO: 

## License

Whisper Demo påroject source code files are made available under the MIT License, located in the LICENSE file.