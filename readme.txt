Design Choices:

OpenGL -> I wanted to make an engine that had an eventgraph, with a simple inspector and a scene window.

To accomplish this, I will be using ImGUI, but when making the Window a Dockingspation for the Imgui Windows, the screen turned gray. To fix this I needed to do a FrameBuffer.
Projecting the Window on the Imgui Window, when looking up about this, most said/did this with oipen GL, I tried to do this with the SDL imagaes, but I didnt manage to find how.  To be frank I didnt reallty try very hard since 
I did feel the limitations of  SDL the previous years. So I decide to go with Open GL and see where it goes.s


Resourcemanager -> loads and manages shared resources 



Observer -> health, Score, Collsision, If bomb gets collided with explosion, on bomb explode