# OperaThemeMaker
Create Themes for Opera and Opera GX.

![OTM Main Sample](/Resources/otmThemeVanilla.gif)

**Artwork by KeySixZero.**

**Artist Twitter:** https://twitter.com/KeysixZero

**Artist ArtStation:** https://www.artstation.com/keysixzero

## **Description**

OperaThemeMaker automates the creation of archive files to be consumed by various iterations of Opera Browsers to customize still and animated themes. Basic video editing capabilities are also included to add better personalization namely:

1. Vignette
2. Box Blur
3. Edge Detection
4. Monochrome

## **Theme Samples**

![OTM Theme Sample 1](/Resources/otmThemeVanilla.gif)

![OTM Theme Sample 2](/Resources/otmThemeVignMon.gif)

## **App Sample**

![OTM App Sample](/Resources/otmSample.gif)

## **Build**

### Cloning

In a terminal, enter the following commands.

```plaintext
git clone --recursive https://github.com/HawtStrokes/OperaThemeMaker.git
cd OperaThemeMaker/External/
git submodule update --recursive --init
```

### FFMPEG-CPP

Follow Installation guide at https://github.com/Raveler/ffmpeg-cpp

### GLFW

Use Cmake to generate the solution file. For ease of building, I suggest using cmake-gui. Open cmake-gui and point the source code to `<complete absolute path to>\OperaThemeMaker\External\glfw` and set the binaries location to `<complete absolute path to>\OperaThemeMaker\External\glfw\build`

Open the generated solution file in the build folder and build `Debug|x64` and `Release|x64` configurations.
### HawtLib

Open the solution file in the OperaThemeMaker root directory, and build `Release_Static` and `Debug_Static` configurations of HawtLib.

In file explorer, go to the following directories and open the following batch file:

*   In`OperaThemeMaker\External\HawtLib\HawtLib\Scripts` open `MakeIncludes.bat`

## **How It Works**

Video editing is done via ffmpeg-cpp, which adds the desired video effects and converts the input video to WEBM format. The output video, along with the thumbnail and persona.ini are then archived and placed in Opera's theme folder.

More information at https://dev.opera.com/articles/making-animated-themes/

## **Known Issues**

Using Monochrome and Edge Detection simultaneously corrupts the video output.

## **Dependencies**

- https://github.com/glfw/glfw
- https://github.com/ocornut/imgui
- https://github.com/Raveler/ffmpeg-cpp
- https://github.com/HawtStrokes/HawtLib
- https://github.com/AirGuanZ/imgui-filebrowser/
- https://github.com/tfussell/miniz-cpp
