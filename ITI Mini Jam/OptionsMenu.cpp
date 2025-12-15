#include "OptionsMenu.h"

#include <iostream>
#include <string>

using namespace sf;
using namespace std;

OptionsMenu::OptionsMenu(float WIDTH, float HEIGHT, SoundManager* sm)
{
    soundMgr = sm;
    soundMgr->setMusicVolume(1.f);
    soundMgr->setSFXVolume(0.f);

    if (!backgroundTex.loadFromFile("Assets/SettingsMenu/Settings_Menu_BG.png")) {
        std::cerr << "Options background not found\n";
    }
    backgroundSprite.setTexture(backgroundTex);
    backgroundSprite.setPosition(0.f, 0.f);

    // Optional: scale to screen size
    backgroundSprite.setScale(
        WIDTH / backgroundTex.getSize().x,
        HEIGHT / backgroundTex.getSize().y
    );

    float centerX = WIDTH / 2.f;
    float baseY = HEIGHT / 2.f - 120.f;

    musicSlider = Slider(centerX - 680.f, baseY + 150.f, 400.f, static_cast<int>(sm->musicVolume));
    sfxSlider = Slider(centerX - 680.f, baseY + 430.f, 400.f, static_cast<int>(sm->sfxVolume));

    backButton = UIButton({ 155.f, 142.f }, { centerX-750, baseY - 300}, Color(255, 255, 255));
	backButton.loadTexture("Assets/Buttons/Back_Button.png");


    font = Font();
    if (!font.loadFromFile("Assets/Fonts/MyFont.ttf"))
        cerr << "Warning: arial.ttf not found (using default font)\n";

    musicValueText.setFont(font); 
    musicValueText.setCharacterSize(20); musicValueText.Bold; 
    musicValueText.setPosition(centerX - 725.f, baseY + 135.f);
    musicValueText.setFillColor(Color::Black);

    sfxValueText.setFont(font); 
	sfxValueText.setCharacterSize(20); sfxValueText.Bold;
    sfxValueText.setPosition(centerX - 735.f, baseY + 415.f); 
    sfxValueText.setFillColor(Color::Black);

    updateValueTexts();
}

int OptionsMenu::update(RenderWindow& window, const Event& ev)
{
    bool changed = false;
    if (musicSlider.handleEvent(window, ev)) {
        changed = true;
        if (soundMgr) soundMgr->setMusicVolume(static_cast<float>(musicSlider.getValue()));
    }
    if (sfxSlider.handleEvent(window, ev)) {
        changed = true;
        if (soundMgr) soundMgr->setSFXVolume(static_cast<float>(sfxSlider.getValue()));
    }

    if (changed) updateValueTexts();

    if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
        Vector2i mp = Mouse::getPosition(window);
        if (backButton.contains(mp)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1;
        }
    }

    return 0;
}

void OptionsMenu::updateValueTexts() {
    musicValueText.setString(to_string(musicSlider.getValue()) + "%");
    sfxValueText.setString(to_string(sfxSlider.getValue()) + "%");
}

void OptionsMenu::draw(RenderWindow& window) {
    window.draw(backgroundSprite);

    window.draw(titleText);
    window.draw(musicLabel);
    window.draw(sfxLabel);
    musicSlider.draw(window);
    sfxSlider.draw(window);
    window.draw(musicValueText);
    window.draw(sfxValueText);
    backButton.draw(window);
}


