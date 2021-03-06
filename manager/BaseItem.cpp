#include "BaseItem.h"
/*
  ==============================================================================

    BaseItem.cpp
    Created: 28 Oct 2016 8:04:25pm
    Author:  bkupe

  ==============================================================================
*/

BaseItem::BaseItem(const String &name, bool _canBeDisabled, bool _canHaveScripts, bool _canHaveCustomParameters) : 
	ControllableContainer(name.isEmpty()?getTypeString():name),
	canBeDisabled(_canBeDisabled),
	canHaveScripts(_canHaveScripts),
	canHaveCustomParameters(_canHaveCustomParameters),
	userCanRemove(true)
{
	saveAndLoadName = true;

	if (canBeDisabled)
	{
		enabled = addBoolParameter("Enabled", "Enable / Disable this component", true);
		enabled->hideInOutliner = true;
	}

	nameParam = addStringParameter("Name", "Name of the component", niceName);
	nameParam->includeInScriptObject = false;
	nameParam->hideInEditor = true;
	nameParam->hideInOutliner = true;
	nameParam->isTargettable = false;
	nameParam->lockManualControlMode = true;

	if (canHaveScripts)
	{
		scriptManager = new ScriptManager(this);
		addChildControllableContainer(scriptManager);
	}

	if (canHaveCustomParameters)
	{
		customParams = new GenericControllableManager("Custom Parameters");
		customParams->setCustomShortName("params");
		addChildControllableContainer(customParams);
		DBG("Add listener to custom Params ");
	}

	//For UI
	miniMode = addBoolParameter("Mini Mode", "Set the mini mode", false);
	miniMode->hideInOutliner = true;
	miniMode->hideInEditor = true;
	miniMode->isTargettable = false;

	listUISize = addFloatParameter("List Size", "Size in list", 0, 0, 500);
	listUISize->hideInEditor = true;
	listUISize->hideInOutliner = true;
	listUISize->isTargettable = false;

	viewUIPosition = addPoint2DParameter("Position", "Position the view");
	viewUIPosition->setBounds(-1000, -1000, 1000, 1000);
	viewUIPosition->hideInEditor = true;
	viewUIPosition->hideInOutliner = true;
	viewUIPosition->isTargettable = false;

	viewUISize = addPoint2DParameter("Size", "Size in the view");
	viewUISize->setBounds(30, 60, 1000, 1000);
	viewUISize->setPoint(200, 200);
	viewUISize->hideInEditor = true;
	viewUISize->hideInOutliner = true;
	viewUISize->isTargettable = false;
}

BaseItem::~BaseItem()
{
	clear();
}

void BaseItem::duplicate()
{
	baseItemListeners.call(&BaseItem::Listener::askForDuplicateItem, this);
}

void BaseItem::copy()
{
	var data = getJSONData();
	data.getDynamicObject()->setProperty("itemType", getTypeString());
	SystemClipboard::copyTextToClipboard(JSON::toString(data));
	NLOG(niceName, "Copied to clipboard");
}

void BaseItem::paste()
{
	//default behavior can be overriden
	baseItemListeners.call(&BaseItem::Listener::askForPaste);
}

void BaseItem::remove()
{
	baseItemListeners.call(&BaseItem::Listener::askForRemoveBaseItem, this);
}

void BaseItem::onContainerParameterChanged(Parameter * p)
{
	if (p == nameParam)
	{
		setNiceName(nameParam->stringValue());
	}

	Array<var> args;
	args.add(p->getScriptObject());
	if (canHaveScripts) scriptManager->callFunctionOnAllItems("localParamChanged", args);

	onContainerParameterChangedInternal(p);
}

void BaseItem::onContainerTriggerTriggered(Trigger * t)
{
	Array<var> args;
	args.add(t->getScriptObject());
	if (canHaveScripts) scriptManager->callFunctionOnAllItems("localParamChanged", args);
}

void BaseItem::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{

	if (cc == customParams)
	{
		Array<var> args;
		args.add(c->getScriptObject());
		if (canHaveScripts) scriptManager->callFunctionOnAllItems("customParamChanged", args);
	}

	onControllableFeedbackUpdateInternal(cc, c);
}

void BaseItem::onContainerNiceNameChanged()
{
	nameParam->setValue(niceName);
}

var BaseItem::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	if (canHaveScripts) data.getDynamicObject()->setProperty("scripts", scriptManager->getJSONData());
	if (canHaveCustomParameters) data.getDynamicObject()->setProperty("params", customParams->getJSONData());
	return data; 
}

void BaseItem::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);
	if (canHaveScripts) scriptManager->loadJSONData(data.getProperty("scripts",var()));
	if (canHaveCustomParameters) customParams->loadJSONData(data.getProperty("params", var()));
}

InspectableEditor * BaseItem::getEditor(bool isRoot)
{
	return ControllableContainer::getEditor(isRoot);// new BaseItemEditor(this, isRoot);
}
