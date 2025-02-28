This project uses [mujoco](https://github.com/deepmind/mujoco) as simulator and library.

Content
1. [Installation](#installation)
1. [Before run](#before-run)
1. [Run](#run)
1. [Run with custom parameters](#run-with-custom-parameters)
1. [Project description](#project-description)
    1. [Problem description](#problem-description)
    1. [Planner](#planner)
        1. [General description](#general-description)
        1. [A* algorithm](#a-algorithm)
        1. [Collision checking](#collision-checking)
    1. [Task generation](#task-generation)
    1. [Model](#model)
    1. [Interaction with mujoco](#interaction-with-mujoco)

# Installation
This instruction is written for linux. Before start make sure that you have g++ compiler installed on your machine.  
To compile and run this project you need to install OpenGL (glfw) library

```
sudo apt update
sudo apt install libglfw3-dev
```

Then copy-paste two files from 'lib' folder libmujoco.so and libmujoco.so.2.3.2 to '/usr/lib' to install mujoco libraries

```
sudo cp lib/libmujoco.so /usr/lib/libmujoco.so
sudo cp lib/libmujoco.so.2.3.2 /usr/lib/libmujoco.so.2.3.2
```
Congradulations! You have completed installation.  

# Before run

This project uses some output files witch don't exist in github repository. To make all necessary folders run
```
./github/build_structure
```
It will create empty folders in witch you can find output files after run: with logs and statistics.

# Run
For run on linux you can use 'run_linux' file. Execute this in the root of the repository.  
```
./run_linux
```

Or you can compile this using Makefile
```
make simulator
```

After this operation in the root of this repository you can find 'manipulator' file. Run it. 
```
./simulator
```

If you got into trouble with running some of this files, do this and try again:
```
chmod +x <execution_file>
```

# Run with custom parameters

To run this project with your custom parametrs you need to change content in `main.cpp` file. All simulation realized in object Interactor.

- To choose scene (manipulator and obstacles) use [constructor of Interactor](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L22). Write the path of file from the root of repository. In `models` folder you can find several scenes or create your xml-file of model.
- Another options you can choose in method setUp(). You can set the maximum time that planner may spend on one task. If algorithm is not found solution during this time, it returns PATH NOT FOUND verdict. Use [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L24) to set time bound.
- You can set weight of heuristic using [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L25).
- You can set the number of generated random tasks if you want use random generated tasks. But planner will not solve incorrect tasks if there are any (for example, the start position intersects obstacle). Generating only correct tasks is planned for this project. Use [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L26) to set this parameter.
- You can choose kind of task at [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L27). Now available TASK_STATE and TASK_POSITION. In task state goal is full configuration, in task position only coordinates of end-effector.
- You can turn on random task generation at [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L28). Set true if want random taskset.
- At [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L29) output file for data about tasks: start configuration, goal, difficult (not really good), cost of path, runtime in csv format. You can use this file and script `tools/clusterizer_*.py` to convert this data to input format of taskset. Using this output you can store random generated taskset.
- At [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L30) output file for data about solutions of tasks: the number of expansions, runtime, cost of path etc in csv format.
- At [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L31) you can set taskset if flag of random generation is turned off in special format. You can generate it from csv using scripts in `tools`. Script depends of kind of task. And you have to set kind of task which mathes the kind of tasks presented in taskset.
- At [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L32) output file for profiling data for general functions of algorithm in csv format.
- At [this line](https://github.com/machine-solution/motion_planning_for_manipulators/blob/356d2f567f8efbd18be9b16109bd777bfc7c4f25/src/main.cpp#L33) you can choose show found solution by actions in graphical window or not. Set true to show.


# Project description

This chapter may no longer be relevant and soon I add pdf-file with description of problem and general concept of project.

## Problem description
We have a manipulator in space with obstacles and two positions of this manipulator: start and finish. It is required to plan the actions using which the manipulator will come from the start position to the finish.

## Planner

### General description
The most important class in this project is [ManipulatorPlanner](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/include/planner.h#L16). It solves problem in method [planSteps](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/include/planner.h#L30). This method must return solution with statistics and sequence of actions needed to reach finish from start.

### A* algorithm
A* algorithm is realized in two places: [node and tree](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/astar.cpp#L8) and [algorithm](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/planner.cpp#L183) in planner.\
It is planned to move algorithm to astar.cpp.

### Collision checking
For collision checking I use copy of original model on scene. Planner [gets this copy](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/main.cpp#L345) and uses it in [checkCollisionAction](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/planner.cpp#L36) and [checkCollision](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/planner.cpp#L22) methods.\
For speed I use [light_collision](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/light_mujoco.cpp#L96) function instead mujoco standard 'mj_step_1'. Code of this function was copied from mujoco source files and refactored to more light function. But it has one constraint: it works only for predefined pairs of geoms. It means that you have to define in model file witch pair of geoms we need to check on collision. It makes some of discomfort, but gains about 20% speeding up.

## Task generation
Now task generation is united with interactor and use only one function - [randomState](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/main.cpp#L215). This process very simple: manipulator strts with zero state and gets random finish. If it finds solution, it will get new random finish and start will be previous finish. If no, it gets new random finish without changing start.\
I'm planning to separate interaction and test generation in future.

## Model
For this project we can do manipulator model in xml format. To learn how to do this please read [xml reference](https://mujoco.readthedocs.io/en/latest/XMLreference.html).\
All models are located in 'model' directory.\
Now the easiest way to change model is execute   ```./simulator <filename>```   where filename is path to your model from root of repository.\
Another way to do this is change default filename [here](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/main.cpp#L14).

## Interaction with mujoco
All interaction planner and mujoco simulator in [planner_step](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/src/main.cpp#L197) function, which is being called in infinity loop of simulation.\
To simulate actions of manipulator I divided angles from 0 to pi on [worldUnits](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/include/global_defs.h#L8) - minimal angle to move.\
Planner can divide angles on another [units](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/include/global_defs.h#L5) and when interaction function gets next step, it move manipulator in step direction, but every interactor step move manipulator on one worlUnit and do this [unitSize](https://github.com/machine-solution/motion_planning_for_manipulators/blob/261f3460d69ccef7a86ff90b380b45a91f1aa76f/include/global_defs.h#L7) times.
