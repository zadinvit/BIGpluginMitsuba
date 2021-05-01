**bigbtf mitsuba plugin**
This plugin is part of my diploma thesis "Visualization of advanced material appearence representations."
**Instalation**

 1. add code to mitsuba/ext/CMakeLists.txt 
 2. Copy folder bigplugin to mitsuba/src/bsdfs
 3.  in mitsuba/src/bsdfs edit CMakeLists.txt add a row
	 3.1. `add_plugin(bigbtf          bigplugin/bigbtf.cpp)`
 4.  now you need link libraries (you can use MIFlib 0.4 version from this repository (folder MIFlib) or download newer version from [https://github.com/RadomirVavra/MIFlib](https://github.com/RadomirVavra/MIFlib))
 5. Use Cmake to generate new Mitsuba.sln, in solution go to folder plugins/bsdfs/bigbtf, there are bigbtf project files
	 5.1. In bigbtf project you need set path to directories which stores MIFlib and PNGlib (in bigplugin folder). So in Linker->General->Additional Library Directories add the path to MIFlib 
	 5.2 In Linker->Input->Additional Dependencies add "MIFlib.lib;PNGlib.lib"
	 5.3 In bigbtf-obj project go to properties C/C++->General->Additional Include Directories and add the path to MIFlib\include and path to folder with bigbtf plugin include directory, you can add the path to plugin in mitsuba path_to_mitsuba\mitsuba\src\bsdfs\bigplugin\include
 6. Now you can build mitsuba INSTALL project and now you have mitsuba with bigbtf plugin, or you can build only bigbtf plugin and go to build/src/bsdfs/release and copy bigbtf.dll to dist/plugins, this work too and you don't need to reinstall whole Mitsuba

**Use**
To add BTF texture to your object you need to use this tag 

    <bsdf type="bigbtf" id="basket">
          <string name="filepath" value="F:\vita_skola\mitsubaTestFiles\mifFiles\MAM2014_016.mif"/>
          <boolean name="memory" value="true"/>
          <string name="cubemap_path" value="\cubemaps\081\0256"/>
          <float name="scale" value ="400.0"/>
          <string name="filtering" value="ANIZO_4x"/>
          <integer name="level" value="3" />
        </bsdf>
Only **filepath** parameter is mandatory, but i recomend use **memory** tag with true, for load BTF textures to memory while rendering. 
**For UBO reprenzentations of textures you need add path to cubemaps with interpolations indexes.** ( `<string name="cubemap_path" value="path_to\cubemaps\081\0256"/>`)
**Scale**  parameteris use to scale size of texture, default value is 7.
Plugin support mipmapping and anisotropy filtering, you can use it with parameter **filtering**, but you need to generate mif files with mipmaps this filtering (you can generate this files with [https://github.com/zadinvit/bigConvert](https://github.com/zadinvit/bigConvert))

You can use parameter **level** to change the default level of filtering, so if you want to start filtering closer to the camera add a positive value. 

***Types of filtering***

 - MIPMAP_LINEAR - mipmapping without interpolation between levels
 -  MIPMAP_WEIGHTED - mipmapping with interpolation between levels
 - ANIZO_1x - anisotropy filtering without interpolation between levelx and levely (recommended)
 -  ANIZO_4x anisotropy filtering with interpolation between levelx and levely (not recommended - slow + same results as ANIZO_1x)
 - none

Documentation created by Vít Zadina 

