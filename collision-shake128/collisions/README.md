# Collisions in weakened SHAKE128

Pierre-Elisée Flory

## The algorithms

### Bruteforce

Up until outputs of SHAKE of size 5 bytes, I used a simple brute force algorithm to produce collisions. This enabled to produce instantly thousands of collisions. Two variants have been used : one that try a combination of my mail adress with some ASCII characters (even non printables ones). After that I used characters that could be easily mistaken, that is space *' '*, *\n* and *\t*. These last collisions are the ones mainly used in the examples provided.

### Pollard's Rho

This method was implemented to find some collisions of size 5 and 6 bytes. It didn't seem to be much more efficient than the bruteforce method. It produced completely random collision. The files *collision-6/0.A* and *collision-6/0.B* are examples of outputs from this algorithm.

### Parrallel collision seearch (van Oorschot method)

I finally implemented a parrallelized version of the previous algorithm according to the paper from Van Oorschot. This was performed with MPI on a server with 64 cores. The algorithm work with a master process that keep track of the distinguishable points sent to him by slave processes. The master then dispatch to process the duty to preform the trail walk when a collision is detected. Rigth now, the processes that are beeing tasked to find the collision between two trails stop at when they yield a result, which limits the number of possible collisions found with this method.

It enabled to yields in seconds collisions in SHAKE128 with output 6 and 7 bytes. Once again I ensured that the collisions included my mail adress to authenticate them. However the algorithm takes tens of minutes to yield a result for 8 bytes and wouldnt yield any result for 9 bytes output after a couple of hours. Since I didn't watch properly the memory consumption due to the aggregation of the distinguished points, it would probably not perform any better after a longer computation time in the current state.

## Results

The collisions presented here are only a fraction of those found (up until output sizes below 8 bytes) because I did not automate the formating into the expected files of the assignment. Thus the process is long and I only presented some representative values.
