#include "astar.h"
#include "utils.h"

#include <vector>

namespace astar {

SearchNode::SearchNode(CostType g, CostType h, const JointState& state, int stepNum, SearchNode* parent)
{
    _g = g;
    _h = h;
    _f = _g + _h;
    _state = state;
    _stepNum = stepNum;
    _parent = parent;
}

CostType SearchNode::g() const
{
    return _g;
}
CostType SearchNode::h() const
{
    return _h;
}
CostType SearchNode::f() const
{
    return _f;
}
int SearchNode::stepNum() const
{
    return _stepNum;
}
const JointState& SearchNode::state() const
{
    return _state;
}
SearchNode* SearchNode::parent()
{
    return _parent;
}

bool SearchNode::operator<(const SearchNode& sn)
{
    return f() == sn.f() ? -g() < -sn.g() : f() < sn.f();
}

bool CmpByState::operator()(SearchNode* a, SearchNode* b) const
{
    return a->state() < b->state();
}
bool CmpByPriority::operator()(SearchNode* a, SearchNode* b) const
{
    return *a < *b;
}


SearchTree::SearchTree() {}
SearchTree::~SearchTree()
{
    while (!_open.empty())
    {
        SearchNode* node = *_open.begin();
        _open.erase(_open.begin());
        delete node;
    }

    while (!_closed.empty())
    {
        SearchNode* node = *_closed.begin();
        _closed.erase(_closed.begin());
        delete node;
    }
}

void SearchTree::addToOpen(SearchNode* node)
{
    startProfiling();
    _open.insert(node);
    stopProfiling();
}
void SearchTree::addToClosed(SearchNode* node)
{
    startProfiling();
    _closed.insert(node);
    stopProfiling();
}

SearchNode* SearchTree::extractBestNode()
{
    startProfiling();
    while (!_open.empty())
    {
        SearchNode* best = *_open.begin();
        _open.erase(_open.begin());
        if (wasExpanded(best))
        {
            // we must delete this node
            delete best;
        }
        else
        {
            stopProfiling();
            return best;
        }
    }
    stopProfiling();
    return nullptr;
}

size_t SearchTree::size() const
{
    return _open.size() + _closed.size();
}
size_t SearchTree::sizeOpen() const
{
    return _open.size();
}

bool SearchTree::wasExpanded(SearchNode* node) const
{
    startProfiling();
    bool res = _closed.count(node);
    stopProfiling();
    return res;
}

vector<SearchNode*> generateSuccessors(
    SearchNode* node,
    IAstarChecker& checker,
    const JointState& goal,
    CostType (*heuristicFunc)(const JointState& state1, const JointState& state2),
    double weight
)
{
    vector<SearchNode*> result;
    for (size_t i = 0; i < checker.getActions().size(); ++i)
    {
        JointState action = checker.getActions()[i];
        JointState newState = node->state() + action;
        if (!checker.isCorrect(node->state(), action))
        {
            continue;
        }
        result.push_back(
            new SearchNode(
                node->g() + checker.costAction(action),
                heuristicFunc(newState, goal) * weight,
                newState,
                i,
                node
            )
        );
    }

    return result;
}

Solution astar(
    const JointState& startPos, const JointState& goalPos,
    IAstarChecker& checker,
    CostType (*heuristicFunc)(const JointState& state1, const JointState& state2),
    double weight,
    double timeLimit
)
{
    Solution solution(checker.getActions(), checker.getZeroAction());
    clock_t clockTimeLimit = timeLimit * CLOCKS_PER_SEC;

    // start timer
    clock_t start = clock();

    // init search tree
    SearchTree tree;
    SearchNode* startNode = new astar::SearchNode(0, heuristicFunc(startPos, goalPos) * weight, startPos);
    tree.addToOpen(startNode);
    SearchNode* currentNode = tree.extractBestNode();

    while (currentNode != nullptr)
    {
        if (checker.isGoal(currentNode->state()))
        {
            solution.stats.pathVerdict = PATH_FOUND;
            break;
        }
        // give up if time limit is exhausted
        if (clock() - start > clockTimeLimit)
        {
            solution.stats.pathVerdict = PATH_NOT_FOUND;
            break;
        }
        // expand current node
        vector<astar::SearchNode*> successors = generateSuccessors(currentNode, checker, goalPos, heuristicFunc, weight);
        for (auto successor : successors)
        {
            tree.addToOpen(successor);
        }
        // retake node from tree
        tree.addToClosed(currentNode);
        currentNode = tree.extractBestNode();
        // count statistic
        solution.stats.maxTreeSize = std::max(solution.stats.maxTreeSize, tree.size());
        ++solution.stats.expansions;
    }

    // end timer
    clock_t end = clock();
    solution.stats.runtime = (double)(end - start) / CLOCKS_PER_SEC;

    if (currentNode == nullptr)
    {
        solution.stats.pathVerdict = PATH_NOT_EXISTS;
    }
    else if (solution.stats.pathVerdict == PATH_FOUND)
    {
        vector<size_t> steps;
        while (currentNode->parent() != nullptr)
        {
            // count stats
            solution.stats.pathCost += checker.costAction(checker.getActions()[currentNode->stepNum()]);
            //
            steps.push_back(currentNode->stepNum());
            currentNode = currentNode->parent();
        }

        solution.stats.pathPotentialCost = heuristicFunc(startPos, goalPos);

        // push steps
        for (int i = steps.size() - 1; i >= 0; --i)
        {
            solution.addStep(steps[i]);
        }
    }

    solution.searchTreeProfile = tree.getNamedProfileInfo();
    return solution;
}

} // namespace astar
