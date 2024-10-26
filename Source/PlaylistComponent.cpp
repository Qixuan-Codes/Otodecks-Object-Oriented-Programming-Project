/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 16 Feb 2023 7:28:44pm
    Author:  Qixuan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "MainComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    formatManager.registerBasicFormats();

    // Creating the table component with the respective headers
    tableComponent.getHeader().addColumn("Track Title", 1, 200);
    tableComponent.getHeader().addColumn("Track Length", 2, 200);
    tableComponent.getHeader().addColumn("Deck 1", 3, 100);
    tableComponent.getHeader().addColumn("Deck 2", 4, 100);
    tableComponent.getHeader().addColumn("Delete", 5, 200);

    tableComponent.setModel(this);
    tableComponent.setColour(juce::ListBox::backgroundColourId, Colours::black);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(findFile);

    loadButton.addListener(this);
    findFile.addListener(this);

    // reading from the playlist.txt file to ensure that the tracks added before the
    // application was closed still persists and appears in the application
    std::ifstream playlist("Playlist.txt");
    std::string string;
    if (is_empty(playlist)) {
        playlist.close();
        DBG("No tracks");
    }
    else {
        while (std::getline(playlist, string)) {
            // Adding the path to track playlist
            trackPlaylist.add(string);

            String file_path = string;
            // Converting the string to a JUCE File
            juce::File file(file_path);

            if (!file.existsAsFile()) {
                DBG("File does not exists");
            }

            // Adding the track title and track length
            trackTitles.add(title(file));
            trackLength.add(duration(file));
        }
        playlist.close();
    }
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(findColour(juce::ListBox::backgroundColourId));   // clear the background

    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);

    // Text Editor Styles
    findFile.setColour(TextEditor::highlightedTextColourId, Colours::white);
    findFile.setColour(TextEditor::highlightColourId, Colours::blue);
    findFile.setColour(TextEditor::textColourId, Colours::white);
    findFile.setColour(TextEditor::backgroundColourId, Colours::black);
    findFile.setIndents(4, 8);
    findFile.setFont(16.0f);
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 8;
    loadButton.setBounds(0, 0, getWidth() / 2, rowH);
    findFile.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), rowH * 7);

    findFile.setTextToShowWhenEmpty("Search.....", Colours::white);
    findFile.setFont(14.0f);
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::black);
    }
    g.setColour(Colours::orange);
    g.drawLine(0, height - 1, width, height - 1);
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1) {
        g.setColour(juce::Colours::white);
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);

    }
    if (columnId == 2) {
        g.setColour(Colours::white);
        g.drawText(convert(trackLength[rowNumber]), 2, 0, width - 4, height, Justification::centredLeft, true);
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton("Add");
            btn->setColour(TextButton::buttonColourId, Colours::darkgreen);
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            // Attaching an onclick function to the buttons
            btn->onClick = [this] {addDeck1(); };
        }
    }
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton("Add");
            btn->setColour(TextButton::buttonColourId, Colours::darkgreen);
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            // Attaching an onclick function to the buttons
            btn->onClick = [this] {addDeck2(); };
        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton("Delete");
            btn->setColour(TextButton::buttonColourId, Colours::darkred);
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            // Attaching an onclick function to the buttons
            btn->onClick = [this] {remove(); };
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadButton) {
        auto fileChooserFlags = FileBrowserComponent::canSelectMultipleItems;

        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser) {
            // Getting the file chosen by the user and adding it to an array of files
            Array<File> fileChosen = chooser.getResults();

        // looping the array
        for (int i = 0; i < fileChosen.size(); i++) {
            String trackPath = fileChosen[i].getFullPathName().toStdString();
            String name = title(fileChosen[i]);
            double length = duration(fileChosen[i]);

            // checking if there are duplicates
            if (trackPlaylist.indexOf(trackPath) != -1) {
                DBG("File is already in playlist");
            }
            else {
                // updating the various arrays
                trackPlaylist.add(trackPath);
                trackTitles.add(name);
                trackLength.add(length);
            }
        }

        // Updating the Playlist.txt
        updateFile(trackPlaylist);

        // Refresh the table component to display the newly added tracks
        tableComponent.updateContent();
        });
    }
    else {
        // Getting the rowIndex
        rowIndex = std::stoi(button->getComponentID().toStdString());
    }
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor)
{
    // Getting the text in the text editor
    search = findFile.getText();

    for (int i = 0; i < trackTitles.size(); ++i) {
        if (trackTitles[i].containsWholeWordIgnoreCase(search) == 1 && search != "") {
            // Highlight the row when the user types in a word that matches the track titles
            tableComponent.selectRow(i, false, true);
        }
        else if (search == "" || trackTitles[i].containsWholeWordIgnoreCase(search) == -1) {
            // Deselect all rows when nothing is typed
            tableComponent.deselectAllRows();
        }
    }
}

void PlaylistComponent::remove()
{
    // removing the track from the arrays based on the row index
    trackPlaylist.remove(rowIndex);
    trackTitles.remove(rowIndex);
    trackLength.remove(rowIndex);

    // Refresh the table component to display the changes
    tableComponent.updateContent();

    // Update playlist.txt
    updateFile(trackPlaylist);
}

String PlaylistComponent::convert(double length)
{
    // Getting the minutes
    int lengthInMin = floor(length / 60);

    // Getting the seconds
    int seconds = length - (lengthInMin * 60);

    // Putting the minutes and seconds to a string and returning it
    String totalLength = std::to_string(lengthInMin) + "m " + std::to_string(seconds) + "s";
    return totalLength;
}

double PlaylistComponent::duration(File chosen)
{
    // Creating a reader for the file
    AudioFormatReader* reader = formatManager.createReaderFor(chosen);

    // Getting the length of the track and returning it
    double length = reader->lengthInSamples / reader->sampleRate;
    return length;
}

String PlaylistComponent::title(File chosen)
{
    // Getting the track title and returning it
    String tracktitle = juce::URL::removeEscapeChars(juce::URL{ chosen }.getFileName());
    return tracktitle;
}

void PlaylistComponent::updateFile(Array<String> trackPlaylist)
{
    // Writing to playlist.txt
    std::ofstream playlist("Playlist.txt");
    for (int i = 0; i < trackPlaylist.size(); i++) {
        playlist << trackPlaylist[i] << std::endl;
    }
    playlist.close();
}

void PlaylistComponent::selectMainComponent(MainComponent* mainComponent)
{
    // Selecting the instance of main component to be used in addDeck1 and addDeck2
    this->mainComponent = mainComponent;
}

void PlaylistComponent::addDeck1()
{
    // Converting the String to a URL
    String track = trackPlaylist[rowIndex];
    File trackChosen = File(track);
    URL fileURL = URL{ trackChosen };

    // Calling a mainComponent function to play the track in deck 1
    mainComponent->deckGUIPlay(fileURL, 1);
}

void PlaylistComponent::addDeck2()
{
    // Converting the String to a URL
    String track = trackPlaylist[rowIndex];
    File trackChosen = File(track);
    URL fileURL = URL{ trackChosen };

    // Calling a mainComponent function to play the track in deck 2
    mainComponent->deckGUIPlay(fileURL, 2);
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    for (int i = 0; i < files.size(); i++) {
        if (trackPlaylist.indexOf(files[i]) != -1) {
            DBG("File is already in playlist");
        }
        else {
            trackPlaylist.add(files[i]);
            trackTitles.add(title(files[i]));
            trackLength.add(duration(files[i]));
        }
    }
    updateFile(trackPlaylist);
    tableComponent.updateContent();
}