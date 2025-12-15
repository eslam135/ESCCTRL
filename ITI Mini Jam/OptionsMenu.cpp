#include "OptionsMenu.h"
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

OptionsMenu::OptionsMenu(float WIDTH, float HEIGHT, SoundManager* sm)
{
    soundMgr = sm;

    // -------- Background --------
    if (!backgroundTex.loadFromFile("Assets/SettingsMenu/Settings_Menu_BG.png"))
        std::cerr << "Options background not found\n";

    backgroundSprite.setTexture(backgroundTex);
    backgroundSprite.setScale(
        WIDTH / backgroundTex.getSize().x,
        HEIGHT / backgroundTex.getSize().y
    );

    float centerX = WIDTH / 2.f;
    float baseY = HEIGHT / 2.f - 120.f;

    // -------- Sliders --------
    musicSlider = Slider(centerX - 680.f, baseY + 150.f, 400.f,
        static_cast<int>(sm->musicVolume));

    sfxSlider = Slider(centerX - 680.f, baseY + 430.f, 400.f,
        static_cast<int>(sm->sfxVolume));

    // -------- Back Button --------
    backButton = UIButton({ 155.f, 142.f }, { centerX - 750.f, baseY - 300.f });

    if (tBack.loadFromFile("Assets/Buttons/Back_Button.png"))
        backButton.rect.setTexture(&tBack);

    tBackHover.loadFromFile("Assets/Buttons/BackButton_hover.png");

    // -------- Font --------
    if (!font.loadFromFile("Assets/Fonts/MyFont.ttf"))
        cerr << "Warning: MyFont.ttf not found\n";

    musicValueText.setFont(font);
    musicValueText.setCharacterSize(20);
    musicValueText.setPosition(centerX - 725.f, baseY + 135.f);
    musicValueText.setFillColor(Color::Black);

    sfxValueText.setFont(font);
    sfxValueText.setCharacterSize(20);
    sfxValueText.setPosition(centerX - 735.f, baseY + 415.f);
    sfxValueText.setFillColor(Color::Black);

    updateValueTexts();
}

int OptionsMenu::update(RenderWindow& window, const Event& ev)
{
    bool changed = false;

    if (musicSlider.handleEvent(window, ev)) {
        changed = true;
        if (soundMgr)
            soundMgr->setMusicVolume(static_cast<float>(musicSlider.getValue()));
    }

    if (sfxSlider.handleEvent(window, ev)) {
        changed = true;
        if (soundMgr)
            soundMgr->setSFXVolume(static_cast<float>(sfxSlider.getValue()));
    }

    if (changed)
        updateValueTexts();

    Vector2i mousePos = Mouse::getPosition(window);

    // -------- Hover logic --------
    if (backButton.contains(mousePos)) {
        if (tBackHover.getSize().x)
            backButton.rect.setTexture(&tBackHover);
    }
    else if (tBack.getSize().x) {
        backButton.rect.setTexture(&tBack);
    }

    // -------- Click logic --------
    if (ev.type == Event::MouseButtonPressed &&
        ev.mouseButton.button == Mouse::Left)
    {
        if (backButton.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1;
        }
    }

    return 0;
}

void OptionsMenu::updateValueTexts()
{
    musicValueText.setString(to_string(musicSlider.getValue()) + "%");
    sfxValueText.setString(to_string(sfxSlider.getValue()) + "%");
}

void OptionsMenu::draw(RenderWindow& window)
{
    window.draw(backgroundSprite);

    musicSlider.draw(window);
    sfxSlider.draw(window);
    window.draw(musicValueText);
    window.draw(sfxValueText);
    backButton.draw(window);
}
