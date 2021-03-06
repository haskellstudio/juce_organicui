/*
  ==============================================================================

    AppUpdater.h
    Created: 8 Apr 2017 4:26:46pm
    Author:  Ben

  ==============================================================================
*/

#ifndef APPUPDATER_H_INCLUDED
#define APPUPDATER_H_INCLUDED


class AppUpdater
{
public:
	juce_DeclareSingleton(AppUpdater, true);

	URL updateURL;
	URL downloadURL;

	void setURLs(URL _updateURL, URL _downloadURL);

	void checkForUpdates();
};


#endif  // APPUPDATER_H_INCLUDED
