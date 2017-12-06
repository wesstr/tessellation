# Tessellation in OpenGL
Basic tessellation implemented in OpenGL. This implementation uses a similar method found [here](http://in2gpu.com/2014/07/12/tessellation-tutorial-opengl-4-3/). Also loads the mini cooper model and displays it to the screen. Phong lighting is also implemented in the fragment shader.

## Usage
* To move the mini use up, down, left and right arrow keys
* To toggle wireframe press w
* To add or subtract to inner level subdivision press = to add and - to subtract
* To add or subtract to outer subdivision press ] to add and [ to subtract
* Pressing h will re-center the mini model
* Press esc to exit program

## Compilation
* Implemented using Visual Studio 2017 using static linking
* In order to clone repo git lfs must be installed
* To compile and run, clone repo, open Tessellation.sln, compile in Debug mode and run. Some additional configuration may be needed.

## Demo
Without wireframe. Notice that the specular highlights smoothen as the subdivision rate increases.

![alt text](https://github.com/wesstr/tessellation/blob/master/demo_imgs/tess.gif)

With wireframe

![alt text](https://github.com/wesstr/tessellation/blob/master/demo_imgs/tess_w.gif)

Some stupid subdivision amount. 100% GPU usage.

![alt text](https://github.com/wesstr/tessellation/blob/master/demo_imgs/tess-stupid.PNG)

[Video Demonstration](https://youtu.be/x3179oPMrNs)
