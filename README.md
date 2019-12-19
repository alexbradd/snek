# snek
Snek is a simple snake clone running in a terminal. It was written just
for fun.

## Compilation
Snek can be compiled on Windows, Linux and OSX. 

### Linux, OSX and other unixes
To compile snek, if make is installed in your system, simply run in a terminal in the project directory: 

```sh
make
```

If `make` is not installed, you can compile it manually with:

```sh
gcc main.c graphics.c input.c -o snek
```

Obviously `gcc` and the standard C library should be installed.

### Windows
To compile snek you should first install MinGW and the `gcc` compiler. Add the `bin` subfolder of your MinGW installation
path to your PATH environment variable. If you don't know what I am talking about, I suggest reading 
[this helpful link](http://www.mingw.org/wiki/MinGW_for_First_Time_Users_HOWTO).

After the setup open a command prompt, navigate to the project directory and run:

```sh
gcc main.c graphics.c input.c -o snek
```

## Running
Since snek is a terminal application, it should be run inside a terminal. So navigate to the project directory, compile
the program as described above and run:

```sh
./snek # For Linux, OSX and other unixes
snek   # For Windows
```

(**Note**: you don't have to write everything after the `#`, `#` included)

## Compatibility
The code *should* be cross platform. However it has not been tested.

Tested platforms:

- Linux
- Windows 10 1909

If you use OSX please try it and feel free to open a bug report for the problems 
you found.
