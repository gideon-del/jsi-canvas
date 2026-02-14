#include "CommandHistory.h"

namespace CanvasMVP
{
    void CommandHistory::execute(std::unique_ptr<Command> command)
    {
        if (!command)
        {
            logger.info("Command not found");
            return;
        }

        if (currentIndex_ < (int)history_.size() - 1)
        {
            history_.erase(history_.begin() + currentIndex_ + 1, history_.end());
        }

        if (currentIndex_ >= 0 && history_[currentIndex_]->canMergeWith(command.get()))
        {

            history_[currentIndex_]->mergeWith(command.get());
            logger.info("Command merged");
            history_[currentIndex_]->execute(sceneGraph_);
            return;
        }
        command->execute(sceneGraph_);

        history_.push_back(std::move(command));
        currentIndex_++;
        logger.info("Command History updated");

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
            logger.info("Cannot undo - no commands");
            return false;
        }

        history_[currentIndex_]->undo(sceneGraph_);

        currentIndex_--;
        logger.info("Undo Successful");
        return true;
    }
    bool CommandHistory::redo()
    {
        if (!canRedo())
        {
            logger.info("Cannot redo");
            return false;
        }
        currentIndex_++;
        history_[currentIndex_]->execute(sceneGraph_);
        logger.info("Redo Successful");
        return true;
    }

    void CommandHistory::clear()
    {
        history_.clear();
        currentIndex_ = -1;
        logger.info("Cleared");
    }

}