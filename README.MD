# GBA Pong

Pong for Game Boy Advance. Work in Progress.

For now there is no way to lose and is only featuring a "play by yourself" mode. So you can't lose for now even if you want to ¯\\\_(ツ)_/¯

![](https://returnnull.de/images/pong.gif)

## How to Start Development (on Linux)
* prerequisites: get [docker](https://www.docker.com/get-started/) and [docker-compose](https://docs.docker.com/compose/install/)
* clone repository
* go to the root of the repository
* build docker image with `make build_image`
* optional: get include files with `make getincludes` (`make deleteincludes` to remove them)
* recommended: install [mGBA](https://mgba.io/downloads.html) 

## Convert Images with grit
* use `make grit img=[your path starting from the, excluding, /code folder] args="[your arguments]"`
    * for example: `make grit img=img/pong_logo.png args="-ftc -gb -gB16"`
* after that just include the generated file and memcopy your image to the vram
    * for example: `tonccpy(m3_mem, pong_logoBitmap, 76800)`

## How to Start the Game
* use `make compile` to compile the source (you'll find it in the /out folder)
* or use `make run` to compile and run the compiled game
    * probably needs to be configured for your emulator in case you use an other than `visualboyadvance`
* `make cleanup` to delete all the build data again


_____

###### Repository originally maintained by [tolik518](https://github.com/tolik518). Dockerfile based on the Dockerfile from [Nathan Chowning](https://github.com/nchowning/dockerfiles/tree/master/switchdev). The Makefile is a mashup of the original from [devkitPro](https://github.com/devkitPro/gba-examples) and from [coranac](https://www.coranac.com/tonc/text/toc.htm).