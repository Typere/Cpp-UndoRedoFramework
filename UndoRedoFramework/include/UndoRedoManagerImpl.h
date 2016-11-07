#ifndef UNDOREDOMANAGERIMPL_H
#define UNDOREDOMANAGERIMPL_H

#include "UndoRedoManager.h"
#include "UndoRedoStack.h"
#include "Command.h"

#include <stack>
#include <memory>


class UndoRedoManagerImpl :  public UndoRedoManager
{
public:

	UndoRedoManagerImpl();
	UndoRedoManagerImpl(UndoRedoStack && urStack);
	UndoRedoManagerImpl(UndoRedoManagerImpl && rhs);

	//for non copyable commands
	//for copyable commands
	//for SmartPointers
	virtual void doIt(Command && command) override;
	virtual void doIt(Command const& command) override;

	virtual void doIt(SmartPointer && command) override;

	virtual bool isUndoable() const override;
	virtual void undo() override;

	virtual bool isRedoable() const override;
	virtual void redo() override;
	
	virtual bool isModified() const override;
	virtual void resetModified() override;
	virtual std::size_t undoStackSize() const override;
	virtual void clear() override;
	virtual std::unique_ptr<UndoRedoStack> clone() && override;
private:
	std::unique_ptr<UndoRedoStack> urStack;
	int modifications;
};

#endif //UNDOREDOMANAGERIMPL_H
