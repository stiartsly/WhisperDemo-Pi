Whisper Demo Run on RaspberryPi
===============================

Whisper Demo is an exemplary and easy-understand app to show what whisper framework can do over whisper network. It shows you can use apps on Android/iOS/Raspberry to control remote device (vice versa) based on P2P technology.

## Whisper network types

Two types of whisper network would be supported:

 * Managed whisper network (or centralized whisper network)
 * Decentralized whisper network

## Features:

The items for remote control currently includes:

- Turn on/off torch (or flashlight)
- Increase/Decrease screen backgroud light
- Turn on/off ringtone
- Increase/Decrease ringtone volume
- Turn on/off camera video

## Build from source

Run following commands for compilation on MacOS or Linux:

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

Before building whisper demo, you have to download and build the following dependencies:

* whisper framework
* libconfuse

and copy distributions to **deps** direcotry.

The structure of **deps** derectory should look as listed below:

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

Run the following command to start wdemo:

```shell
$ wdemo -c YOUR-CONFIG-FILE.conf
```
or run command with option **-h** to get help information

```shell
$ wdemo -h
```

## License

MIT
