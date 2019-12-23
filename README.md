# snek
Snek is a simple snake clone running in a terminal, written for fun.

## Minimum Requirements
A snake game with requirements? Unbelievably yes. You see, not all terminals
are created equal. Snek requires the support of ANSI vt100 escape codes to work
properly. However, support for these is not uniform across all platfroms, so
to run this small game properly you need:

- Any GNU/Linux distro that provides glibc >= 2.30
- Android newer enough to support the Termux app
- OSX with a terminal that suppports ANSI escapes (e.g. iTerm and iTerm2)
- Windows 10 build >= 10586.X (newer than November 2015). 
  [Check this article if you don't know where to look](https://www.howtogeek.com/236195/how-to-find-out-which-build-and-version-of-windows-10-you-have/) 

## Project Structure

- **src**: this folder contains source files
- **bin**: this folder contains precompiled binaries that save you the hassle

Simple right?

## Compilation
First clone the repository with `git clone` or by downloading the zip file.
Then chose depending on you operating system.

### Android

1. Install the Termux app for the Play Store or from F-Droid. If you already downloaded
   the files inside the repo, delete them as we are goning to clone the repo
   inside Termux 
2. Open Termux and at the prompt execute:

    ```sh
    pkg install git clang make -y
    git clone https://github.com/BreadyX/snek
    cd snek/src
    make
    ```

### Linux, OSX and other unixes

1. Make sure that a C compiler and the development headers are installed on your
   system. `gcc` is preferred (makefile default)
2. If `make` is installed on your system, in the `src` directory run: 

    ```sh
    make
    ```

    If `make` is not installed or you don't want to use `gcc`, you can compile 
    it manually with:

    ```sh
    CC -I./ -O2 -Wall main.c graphics.c input.c -o snek
    ```

    Where `CC` is your compiler of choice.

### Windows

1. First install Visual Studio (the free one is OK) and be sure to set it up
   for C\C++ development and to install the Windows SDK (version >= 10586.X).
2. Create a new empty solution
3. Import every C-related file in the `src` directory (drag 'n drop in the `Explore Solutions` pane)
4. Switch to the `Release` target. Doesn't matter if `x86` or `x64`
5. In the `Compilation` menu click `Compile solution` or hit CTRL-SHIFT-B
6. (Optional) If you want to export the executable go to your solution folder
   (the one with the `*.sln` file) and from the `Release` subfolder copy the 
   executable where you want

## Execution
Since snek is a terminal application, it should be run inside a terminal! 

If not installed, install `git`. Then run:

```sh
(Linux, OSX, and Android)
git clone https://github.com/BreadyX/snek
cd snek/bin
./snek

(Windows)
git clone https://github.com/BreadyX/snek
cd snek\bin
.\snek
```

## Compatibility
The code *should* be cross platform. However, due to *reasons*, it has been
tested on:

- Linux (Fedora 31)
- Adnroid (9.0 Pie)
- Windows 10 1909

If you use OSX please try it and feel free to open a bug report for the problems 
you found.
