# Langton Ant
## Dependencies

Libraries used : `glew`, `SFML`, `nlohmann::json`, `GLM` and `libglw`.

Every libraries except libglw are avalable in [vcpkg](https://github.com/microsoft/vcpkg).

## Note installation

Install dependencies with vcpkg.

Get `libglw` by init git submodules.

Configure the project via CMake.

After CMake configuration, in the CMakeCache.txt, make sure to put `LIBGLW_LIBRARY_ONLY` to `ON`.

Build.

## Configuration file

### Declaration

Put a `conf.json` file next to the program or call `LangtonAnt.exe --conf "/path/to/conf.json"`

### Structure

```json
{
    "winSize":{
        "x": 1280,
        "y": 720
    },
    "duration": 0,
    "maxColor": 250,
    "sizePixel": 4
}
```

* winSize : Size of the window. Default 1280x720.
* duration : If positive, time between ant move in seconds. If negative, number of skip move un 1/100 unit (e.g. -1 will skip 100 moves). if null, 1 move per frame. Default 0.
* maxColor : Maximum number of color state. Defaut 250 (cannot under 4)
* sizePixel : size of the texture pixel. Defaut 4.