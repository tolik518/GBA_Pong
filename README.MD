# ![GBA Pong](https://returnnull.de/images/pong_github.png)
[![Compiled GBA file](https://github.com/tolik518/GBA_Pong/actions/workflows/compile_gba.yml/badge.svg?event=push)](https://github.com/tolik518/GBA_Pong/actions/workflows/compile_gba.yml)

Just a version of Pong for Game Boy Advance.


![Pong Titlescreen](/.github/readme/pong-2.png)
![Pong Game](/.github/readme/pong-1.png?)
![Pong Game](/.github/readme/pong-3.png)


Download: [Nightly builds](https://returnnull.de/github/GBA_Pong/)
Naming scema: `[DATE CREATED]_[COMMIT HASH].zip`
____________

## How to start development (on Linux)
* prerequisites: get [docker](https://www.docker.com/get-started/) and [docker-compose](https://docs.docker.com/compose/install/)
* clone repository
* go to the root of the repository
* build docker image with `make build_image`
* optional: get include files with `make getincludes` (`make deleteincludes` to remove them)
* recommended: install [mGBA](https://mgba.io/downloads.html)

![Installing the project and compiling the game](/.github/readme/demo.gif)



## How to start the Game
* use `make compile` to compile the source (you'll find it in the /out folder)
* or use `make run` to compile and run the compiled game
    * probably needs to be configured for your emulator in case you use an other than `visualboyadvance`
* `make cleanup` to delete all the build data again

## Whats my goal of that project?
* to learn C basics
* to learn basics of the GBA
* gaining experience in DevOps ([Docker](https://github.com/tolik518/GBA_Pong/blob/master/docker/dkp_compiler/Dockerfile), [CI/CD](https://github.com/tolik518/GBA_Pong/blob/master/.github/workflows/compile_gba.yml))
* gaining practice with my [gba docker template](https://github.com/tolik518/GBA_Dev_Docker_Template)


_____

### Convert Images with grit
* use `make grit img=[your path starting from the, excluding, /code folder] args="[your arguments]"`
    * for example: `make grit img=img/pong_logo.png args="-ftc -gb -gB16"`
* after that just include the generated file and memcopy your image to the vram
    * for example: `tonccpy(m3_mem, pong_logoBitmap, 76800)`
_____

###### Repository created and maintained by [tolik518](https://github.com/tolik518).
###### Dockerfile based on the [Dockerfile](https://github.com/nchowning/dockerfiles/blob/master/switchdev/Dockerfile) from [Nathan Chowning](https://github.com/nchowning).
###### The Makefile is a mashup of the original from [devkitPro](https://github.com/devkitPro/gba-examples) and from [coranac](https://www.coranac.com/tonc/text/toc.htm).
###### The 'Pong'-font is based on [this BitmapFont](https://github.com/ianhan/BitmapFonts/blob/main/font-pack/4138906425_29cbc92641_o.png) by [Ian Hanschen](https://github.com/ianhan)
