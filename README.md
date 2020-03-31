# Piecemaker2d

 - Panel cut / cutting / sawing optimization for hobby users, craftmen, carpenters, joiners, architects

**already used in production**

The `piecemaker2d` application calculates cutting instructions that tell you how to cut a (twodimensional) basic plate into the set of pieces you need (guillotine panel cut optimization), e.g. for panels from stock sheets of wood. `piecemaker2d` is optimizing the cutting plan in order to minimize waste. The tool may be useful for hobby users, craftmen, carpenters, joiners, architects, and others.

Originally, the tool was developed for usage under MS-DOS. Therefore, it is based on command line invocation and file input/output. The tool can easily be called from other applications. The binaries are less than 100kB in size, RAM usage is less than 1MB. The program can deal with huge panel sizes and a long bill of materials. As for almost all optimization algorithms, there is no guarantee that the tool finds the best solution, but it always finds a good one. It is/was already used in production by a joiner.


## Other open source solutions

 * cutlet https://github.com/mru00/cutlet/
 * COPanno https://sourceforge.net/projects/copanno
 * Cut Micro https://sourceforge.net/projects/ctmc
 * Cutting Problem https://sourceforge.net/projects/cuttingproblem


## Usage

For usage see the manuals (English and German) in the `doc` directory. You must define parameters in a config file (including name and size of the plates to be cut), a bill of material (number and sizes of the pieces you need) and then just run `piecemaker2d`.

Easiest way for usage is to copy and edit one of the usage examples in the `test` folder.


## Implementation

The tool is implemented in C using make, bison, and yacc. No GUI is implemented. All customizing, input, and output is file based. The sources in the `src` folder should compile using any GNU C environment. I used a 32bit MinGW package for MS-Windows. For compilation, just type  
    make install  
Please find binaries for MS-DOS / MS-Windows in the `bin` folder.

If you want to contribute, please do so, any help would be appreciated.

If you want to use this project, please do so. I would be happy to hear how it is working for you.


## Example result

![image](https://github.com/ManfredMaennle/Piecemaker2d/blob/master/piecemaker2d_example01_result.PNG)
