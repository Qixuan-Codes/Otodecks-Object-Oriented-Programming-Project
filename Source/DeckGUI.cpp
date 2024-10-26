/*
  ==============================================================================

    DeckGUI.cpp
    Created: 16 Feb 2023 7:28:04pm
    Author:  Qixuan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                ) : player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse)
{
    // Getting the Image from JUCE binary data and assigning it to a JUCE Image
    Image playImage = ImageFileFormat::loadFrom(BinaryData::play_png, BinaryData::play_pngSize);
    Image pauseImage = ImageFileFormat::loadFrom(BinaryData::pause_png, BinaryData::pause_pngSize);
    Image restartImage = ImageFileFormat::loadFrom(BinaryData::restart_png, BinaryData::restart_pngSize);
    Image stopImage = ImageFileFormat::loadFrom(BinaryData::stop_png, BinaryData::stop_pngSize);
    Image loadImage = ImageFileFormat::loadFrom(BinaryData::load_png, BinaryData::load_pngSize);

    // Setting the images of the image buttons
    playButton.setImages(true, true, true, playImage, 0.7f, juce::Colours::transparentBlack, 
                         playImage, 1.0f, juce::Colours::transparentBlack, playImage, 0.7f, juce::Colours::transparentBlack, 0.0f);
    pauseButton.setImages(true, true, true, pauseImage, 0.7f, juce::Colours::transparentBlack, 
                         pauseImage, 1.0f, juce::Colours::transparentBlack, pauseImage, 0.7f, juce::Colours::transparentBlack, 0.0f);
    restartButton.setImages(true, true, true, restartImage, 0.7f, juce::Colours::transparentBlack, 
                         restartImage, 1.0f, juce::Colours::transparentBlack, restartImage, 0.7f, juce::Colours::transparentBlack, 0.0f);
    stopButton.setImages(true, true, true, stopImage, 0.7f, juce::Colours::transparentBlack, 
                         stopImage, 1.0f, juce::Colours::transparentBlack, stopImage, 0.7f, juce::Colours::transparentBlack, 0.0f);
    loadButton.setImages(true, true, true, loadImage, 0.7f, juce::Colours::transparentBlack, 
                         loadImage, 1.0f, juce::Colours::transparentBlack, loadImage, 0.7f, juce::Colours::transparentBlack, 0.0f);

    // Buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    // Sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    // Waveform
    addAndMakeVisible(waveformDisplay);

    // Slider Labels
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(titleLabel);

    // Setting the text of the labels
    volumeLabel.setText("Gain", dontSendNotification);
    speedLabel.setText("Speed", dontSendNotification);
    positionLabel.setText("Position", dontSendNotification);
    titleLabel.setText("", dontSendNotification);

    // Attaching the label to its component
    volumeLabel.attachToComponent(&volSlider, false);
    speedLabel.attachToComponent(&speedSlider, false);
    positionLabel.attachToComponent(&posSlider, false);
    titleLabel.attachToComponent(&waveformDisplay, false);

    // Setting the label's justification type
    volumeLabel.setJustificationType(Justification::centred);
    speedLabel.setJustificationType(Justification::centred);
    positionLabel.setJustificationType(Justification::centred);
    titleLabel.setJustificationType(Justification::topLeft);
    
    // Adding a listener to all the buttons
    playButton.addListener(this);
    pauseButton.addListener(this);
    restartButton.addListener(this);
    loopButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    // Adding a listener to all the sliders
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Setting the range of the sliders
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    // Setting the style of the slider to rotary
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    posSlider.setSliderStyle(Slider::SliderStyle::Rotary);

    // Set the text box to appear below the slider
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
    posSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);

    // Setting the slider Look and Feel using the custom look and feel
    otherLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    volSlider.setLookAndFeel(&otherLookAndFeel);
    speedSlider.setLookAndFeel(&otherLookAndFeel);
    posSlider.setLookAndFeel(&otherLookAndFeel);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);   // clear the background

    g.setColour(Colours::red);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    // g.drawText ("DeckGUI", getLocalBounds(),Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 5;
    double rowW = getWidth() / 6;
    double sliderW = getWidth() / 3;

    // Bounds setting for the buttons
    loadButton.setBounds(0, rowH * 0.05, rowW, rowH * 0.85);
    playButton.setBounds(rowW, 0, rowW, rowH);
    pauseButton.setBounds(rowW * 2, 0, rowW, rowH);
    stopButton.setBounds(rowW * 3, rowH * 0.05, rowW, rowH * 0.9);
    restartButton.setBounds(rowW * 4, 0, rowW, rowH);
    loopButton.setBounds(rowW * 5, 0, rowW, rowH);

    // Bounds setting for the sliders
    volSlider.setBounds(0, rowH * 1.4, sliderW, rowH * 1.5);
    speedSlider.setBounds(sliderW, rowH * 1.4, sliderW, rowH * 1.5);
    posSlider.setBounds(sliderW * 2, rowH * 1.4, sliderW, rowH * 1.5);

    // Bounds setting for waveform display
    waveformDisplay.setBounds(0, rowH * 3.4, getWidth(), rowH * 1.6);
    
}

void DeckGUI::buttonClicked(Button* button)
{
    // Play Button
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }

    // Pause Button
    if (button == &pauseButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }

    // Restart Button
    if (button == &restartButton) {
        player->setPosition(0);
    }

    // Stop Button
    if (button == &stopButton) {
        player->stop();
        player->setPosition(0);
    }

    // Load Button
    if (button == &loadButton) {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser) {
            URL track = URL(chooser.getResult());
            player->loadURL(track);
            waveformDisplay.loadURL(track);
            titleLabel.setText("Now Playing: " + track.getFileName(), dontSendNotification);
        });
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    // Volume Slider
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    // Speed Slider
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    // Position Slider
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

}

void DeckGUI::timerCallback()
{
    // Display the moving rectangle box on waveform display determined by the current position of the track
    waveformDisplay.setPositionRelative(player->getPositionRelative());

    // Ensure that the track will be looped when it is finished
    if (loopButton.getToggleState() == 1) {
        if (player->getPositionRelative() >= 1) {
            player->setPosition(0);
            player->start();
        }
    }
    else {
        if (player->getPositionRelative() >= 1) {
            player->stop();
        }
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        File track = File(files[0]);
        URL trackURL = URL{ track };
        DBG(trackURL.toString(true));
        player->loadURL(trackURL);
        waveformDisplay.loadURL(trackURL);
    }
}

void DeckGUI::play(URL track)
{
    player->loadURL(track);
    waveformDisplay.loadURL(track);
    // Setting the text to appear when a song is loaded
    titleLabel.setText("Now Playing: " + track.getFileName(), dontSendNotification);
}