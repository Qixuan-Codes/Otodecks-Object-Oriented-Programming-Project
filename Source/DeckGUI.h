/*
  ==============================================================================

    DeckGUI.h
    Created: 16 Feb 2023 7:28:04pm
    Author:  Qixuan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//==============================================================================

// Custom LookAndFeel for the 3 sliders
class OtherLookAndFeel : public LookAndFeel_V4
{
public:
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override {
        // get the lesser of the 2 values as the height and width is different
        float diameter = jmin(width, height);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;

        // scale the angle down
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        Rectangle<float> dialArea(rx, ry, diameter, diameter);

        g.setColour(Colours::silver);
        g.fillEllipse(dialArea);

        g.setColour(Colours::black);

        // sequence of lines and curves that may either form a closed shape or be open-ended.
        Path dialTick;
        dialTick.addRectangle(0, -radius, 4.0f, radius * 0.33);

        // AffineTransform represents a 2d matrix
        g.fillPath(dialTick, AffineTransform::rotation(angle).translated(centerX, centerY));
    }
};

//==============================================================================
class DeckGUI  : public Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public Timer,
                 public FileDragAndDropTarget
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint(Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    void timerCallback() override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    // function to play the track and display its waveform
    void play(URL track);

private:

    // Image Buttons
    ImageButton playButton;
    ImageButton pauseButton;
    ImageButton restartButton;
    ImageButton stopButton;
    ImageButton loadButton;

    // Toggle Button
    ToggleButton loopButton{ "LOOP" };

    // Now playing label
    Label titleLabel;

    // Sliders
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    // Slider labels
    Label volumeLabel;
    Label speedLabel;
    Label positionLabel;

    OtherLookAndFeel otherLookAndFeel;

    WaveformDisplay waveformDisplay;

    FileChooser fChooser{ "Select a file..." , File(), "*.mp3", true, false };

    DJAudioPlayer* player;

    AudioFormatManager formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};

