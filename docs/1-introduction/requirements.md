# Requirements
## A foreward;
Our engine is built upon the fundamental concept of Open Source software and customizability; two things that are more common in software development than the game design industry.
With this, our engine comes with some tradeoffs. For one, our engine involves much more coding than usual, and assumes you have a working proficiency with some C-like language. On the other hand, however, because you have direct access to the source code, you can see exactly how the engine runs and why it operates the way it does. This can be used as is, changed to suit your needs, or extended into other fully featured things.

In place of some pre-built software like Unity or Unreal, the SadBoat engine requires you to build it yourself. This means that you need to write your program, compile your code, and link it against some already existing software libraries so that it can be exported as an executable file. This process is more involved than certain other engines, but is a necessary step in almost all computer software at some point in it's life. This means you need to know how to use a compiler and linker. We provide a makefile that should help with this, but it is specific to Linux.

Further, there's one more intricacy regarding our engine which may trip people up. We do not provide the files used by the linker, like Vulkan and OpenAL. It is assumed you provide these on your own. There's reasons for this; not every developer will use the same libraries, software versions will likely be updated often, some files do not provide precompiled libraries and must be compiled by their own standards, etc. Using Vulkan as an example; despite the fact that it is a cross platform tool, the Vulkan libraries differ from Windows to Linux to Mac. This means that downloading and compiling Vulkan is dependent on **your** development environment, not ours-- and so we cannot predict this for you. We use Mesa3D's implementation of Vulkan, because it's Open Source and compatible with Linux.

## The List: What Do you Need?
### Our Includes
Our engine is almost entirely a header only engine. In order to start using it, all you need is to include sb-engine.hpp and you should be all set.
All of our classes for ingame objects are under the namespace SBE to prevent conflicts with external tools.

### Catch
Our engine uses the Catch unit testing library to perform unit tests. Catch is a header only library, meaning we don't need to store it as a .lib, .a, .so, or .dll file and can provide it directly with our code. This helps ensure that bugs never leave our development branches, and the code you have access to is reliable.
Unit tests aren't the be-all-end-all of bug fixing. Our code *will* have errors in it, especially in some more complicated features that are hard to test. 
We recommend that if you desire to extend our feature set, you set up your own unit tests to catch bugs before they cause problems and not after. See test_blueprint.hpp for some blank tests.

### Rapidxml
Our engine utilizes the RapidXML library for loading data from XML files. RapidXML is a header only library, like Catch, and is provided with the engine.
RapidXML uses a structure that allows users to read data from XML files fairly easily. The documentation for RapidXML is not included in our table of contents, but is available online. We use version 1.13.

### Vulkan
Vulkan is a graphics programming interface used by our engine in order to send commands and data to the GPU for rendering. It is the backbone of all on-screen graphics and is required for anything visual to work. Vulkan is **not** a header only library-- it must be compiled separately from the engine, and then linked at compile time. This process depends on what compiler you're using, what system you're compiling on, and what version of Vulkan you're using. 

Our development tools and makefiles are designed to be used on Linux systems. There is an Open Source version of Vulkan that is available for install on Linux called Mesa3D. It is what we use in our engine. The documentation and installation instructions for Mesa3D are available on their site where you can also find the most recent version of their API.

**We provide a Vulkan installer with our engine, which installs the Mesa3D versions you'd use on Linux. This may not work on other systems.**

### OpenAL
OpenAL is an audio library built similarly to OpenGL. It is used by our engine in order to process and play audio. It is a required library which is not provided with the engine.

### A Game's Contents
Finally, we have the actual game itself. The code we provide allows the user to create games and systems much easier than if they were starting from scratch. Things like multithreading, object creation and colissions, sound effects, graphics, and more are provided to the user to hasten this. However, the system to manage these is dependent on what type of game you'd like to make.

Our main.cpp file provides some boilerplate for some simpler games, demonstrating how these systems could be used. We may, in time, provide demos aswell. However, actual game implementation and game logic is the responsibility of the user.

### Extra Resources  

[Last](https://www.github.com/Zomon333/SadBoat-Engine/tree/Linux-Refactor/docs/1-introduction/table-of-contents.md) --- [Next](https://www.github.com/Zomon333/SadBoat-Engine/tree/Linux-Refactor/docs/1-introduction/building.md)