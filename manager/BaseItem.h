/*
  ==============================================================================

    BaseItem.h
    Created: 28 Oct 2016 8:04:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEITEM_H_INCLUDED
#define BASEITEM_H_INCLUDED


class ScriptManager;
class GenericControllableManager;

class BaseItem : 
	public ControllableContainer
{
public :
	BaseItem(const String &name = "", bool canBeDisabled = true, bool canHaveScript = false, bool canHaveCustomParameters = false);
	virtual ~BaseItem();

	BoolParameter * enabled;
	StringParameter * nameParam; 

	//UI - should move outside data class ? how to save/load if not there 
	BoolParameter * miniMode;
	FloatParameter * listUISize; //width or height in a list
	Point2DParameter * viewUIPosition; //position in a vieww
	Point2DParameter * viewUISize; //size in a view

	bool canBeDisabled;
	bool canHaveScripts;
	bool canHaveCustomParameters;
	bool userCanRemove;

	ScopedPointer<ScriptManager> scriptManager;
	ScopedPointer<GenericControllableManager> customParams;

	void remove();
	virtual void clear() {}

	void onContainerParameterChanged(Parameter *) override;
	void onContainerTriggerTriggered(Trigger *) override;
	virtual void onContainerParameterChangedInternal(Parameter *) {} //child classes override this function
	virtual void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void onContainerNiceNameChanged() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	virtual String getTypeString() const { return "BaseItem"; };

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void askForRemoveBaseItem(BaseItem *) {}
	};

	ListenerList<Listener> baseItemListeners;
	void addBaseItemListener(Listener* newListener) { baseItemListeners.add(newListener); }
	void removeBaseItemListener(Listener* listener) { baseItemListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItem)
};



#endif  // BASEITEM_H_INCLUDED