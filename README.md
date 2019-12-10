# Double Pendulums
Project for *Real-Time Embedded Systems* course - *Department of Engineering 
Enzo Ferrari* (UNIMORE)

## Project Statement
Project nr. 7: *Double Pendulums*. Simulate N double pendulums, where N and 
their initial conditions are read from a text file. Trajectory must be 
visualized as in the following examples: <br/>
https://www.youtube.com/watch?v=V4hvENrtMeE <br/>
https://www.youtube.com/watch?v=QXf95_EKS6E <br/>
Developed by Mirko Cavecchia

## Libraries
The project is based on `Allegro 4` graphic library, available at this link
http://liballeg.org/download.html, and on `Ptask` library 
(Periodic Real-Time Task interface to pthreads), available at this link 
https://github.com/glipari/ptask.

## Project Explanation
The purpose of this application is to show the chaotic movement of **N** double 
pendulums, where the number of pendulums and all the initial parameters 
are read from a text file called `"params.txt"`.
In the file, the initial parameters required for creating each double pendulum 
are:
- **id**: pendulum identifier (from 0 to N-1);
- **x0**: x variable of initial point;
- **y0**: y variable of initial point;
- **l1**: length of the first rod;
- **l2**: length of the secon rod;
- **m1**: mass of the first pendulum;
- **m2**: mass of the second pendulum;
- **th1**: angle between the vertical and the first rod (expressed in degrees);
- **th2**: angle between the vertical and the second rod (expressed in degrees);

In the file, these parameters must be written as in the following example:
> N=3 </br>
> 0 x0=200 y0=200 l1=50 l2=50 m1=50 m2=20 th1=-90 th2=-90 </br>
> 1 x0=1130 y0=200 l1=40 l2=40 m1=40 m2=10 th1=180 th2=180 </br>
> 2 x0=640 y0=400 l1=50 l2=50 m1=60 m2=5 th1=45 th2=30 </br>

You are free to remove or add new pendulums and to modify these parameters in 
the `params.txt` file at any time, keeping mind to respect the syntax of the 
file, as shown above.

The following figure (taken from [Wired](https://www.wired.com/story/fidget-spinners-the-serious-physics-behind-a-double-pendulum-spinner/)) 
better explains the use of all the parameters described above :

<p align="center">
  <img src="https://github.com/Mirk95/DoublePendulum/blob/master/images/Double_Pendulum.jpg">
</p>

## Accelerations Computation
As reported on [MyPhysicsLab](https://www.myphysicslab.com/pendulum/double-pendulum-en.html), 
the two formulas used for accelerations computation are the following:

<p align="center">
  <img src="https://github.com/Mirk95/DoublePendulum/blob/master/images/Formulas.png">
</p>

## Programming Choices
During the development of the project, the following programming choices were 
taken:
- To make the trajectory displayed by double pendulums more presentable, the 
gravitational constant has been replaced with a time scale factor `TSCALE`, 
equals to 0.1.
- To avoid having a gui too full of pendulums, the maximum number of pendulums 
`MAX_P` has been set to 7. In this way, pendulums will have all the colors 
described in the standard VGA.
- The maximum number of previous points shown in the trajectory `TLEN` has been 
set to 40.

## Tasks
- **N** double pendulum tasks: each task has to compute the accelerations of 
the two masses and to update the velocities and the angles of double pendulums 
in a shared memory each `PER_P` milliseconds.
- **1** graphic task: it has to read the shared memory and to update the screen 
each `PER_G` milliseconds.

## Application Structure
<p align="center">
  <img src="https://github.com/Mirk95/DoublePendulum/blob/master/images/Application_Structure.png">
</p>

## Compile and Run
To run the application, execute the following commands:
- `git clone https://github.com/Mirk95/DoublePendulum.git`: to clone project;
- `make all`: to remove all compiled files in build directory and build;
- `make compile`: to compile all C files and put compiled files in build;
- `make run`: to run the application;

Press ESC to end the program.





