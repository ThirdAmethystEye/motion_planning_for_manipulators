#pragma once

#include "planner.h"
#include "logger.h"
#include "testset.h"

#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

struct Config
{
    double timeLimit;
    double w;
    int testNum;
    bool randomTests;
    std::string scenFilename;
    std::string statsFilename;
    std::string testsFilename;
};

struct ModelState
{
    int counter = 0;
    int partOfMove = 0;
    bool haveToPlan = false;
    int solved = 0;
    Solution solution;

    JointState currentState;
    JointState goal;
    JointState action;
};

class Interactor
{
public:
    Interactor(const std::string& modelFilename);
    ~Interactor();

    void setUp(Config config);

    void setManipulatorState(const JointState& state);
    void setGoalState(const JointState& state);
    // simulate action in currentState
    // at end stage aplies action to currentState
    // return next stage
    size_t simulateAction(JointState& currentState, const JointState& action, size_t stage);

    void step();

    void stepLoop(double duration);

    void show();

    bool shouldClose();

    void doMainLoop();

private:
    mjData* _data;
    mjModel* _model;

    mjvCamera _cam;
    mjvOption _opt;
    mjvScene _scn;
    mjrContext _con;
    GLFWwindow* _window;

    ManipulatorPlanner* _planner;
    Logger* _logger;
    TestSet* _testset;

    size_t _dof;

    bool _shouldClose = false;

    Config _config;
    ModelState _modelState;
};

