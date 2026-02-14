#pragma once
#include "Command.h"

namespace CanvasMVP
{
    class CommandHistory
    {
    private:
        int currentIndex_ = -1;
        std::vector<std::unique_ptr<Command>> history_;
        SceneGraph &sceneGraph_;
        size_t maxHistorySize = 100;

    public:
        CommandHistory(SceneGraph &sceneGraph, size_t maxHistorySize) : sceneGraph_(sceneGraph), maxHistorySize(maxHistorySize) {};
        void execute(std::unique_ptr<Command> command);
        bool undo();
        bool redo();

        bool canUndo() const { return currentIndex_ >= 0; }
        bool canRedo() const { return currentIndex_ < (int)history_.size(); };
        int currentIndex() const { return currentIndex_; };
        size_t historySize() const { return history_.size(); };
        void clear();
    };
}