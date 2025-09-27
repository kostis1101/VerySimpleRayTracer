This is a very simple CPU raytracer. I've been wanting to write one for a while, so I figured I should give it a shot.
Despite by dislike towards C++, it is much more helpfull over C in this project, especially when it comes to vector maths.

The only external library I am using is opencv to display, encode and save the images to a file. Everything else is written by me from scratch.
My main reference is the book "Physically Based Rendering: From Theory To Implementation". Allthough I don't follow it's implementation, I learn about some of the algorithms and some of the optimization technics from there.

So far the renderer supports the following:
- Single ray - triangle collision detection using a simple Bounding Volumn Hierarchy acceleration structure.
- Reading of simple Wavefront .obj files
- Simple shading (basically Lambertian diffuse)

I purposefully didn't work on any reflection models, since I'd like to implement a more physically based model of light transport.

On a single CPU core of an intel i7 processor, it renders the following 1000 x 1000 image of the Utah teapot in about 5 seconds:

<img width="1000" height="1000" alt="result" src="https://github.com/user-attachments/assets/33205b1a-4f93-4699-aeb7-08f5d560ca03" />
