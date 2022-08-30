#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    auto options = firebase::AppOptions();
    
    options.set_app_id("1:1048421010400:ios:3702c9caad6f3cc7f4b985");
    options.set_api_key("AIzaSyAyMk17qeuNnz7d7uUrtMQoMvrunGucKGc");
    options.set_project_id("musicapp-a0d2f");
    
    app = std::make_unique<firebase::App*>(firebase::App::Create(options));
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
