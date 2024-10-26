#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : playlistComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { if (granted)  setAudioChannels(2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    formatManager.registerBasicFormats();

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(playlistComponent);

    // Setting playlist component to use this instance of main component
    playlistComponent.selectMainComponent(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);

    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}

void MainComponent::deckGUIPlay(URL track, int deck)
{
    // Calling the function in deckGUI based on what deck was chosen to load the track
    if (deck == 1) {
        deckGUI1.play(track);
    }
    if (deck == 2) {
        deckGUI2.play(track);
    }
}