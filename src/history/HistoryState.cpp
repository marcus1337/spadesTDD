#include "spades/history/HistoryState.h"
#include "spades/data/memento/json.h"

using namespace spd;

void HistoryState::clear()
{
    leftMoves = {};
    rightMoves = {};
}
void HistoryState::add(const Move &item)
{
    leftMoves.push(item);
    rightMoves = {};
}
const Move &HistoryState::peek() const
{
    return leftMoves.top();
}
int HistoryState::getLeftSize() const
{
    return leftMoves.size();
}
int HistoryState::getRightSize() const
{
    return rightMoves.size();
}
void HistoryState::shiftRight()
{
    leftMoves.push(rightMoves.top());
    rightMoves.pop();
}
void HistoryState::shiftLeft()
{
    rightMoves.push(leftMoves.top());
    leftMoves.pop();
}

std::vector<Move> HistoryState::getLeftMoves() const
{
    std::vector<Move> moves;
    std::stack<Move> tempStack = leftMoves;
    while (!tempStack.empty())
    {
        moves.push_back(tempStack.top());
        tempStack.pop();
    }
    std::reverse(moves.begin(), moves.end());
    return moves;
}

using json = nlohmann::json;

std::string HistoryState::serialize() const
{
    auto leftMovesCopy = leftMoves;
    auto rightMovesCopy = rightMoves;
    json leftMovesJson;
    json rightMovesJson;
    while (!leftMovesCopy.empty())
    {
        leftMovesJson.push_back(leftMovesCopy.top().serialize());
        leftMovesCopy.pop();
    }
    while (!rightMovesCopy.empty())
    {
        rightMovesJson.push_back(rightMovesCopy.top().serialize());
        rightMovesCopy.pop();
    }

    json encoding;
    encoding["leftMoves"] = leftMovesJson;
    encoding["rightMoves"] = rightMovesJson;
    return encoding.dump();
}
bool HistoryState::deserialize(const std::string &encoding)
{
    const auto &serializedState = json::parse(encoding);
    for (const auto &moveJson : serializedState["leftMoves"])
    {
        leftMoves.push(Move(moveJson.get<std::string>()));
    }
    for (const auto &moveJson : serializedState["rightMoves"])
    {
        rightMoves.push(Move(moveJson.get<std::string>()));
    }
    return true;
}