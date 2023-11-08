#include "history/SpadesHistory.h"
#include <sstream>

using namespace spd;

void SpadesHistory::clear()
{
    undoCommands.clear();
    redoCommands.clear();
}

bool SpadesHistory::canUndo() const
{
    return !undoCommands.empty();
}
void SpadesHistory::undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    std::unique_ptr<SpadesCommand> command = std::move(undoCommands.back());
    undoCommands.pop_back();
    command->undo(state, turn, trumpVariationController);
    redoCommands.push_back(std::move(command));
}

void SpadesHistory::redo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController)
{
    std::unique_ptr<SpadesCommand> command = std::move(redoCommands.back());
    redoCommands.pop_back();
    command->execute(state, turn, trumpVariationController);
    undoCommands.push_back(std::move(command));
}

bool SpadesHistory::canRedo() const
{
    return !redoCommands.empty();
}

void SpadesHistory::addCommand(std::unique_ptr<SpadesCommand> command)
{
    undoCommands.push_back(std::move(command));
    redoCommands.clear();
}

bool SpadesHistory::deserialize(const std::string &data)
{
    clear();
    const auto encoding = getEncoding(data);
    const auto undoEncoding = getUndoEncoding(encoding);
    const auto redoEncoding = getRedoEncoding(encoding);
    undoCommands = deserializeCommands(undoEncoding);
    redoCommands = deserializeCommands(redoEncoding);
    return undoEncoding.size() / 2 == undoCommands.size() && redoEncoding.size() / 2 == redoCommands.size();
}

std::vector<std::unique_ptr<SpadesCommand>> SpadesHistory::deserializeCommands(const std::vector<int> encoding) const
{
    std::vector<std::unique_ptr<SpadesCommand>> commands;
    for (int i = 0; i+1 < encoding.size(); i += 2)
    {
        int cmdValue = encoding[i];
        int serializedValue = encoding[i + 1];
        if (cmdValue == placeCmdValue)
        {
            commands.push_back(std::make_unique<PlaceCommand>(serializedValue));
        }
        else if (cmdValue == bidCmdValue)
        {
            commands.push_back(std::make_unique<BidCommand>(serializedValue));
        }
        else if (cmdValue == bidOptCmdValue)
        {
            commands.push_back(std::make_unique<BidOptionCommand>(serializedValue));
        }
        else
        {
            std::cerr << "Error: wrong command deserialization value " << i << " " << cmdValue << " " << serializedValue << "\n";
        }
    }
    return commands;
}

std::vector<int> SpadesHistory::getEncoding(const std::string &data) const
{
    std::vector<int> encoding;
    std::istringstream iss(data);
    int value;
    while (iss >> value)
    {
        encoding.push_back(value);
    }
    return encoding;
}

std::vector<int> SpadesHistory::getUndoEncoding(const std::vector<int> &encoding) const
{
    std::vector<int> undoEncoding;
    for (int i = 0; i < encoding.size() && encoding[i] != separationValue; i++)
    {
        undoEncoding.push_back(encoding[i]);
    }
    return undoEncoding;
}
std::vector<int> SpadesHistory::getRedoEncoding(const std::vector<int> &encoding) const
{
    std::vector<int> redoEncoding;
    const int startIndex = getUndoEncoding(encoding).size() + 1;
    for (int i = startIndex; i < encoding.size(); i++)
    {
        redoEncoding.push_back(encoding[i]);
    }
    return redoEncoding;
}

std::string SpadesHistory::serialize() const
{
    std::stringstream ss;
    for (const auto &command : undoCommands)
    {
        ss << " " << serializeCommand(*command);
    }
    ss << " " << separationValue;
    for (const auto &command : redoCommands)
    {
        ss << " " << serializeCommand(*command);
    }
    return ss.str();
}

std::string SpadesHistory::serializeCommand(const SpadesCommand &command) const
{
    std::stringstream ss;
    ss << getCommandID(command) << " " << command.serialize();
    return ss.str();
}

int SpadesHistory::getCommandID(const SpadesCommand &command) const
{
    if (dynamic_cast<const PlaceCommand *>(&command) != nullptr)
    {
        return placeCmdValue;
    }
    else if (dynamic_cast<const BidCommand *>(&command) != nullptr)
    {
        return bidCmdValue;
    }
    else if (dynamic_cast<const BidOptionCommand *>(&command) != nullptr)
    {
        return bidOptCmdValue;
    }
    return -1;
}