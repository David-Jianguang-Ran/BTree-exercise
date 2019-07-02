# BinarySearchTree (BTree)
In this assignment you will implement a BTree

Project has following folder tree

.  
├── CMakeLists.txt  
├── CMakeLists.txt.in  
├── README.md  
├── app  
│   └── main.cpp  
├── build  
├── code  
│   ├── BTree.cpp  
│   └── BTree.h  
└── tests  
    └── test_BTree.cpp  

CMakeLists.txt      : Ignore this file  
CMakeLists.txt.in   : Ignore this file  
README.md           : Readme file  
app\                : Application folder  
main.cpp            : Application main file for your personal tests. you can use this executable to debug your own classes.  
build\              : Build folder to build the project. your executables are gonna be here eventually.  
code\               : all your code should be in this folder.   
BTree.cpp           : BTree class source file  
BTree.h             : BTree class heather file  
tests\              : Tests folder  
test_BTree.cpp      : Tests implemented for you / your personal tests  



For your final submission you submitt BTree.cpp and BTree.h files. Any other changes in other folders will be ignored in grading process but feel free to change them if you want to test/debug something.

## Where to Start
Open a terminal window in Jupytherhub(recommended) or your personal linux/mac machine (no windows platforms). First pull the this repository by
```console
jovyan@jupyter-yourcuid:~$ git clone https://github.com/Colorado-CompSci-Ugrad/BTree.git
```
### If you want to use vscode environment
Then open the VScode app through JupyterHub and open BSST folder from vscode.

Now your environment is set up, change BTree.cpp and BTree.h files and eventually press CTRL+SHIFT+B to compile your code.

Open a terminal window in vscode and go into ''build'' folder and run tests by
```console
jovyan@jupyter-yourcuid:~$ ./run_tests
```
debugger is also set up for you, go to debug tab on the left column and select **Run Tests** in drop-down menu and press the green play button to run the debugger. if you have any implementation in **app/main.cpp** you can also debug that code by first choosing **Run App** in drop-down menu and pressing green play button.

### If you want to use linux terminal for comilation
Make sure you have the dependecies installed (ckeck dependencies in this document)
go into your project folder, then build folder
```console
jovyan@jupyter-yourcuid:~$ cd BTree
jovyan@jupyter-yourcuid:~$ cd build
```
run cmake to create make file for your project
```console
jovyan@jupyter-yourcuid:~$ cmake ..
```
run make to compile your code
```console
jovyan@jupyter-yourcuid:~$ make
```
once done, you can run tests by 
```console
jovyan@jupyter-yourcuid:~$ ./run_tests
```
app executable is also in this folder, you can run it by
```console
jovyan@jupyter-yourcuid:~$ ./run_app
```
you can debug in terminal using gdb

## Dependencies
you need **gcc** and **cmake** installed to be able to compile this code.

if you are using vscode environment in JupyterHub, you just need to make sure you have C/C++ extension installed.

## About This homework

### Difficult on purpose
This homework assignment is really hard. That's on purpose! If you just dive in and start hacking, I promise you won't be able to get all points.

The idea is to design a strategy first, with a careful plan of how you're going to work the problem. That was the point of the previous homework assignment.

You'll need helper functions. It is up to you to determine what those should be, and how (or if) you test them individually. I strongly recommend writing tests for your helper functions before you even implement the functions themselves. That way you think through exactly what they should do, what happens if they're given a NULL input, or a leaf node, or an overly-full node, or whatever else might be conceivable in the situation.

### Look at the unit tests
The unit tests that come along with the homework are meant to be read. There are little hints strewn throughout them. In particular, the print_tree function should be helpful. (Take a look at the [sanity check] unit test for help with that.)

### Some things to keep in mind:
insert and remove must handle cases where adding or removing keys will grow or shrink the tree. In some cases you'll have to merge nodes and re-allocate keys; other times you'll have to need to split nodes and similarly shuffle keys around. Consider having logic to test the various situations and handle them in their own functions, which are testable.

## Points
Like all the other assignments in this course, this one is worth 100 points. However, given the difficulty level, you can score up to 200 points (so yay extra credit).

Getting a 100/100 is super respectable. Getting a 200/100 is possible, but to be totally honest not many students get there.