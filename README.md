
![experimental](https://img.shields.io/badge/stability%3F-experimental-orange.svg)
![maintained](https://img.shields.io/maintenance/yes/2003.svg)

*This repository is kept as an archive only*

# Introduction

KOF 91 (aka KOF Linux or FFE) is a 2D Game Engine like Street Fighter but everything is easily customizable so you can easily add characters & Backgrounds, etc or even make your own game.

This repository attempts to archive the project, at least making sure it builds.

# History

KOF 91 was first developed by Moah on Win32 in the early 98 (see http://www.kof91.com ). The Linux version was a port by Duncan Mac-Vicar P. and Ric's Brain (who added patches before).

Allegedly, there's a GP32 port made by Thor ( http://www.geocities.com/rtb7 ) and Ric Leite developed new features, under a KOF2003 project.

A new OS independant version is still was still in developement during 2004-2007 by Laurent Dufour (zesensei) in the original [KOF91](http://kof91.sourceforge.net) website. This project aimed to achieve [M.U.G.E.N](https://en.wikipedia.org/wiki/M.U.G.E.N) compatibility. In 2007 Lauren announced he was not working on it anymore.

Several of these websites are nowadays down.

# Building

## Requirements

In order to build, you need

* C compiler
* [Allegro 4.x](http://liballeg.org/)

## Steps

Compared to the original, the buildsystem is ported to [cmake](https://cmake.org/).

```shell
cmake -S . -B build
cmake --build build
```

## Playing

Call the binary from the original tree structure in [k91v12s.zip](https://archive.org/download/k91v12s/k91v12s.zip) (containing
`midi`, `sprites`, `wav`, `bkgds`, etc).

# License

The original KOF91 was under the GPL license, and therefore all derivatives follow the same license.
