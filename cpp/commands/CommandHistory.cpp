#include "CommandHistory.h"

namespace CanvasMVP
{
    void CommandHistory::execute(std::unique_ptr<Command> command)
    {
        if (!command)
        {
            return;
        }

        if (currentIndex_ < (int)history_.size() - 1)
        {
            history_.erase(history_.begin() + currentIndex_ + 1, history_.end());
        }

        if (currentIndex_ >= 0 && history_[currentIndex_]->canMergeWith(command.get()))
        {
            history_[currentIndex_]->mergeWith(command.get());
            history_[currentIndex_]->execute(sceneGraph_);
            return;
        }
        command->execute(sceneGraph_);

        history_.push_back(std::move(command));
        currentIndex_++;

        if (history_.size() > maxHistorySize)
        {
            history_.erase(history_.begin());
            currentIndex_--;
        }
    }

    bool CommandHistory::undo()
    {
        if (!canUndo())
        {
            return false;
        }

        history_[currentIndex_]->undo(sceneGraph_);

        currentIndex_--;
        return true;
    }
    bool CommandHistory::redo()
    {
        if (!canRedo())
        {
            return false;
        }
        currentIndex_++;
        history_[currentIndex_]->execute(sceneGraph_);

        return true;
    }

    void CommandHistory::clear()
    {
        history_.clear();
        currentIndex_ = -1;
    }

}