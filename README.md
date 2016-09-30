# Flocking

A few problems in our flocking program: 
1. We made a design decision mistake about using 4d vectors rather than 3d vectors. Although 4d vector is a kind of technique we learned in class, this is unnecessary in this homework.
2. The number of square on each side of the checkerboard must be an odd number. This is an implementation problem, but we don't want to spend more time fixing it now.
3. Our bird can fly through and go under the checkerboard, or go out of the checkerboard. If we have more time we can refine this.

Extra credit we did:
1. Flapping the wings. (The flapping looks kind of slow, but you can single step and observe it.)
2. Multiple flocks. (Now the code can only draw two flocks.)
3. Shadows. (The shadows are the vertical projection of the boids on the ground - if the boids are under the checkerboard, the shadows will be projected on the ground.)
