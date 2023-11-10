#pragma once
#include "history/SpadesCommand.h"
#include "history/SpadesCommandOrder.h"
#include <memory>
#include <vector>
#include <string>

namespace spd
{
    class SpadesHistory
    {
        std::vector<std::unique_ptr<SpadesCommand>> undoCommands, redoCommands;
        static constexpr unsigned int placeCmdValue = 0;
        static constexpr unsigned int bidCmdValue = placeCmdValue + 1;
        static constexpr unsigned int bidOptCmdValue = bidCmdValue + 1;
        static constexpr int separationValue = -1;
    
        std::string serializeCommand(const SpadesCommand& command) const;
        int getCommandID(const SpadesCommand& command) const;
        std::vector<int> getUndoEncoding(const std::vector<int>& encoding) const;
        std::vector<int> getRedoEncoding(const std::vector<int>& encoding) const;
        std::vector<int> getEncoding(const std::string& data) const;
        std::vector<std::unique_ptr<SpadesCommand>> deserializeCommands(const std::vector<int> encoding) const;
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