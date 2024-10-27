# Otodecks: Audio Mixing Application

Otodecks is an audio mixing application built with JUCE, offering functionalities such as loading, playing, and mixing multiple audio tracks, and saving a persistent music library. This README outlines the key features, installation instructions, and usage details.

## Features

### 1. Audio File Loading and Playback
- **Single Track Loading**: Load individual audio files into designated audio players using the `Load` button.
- **Simultaneous Playback**: Play multiple tracks simultaneously with independent audio players, allowing for effective audio mixing.

### 2. Track Mixing and Effects
- **Volume Control**: Adjust the volume of each track independently through a slider to achieve desired mix levels.
- **Playback Speed**: Control playback speed to speed up or slow down each track using a dedicated slider.

### 3. Custom Graphics and User Interface
- **Custom Deck Graphics**: Each component has customized visuals, such as black backgrounds and unique borders, offering a professional appearance.
- **Playlist Table Styling**: Tracks in the playlist are displayed in a table with a distinctive row selection and line separation color scheme.

### 4. Music Library and Persistence
- **Library Loading and Saving**: Add multiple files to the music library at once, avoiding duplicates. The library is saved to a text file for easy restoration.
- **Search Functionality**: Quickly find tracks in the library by typing keywords; matching rows are highlighted.
- **Persistent Storage**: When the application restarts, previously loaded tracks are restored from a saved text file.

### 5. Additional Controls and Features
- **Playback Controls**: Adjust playback position, stop or restart tracks, and loop tracks with the available control buttons.
- **Customizable Sliders**: Rotary sliders with custom graphics for an enhanced user experience.

## Installation

To clone and run this project locally, follow these steps:
```
git clone https://github.com/Qixuan-Codes/Otodecks-Object-Oriented-Programming-Project.git
```

### Prerequisites
- [JUCE Framework](https://juce.com/get-juce) should be installed.
- Compatible C++ compiler and IDE (such as Visual Studio or Xcode) is required.

## Usage

1. **Loading Tracks**:
   - Open the application and click on the `Load` button to select audio files. Files will be loaded into the library and displayed in the playlist table.

2. **Playing and Mixing Tracks**:
   - Use playback controls to start, stop, or adjust the track position.
   - Mix tracks by adjusting the volume and speed sliders for each loaded track.

3. **Library Management**:
   - Add new files to your library by clicking the load button.
   - Use the search bar to locate files based on titles. Matching tracks are highlighted.
   - Library content is saved on exit and reloaded upon restarting the application.

4. **Other Controls**:
   - Adjust the rotary sliders for custom playback settings, such as looping or position resetting.

## Contributing
If you’d like to contribute to this project, feel free to fork the repository, create a feature branch, and submit a pull request.