# Double Pendulums
Project for **Real-Time Embedded Systems** course - Department of Engineering 
Enzo Ferrari (UNIMORE)

## Project Statement
Project nr. 7: **Double Pendulums**. Simulate N double pendulums, where N and 
their initial conditions are read from a text file. Trajectory must be 
visualized as in the following examples: <br />
https://www.youtube.com/watch?v=V4hvENrtMeE <br />
https://www.youtube.com/watch?v=QXf95_EKS6E <br />
Developed by Mirko Cavecchia

## Libraries
The project is based on `Allegro 4` graphic library (available at this link
http://liballeg.org/download.html), and on `Ptask` library, 
Periodic Real-Time Task interface to pthreads, (available at this link 
https://github.com/glipari/ptask).

## Project Explanation
The purpose of this application is to show the chaotic movement of **N** double 
pendulums, where the number of pendulums and all the initial parameters 
are read from a text file called `"params.txt"`.
In the file, the initial parameters required for creating the double pendulum 
are:
- **id**: pendulum identifier (from 0 to N-1);
- **x0**: x variable of initial point;
- **y0**: y variable of initial point;
- **l1**: length of the first rod;
- **l2**: length of the secon rod;
- **m1**: mass of the first pendulum;
- **m2**: mass of the second pendulum;
- **th1**: angle between the vertical and the first rod;
- **th2**: angle between the vertical and the second rod;

![alt text](https://github.com/Mirk95/DoublePendulum/blob/master/images/
Double_Pendulum.png)
