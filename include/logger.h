#pragma once

#include "solution.h"

class Logger
{
public:
    Logger(size_t dof);
    ~Logger();

    void prepareCspaceFile(const std::string& filename);
    // empty filename means stdout
    void prepareMainFile(const std::string& filename);
    void prepareRuntimeFile(const std::string& filename);
    void prepareScenFile(const std::string& filename);
    void prepareStatsFile(const std::string& filename);
    void prepareMpathFile(const std::string& filename);

    void printMainLog(const Solution& solution);
    void printRuntimeLog(const Solution& solution);
    void printStatsLog(const Solution& solution);
    void printScenLog(const Solution& solution, const JointState& startPos, const JointState& goalPos);
    void printScenLog(const Solution& solution, const JointState& startPos, double goalX, double goalY);
    void printCSpace(const vector<string>& cSpace);
    void printMPath(const vector<string>& mPath);

private:
    void printMainLog(FILE* file, const Solution& solution);

    void printRuntimeLogHeader(FILE* file, const Solution& solution);
    void printRuntimeLog(FILE* file, const Solution& solution);

    void printStatsLogHeader(FILE* file);
    void printStatsLog(FILE* file, const Solution& solution);

    void printScenLogHeader(FILE* file, size_t dof);
    void printScenLog(FILE* file, const Solution& solution, const JointState& startPos, const JointState& goalPos);
    void printScenLog(FILE* file, const Solution& solution, const JointState& startPos, double goalX, double goalY);

    void printCspace(FILE* file, const vector<string>& cSpace);
    void printMpath(FILE* file, const vector<string>& mPath);

    FILE* _cspaceFile = nullptr;
    FILE* _mpathFile = nullptr;
    FILE* _mainFile = nullptr;
    FILE* _runtimeFile = nullptr;
    FILE* _scenFile = nullptr;
    FILE* _statsFile = nullptr;

    bool _runtimeHaveHeader = false;

    size_t _dof;
};
