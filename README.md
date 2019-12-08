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
- **th1**: angle between the vertical and the first rod (expressed in *radians*);
- **th2**: angle between the vertical and the second rod (expressed in *radians*);

In the file, these parameters must be written as in the following example:
    > N=3
    > 0 x0=200 y0=200 l1=50 l2=50 m1=50 m2=20 th1=-90 th2=-90
    > 1 x0=1130 y0=200 l1=40 l2=40 m1=40 m2=10 th1=180 th2=180
    > 2 x0=640 y0=400 l1=50 l2=50 m1=60 m2=5 th1=45 th2=30

The following figure better explains the use of all the parameters described 
above (taken from https://www.myphysicslab.com/pendulum/double-pendulum-en.html):

<p align="center">
  <img src="https://github.com/Mirk95/DoublePendulum/blob/master/images/Double_Pendulum.png">
</p>

## Accelerations Computation
As reported on the [site] (https://www.myphysicslab.com/pendulum/double-pendulum-en.html), 
the two formulas used for accelerations computation are the following:


$$
\theta_1'' = \frac{-g(2m_1 + m_2) \sin(\theta_1) - m_2 g \sin(\theta_1 - 
2\theta_2) - 2\sin(\theta_1 - \theta_2)m_2(\theta_2'^2 l_2 + \theta_1'^2 
l_1 \cos(\theta_1 - \theta_2))}{l_1 (2m_1 + m_2 - m_2 \cos(2\theta_1 - 
2\theta_2))}
$$









