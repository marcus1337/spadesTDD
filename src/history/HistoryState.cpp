#include "spades/history/HistoryState.h"
#include "spades/data/memento/json.h"

using namespace spd;

void HistoryState::clear()
{
    leftMoves.clear();
    rightMoves.clear();
}
void HistoryState::add(const Move &item)
{
    leftMoves.push_back(item);
    rightMoves.clear();
}
const Move &HistoryState::peek() const
{
    return leftMoves.back();
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
    leftMoves.emplace_back(rightMoves.front());
    rightMoves.pop_front();
}
void HistoryState::shiftLeft()
{
    rightMoves.emplace_front(leftMoves.back());
    leftMoves.pop_back();
}

std::vector<Move> HistoryState::getLeftMoves() const
{
    return std::vector<Move>(leftMoves.begin(), leftMoves.end());
}

using json = nlohmann::json;

std::string HistoryState::serialize() const
{
    json leftMovesJson;
    json rightMovesJson;
    for (const auto &move : leftMoves)
    {
        leftMovesJson.push_back(move.serialize());
    }
    for (const auto &move : rightMoves)
    {
        rightMovesJson.push_back(move.serialize());
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
        leftMoves.push_back(Move(moveJson.get<std::string>()));
    }
    for (const auto &moveJson : serializedState["rightMoves"])
    {
        rightMoves.push_back(Move(moveJson.get<std::string>()));
    }
    return true;
}