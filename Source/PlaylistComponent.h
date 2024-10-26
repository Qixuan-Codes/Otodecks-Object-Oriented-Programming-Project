/*
  ==============================================================================

    PlaylistComponent.h
    Created: 16 Feb 2023 7:28:44pm
    Author:  Qixuan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream> 
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class MainComponent;

class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public FileDragAndDropTarget
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    // Function to check whether the text editor has changed
    void textEditorTextChanged(juce::TextEditor& editor) override;

    // Function to remove the file from tracktitles, trackplaylist and track length
    void remove();

    // Function to convert the duration to mins and seconds
    String convert(double length);

    // Function to get the length of the track
    double duration(File chosen);

    // Function to get the title of the track
    String title(File chosen);

    // Function to update the txt file
    void updateFile(Array<String> trackPlaylist);

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    // Function to select which instance of main component to call
    void selectMainComponent(MainComponent* mainComponent);

    // Functions to add the track to deckGUI
    void addDeck1();
    void addDeck2();

private:
    AudioFormatManager formatManager;
    MainComponent* mainComponent;

    TableListBox tableComponent;

    Array<String> trackTitles;
    Array<String> trackPlaylist;
    Array<double> trackLength;

    String search;
    TextButton loadButton{ "LOAD" };
    int rowIndex;

    bool is_empty(std::ifstream& pFile)
    {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }

    FileChooser fChooser{ "Select a file..." , File(), "*.mp3", true, false };

    juce::TextEditor findFile{ "Search" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
