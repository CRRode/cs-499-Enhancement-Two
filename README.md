# cs-499-Enhancement-Two

The artifact used was the CS 330: Computational Graphics and Visualization Final Project. The idea for this project was to recreate a 2D sense into a 3D sense using OpenGL. The objects in this scene are made up of basic shapes such as cubes, cylinders, spheres, and planes. This project was created over a few weeks in the CS 330: Computational Graphics and Visualization course.
I selected this artifact because it was one of the more complex programs that I made during my time at SNHU, and I knew there were many enhancements that could be done to this project. This project shows skills and albitites in algorithms and data structures because There are many algorithms in the program that are creating the shapes. I improved this artifact by adding instancing algorithms. These algorithms are used to increase the program's performance by only sending the shape data to the GPU once and then rendering it multiple times. For example, before the enhancement, there were multiple of the same cylinders being rendered. Each cylinder was sending data to the GPU even though it was the same data. Now, the data is only sent once, and we get the same number of cylinders in the program with increased performance. This changes the time complexity (BigO), from O(n), where “n” is the number of cylinders being made, to O(1). Also, the code is more concise and easier to maintain since the rendering logic for all the cylinders is contained within a single loop. This reduces the repetitive code for each cylinder, making it easier to manage and less prone to errors. 
Additionally, I added an FPS counter to the program. This displays the current FPS in the window while the program is running. An FPS counter is an effective way to tell how well the program is performing. 


I met all the course outcomes that I planned to meet in module one. These are, 

employ strategies for building collaborative environments that enable diverse audiences to support organizational decision-making in computer science.
I met this outcome thought the comments, in code documentation of changes and a modular structure that any programmer can hop in and edit.

Design, develop, and deliver professional-quality oral, written, and visual communications that are coherent, technically sound, and appropriately adapted to specific audiences and contexts. 
I met this outcome through the code review where I talked about the code and the enhancement plan that was carried out. Also, this project uses DLLs, headers and other resources developed by others, which demonstrates my ability to work with peer-created code. 

Demonstrate an ability to use well-founded and innovative techniques, skills, and tools in computing practices for the purpose of implementing computer solutions that deliver value and accomplish industry-specific goals. 
I met this outcome by meeting the enhancement criteria that I set myself as well as the criteria for the entire project. I also used the best computer science practices when making the project and enhancement.

Design and evaluate computing solutions that solve a given problem using algorithmic principles and computer science practices and standards appropriate to its solution, while managing the trade-offs involved in design choices (data structures and algorithms)
I met this outcome by creating an instancing algorithm to render my shapes that demonstrate optimization and time complexity while increasing performance for the overall program and reducing the amount of redundant code. 

While enhancing this artifact I learned how important algorithms can be and how much they can change a program. Adding the algorithm increased the program's performance by reducing the amount of data sent to the GPU. The instancing algorithms also decreased the amount of code in the source file. There was a lot of code deleted after adding the algorithm because they were not needed any more. If I used instancing algorithms when I originally made the code, there would have been a lot of time saved. Getting the algorithm running was a little bit of a challenge. There were errors in the code when I first added it but after a little research, I figured out the problem and the code ran as intended. 
