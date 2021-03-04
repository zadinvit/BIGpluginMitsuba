**BIG mitsuba plugin**

**Instalation**

 1. Copy BIG folder to mitsuba/ext 
 2. add code to mitsuba/ext/CMakeLists.txt

> 
    # build UTIABIG
    add_subdirectory(BIG)
    set(BIG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/BIG PARENT_SCOPE)
    set_property(TARGET big         PROPERTY FOLDER "dependencies")
 3. Copy bigbtf.cpp to mitsuba/src/bsdfs
 4.  in mitsuba/src/bsdfs edit CMakeLists.txt add row
	 4. `add_plugin(bigbtf          bigbtf.cpp)`
 5.  in mitsuba/src/mitsuba/CMakeLists.txt add big to target_link_libraries (this is optional, this is code build big library with building mitsuba by deafult)
> 
> change `target_link_libraries(mitsuba PRIVATE mitsuba-core mitsuba-render tbb)`
> to `target_link_libraries(mitsuba PRIVATE mitsuba-core mitsuba-render tbb **big**) //add big project to build with mitsuba`


You can use https://mitsuba2.readthedocs.io/en/latest/generated/render_api.html#mitsuba.render.BSDFSample3f to generate newest cmakelist from BIG library (https://github.com/RadomirVavra/BIG), go to folder  vs"your visual code version"/big and copy CMakeLists.txt and copy all library files except vsxx and replace them in the BIG folder, in CMakeLists.txt generated in /big folder you must change paths to src a include files, comment use_props command in this file and delete all post build commands




Documentation create by Vít Zadina 

