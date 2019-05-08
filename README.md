# Romgere Cockpit Arduino Lib

Arduino library for building "real" cockpit/panel with X-Plane and Arduino board.

I use it to build my own "cockpit", an Beechcraft Baron 58 "cockpit" (Still work in progress) :
![My Beechcraft Baron 58 cockpit (WIP)](/resources/img/my_cockpit.jpg?raw=true)
*Instruments display not done with this library*

# WIP
*2019/05/04 :  This library was just moved from bitbucket (private repository) to github and need some work for writing doc/example, translate, review project folder architecture.*

**Please be careful if you using this library as is.**

**Please note that the Slave/Master board mode does not seems to work properly.**

# Documentation

The documentation file are stored in the [`resources/doc`](resources/doc/index.md) directory :

* [Getting started](resources/doc/index.md#getting-started)
  * [Prerequisites](resources/doc/index.md#prerequisites)
  * [How it works](resources/doc/index.md#how-it-works)
  * [Use library on your own sketches](resources/doc/index.md#use-library-on-your-own-sketches)
  * [X-Plane configuration](resources/doc/index.md#x-plane-configuration)
* [Further documentation](resources/doc/index.md#further-documentation)
  * [Configuration reference](resources/doc/1-configuration-reference.md)
  * [Basic example](resources/doc/2-basic-example.md)
  * [Xplane command](resources/doc/3-commands.md)
  * [Control (hardware)](resources/doc/4-controls.md)
  * [Use multiple Arduino boards](resources/doc/5-multiple-boards.md)
  * [Transformation function](resources/doc/6-transformation-function.md)

# Todos

- [x] Translate comment and other text
- [x] Change all file header
- [x] Modify folder and tree to something like lib / test / example / doc...
- [ ] Add "this->" on some methods calls
- [x] Re-indent all pre-compilation conditional blocks
- [ ] Test and Fix Master/Slave board mode
- [ ] Change example and delete all FSX references
- [ ] Write doc and Readme (WIP)
- [ ] Add some "todos" and/or open issues
- [ ] Implements DREF and DATA command support
- [ ] Implements analog control (input and ouput)
- [ ] X-plane 11 support

# License

This library is under the MIT license.  
For the whole copyright, please see the [LICENSE](LICENSE) file.
