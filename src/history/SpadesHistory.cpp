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

bool SpadesHistory::deserialize(const std::string &encodedData)
{
    clear();
    std::vector<int> encoding = getEncoding(encodedData);
    for(int i = 0; i < encoding.size(); i++){
        
    }
    return encodedData.empty() || !encoding.empty();
}

std::vector<int> SpadesHistory::getEncoding(const std::string &encodedData) const
{
    std::vector<int> encoding;
    std::istringstream iss(encodedData);
    int value;
    while (iss >> value)
    {
        encoding.push_back(value);
        if (iss.peek() == ' ')
        {
            iss.ignore();
        }
        if (iss.fail())
        {
            encoding.clear();
            break;
        }
    }
    return encoding;
}

std::string SpadesHistory::serialize() const
{
    std::stringstream ss;
    for (const auto &command : undoCommands)
    {
        ss << undoCmdValue << " " << serializeCommand(*command);
    }
    for (const auto &command : redoCommands)
    {
        ss << redoCmdValue << " " << serializeCommand(*command);
    }
    return ss.str();
}

std::string SpadesHistory::serializeCommand(const SpadesCommand &command) const
{
    std::stringstream ss;
    ss << getCommandID(command) << " ";
    for (const auto &value : command.serialize())
    {
        ss << value << " ";
    }
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