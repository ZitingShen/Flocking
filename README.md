# Flocking

A few problems in our flocking program: 
1. We made a design decision mistake about using 4d vectors rather than 3d vectors. Although 4d vector is a kind of technique we learned in class, this is unnecessary in this homework.
2. The number of square on each side of the checkerboard must be an odd number. This is an implementation problem, but we don't want to spend more time fixing it now.
3. Our bird can fly through and go under the checkerboard, or go out of the checkerboard. If we have more time we can refine this.
4. Our shades are not on the xy-plane, but 50 higher than the plane. For some unknown reason, the shades flicker a lot when we put them on the xy-plane.
5. We try to implement obstacles, but it does not work properly.

Extra credit we did:
1. Flapping the wings. (The flapping looks kind of slow, but you can single step and observe it.)
2. Multiple flocks. (Now the code can only draw two flocks.)
3. Shadows. (The shadows are the vertical projection of the boids on the ground - if the boids are under the checkerboard, the shadows will be projected on the ground.)
4. Obstacles. (This is not working correctly.)

Key control:
'=': add a boid
'-': delete a boid
'v': default view
'g': side view
't': trailing view
'w': move goal along +y direction
's': move goal along -y direction
'd': move goal along +x direction
'a': move goal along -x direction
'<-': decrease the goal speed
'->': increase the goal speed
'p': pause & single step
'r': resume
'q'/'esc': quit