#pragma once
#include "history/SpadesCommand.h"
#include <memory>
#include <vector>
#include <string>

namespace spd
{
    class SpadesHistory
    {
        std::vector<std::unique_ptr<SpadesCommand>> undoCommands, redoCommands;
        const unsigned int placeCmdValue = 0;
        const unsigned int bidCmdValue = placeCmdValue + 1;
        const unsigned int bidOptCmdValue = bidCmdValue + 1;
        const unsigned int undoCmdValue = 0;
        const unsigned int redoCmdValue = 1;

        std::string serializeCommand(const SpadesCommand& command) const;
        int getCommandID(const SpadesCommand& command) const;
        std::vector<int> getEncoding(const std::string& encodedData) const;

    public:
        void clear();
        bool canUndo() const;
        bool canRedo() const;
        void undo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController);
        void redo(State &state, const Turn &turn, const TrumpVariationController &trumpVariationController);
        void addCommand(std::unique_ptr<SpadesCommand> command);

        bool deserialize(const std::string &encodedData);
        std::string serialize() const;
    };
}