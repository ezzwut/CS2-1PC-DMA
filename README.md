# CS2 DMA - In Development

CS2 DMA Software for 1 PC Setup. Optimized with bug fixes, new features.

In this ReadMe you can find the full installation & build guide.

![C++](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![Visual Studio](https://img.shields.io/badge/IDE-Visual%20Studio-5C2D91?logo=visual-studio&logoColor=white)
![Ready to build](https://img.shields.io/badge/status-ready--to--build-brightgreen)

## Installation

Firstly you need to download exe or build it from sources.

You need to put files in same 

- exe
- [offsets.json](https://github.com/a2x/cs2-dumper/blob/main/output/offsets.json)
- [client.dll.json](https://github.com/a2x/cs2-dumper/blob/main/output/client_dll.json)
- config
- FTD3XX.dll
- leechcore.dll
- vmm.dll

## Building

```bash
  git clone https://github.com/YourBrand/CS2-DMA-Software.git
```

Open the project (slnx file) with visual studio (builded with VS2026). 
Ready-to-build on Release.

## Configuration

Example for net kmbox
```json
{
    "type": "net", 
    "en": "en",
    "ip": "192.168.2.198",
    "port": 12345,
    "uuid": "0000000"
}
```
Example for BPro
```json
{
   "type":"b",
   "en": "en"
}
```

Possible values for: 
* "en" - en / ch (English, Chinese)
* "type" - net / net+ (encrypted net-kmbox) / b (its BPro) / none (to use without kmbox)
    
## Features

- **Visuals**
* ESP: Box, health bar, weapon, distance, name
* Eye ray
* Bones
* Lines to enemy
- **Radar**
* Size (small, big), map auto-selection
- **Languages**
* English & German & Turkish supported

## Screenshots

![](https://media.discordapp.net/attachments/1504997940844822680/1507483326133309610/image.png?ex=6a209140&is=6a1f3fc0&hm=300f1ad8a9d20041fa6336792e9650c46af78aed37da644ef7f60bea0f657def&=&format=webp&quality=lossless)


## FAQ

#### How to open & hide menu?

insert on your main keyboard. All binds works only on main computer.

#### vmm.dll not found...

Check one more time installation guide. Do not forget to put needed dlls to same folder as .exe.




##  Credits

- [@CS2_DMA_Extrnal](https://github.com/Mzzzj/CS2_DMA_Extrnal)
- [@KmBoxNet](https://github.com/TKazer/KmBoxNetManager)
- [@KmBoxB](https://github.com/sys-1337/kmbox-communication)

