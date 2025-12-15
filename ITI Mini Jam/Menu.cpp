#include "Menu.h"

#include <filesystem>
#include <thread>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace sf;
using namespace std;
namespace fs = std::filesystem;

Menu::Menu(float WIDTH, float HEIGHT, SoundManager* sm)
{
    soundMgr = sm;

    loadAnimatedBackground(
        "Assets/MainMenu/",
        WIDTH, HEIGHT
    );

    Vector2f startBtnSize(423.f, 169.f);
	Vector2f optionsBtnSize(422.f, 138.f);
	Vector2f exitBtnSize(367.f, 141.f);
    btnStart = UIButton(startBtnSize, { (WIDTH / 2.f) + 469.f, (HEIGHT / 2.f ) - 59.f });
    btnOptions = UIButton(optionsBtnSize, { (WIDTH / 2.f) + 420.f, (HEIGHT / 2.f) + 110.f });
    btnExit = UIButton(exitBtnSize, { (WIDTH / 2.f) + 469.f, (HEIGHT / 2.f) + 250.f });

    if (tStart.loadFromFile("Assets/Buttons/start.png"))
        btnStart.rect.setTexture(&tStart);

    tStartHover.loadFromFile("Assets/Buttons/start_hover.png");

    if (tOptions.loadFromFile("Assets/Buttons/options.png"))
        btnOptions.rect.setTexture(&tOptions);

    tOptionsHover.loadFromFile("Assets/Buttons/options_hover.png");

    if (tExit.loadFromFile("Assets/Buttons/exit.png"))
        btnExit.rect.setTexture(&tExit);

    tExitHover.loadFromFile("Assets/Buttons/exit_hover.png");
}

void Menu::loadAnimatedBackground(const std::string& folder,
    float WIDTH, float HEIGHT)
{
    bgFrames.clear();

    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".png") {
            files.push_back(entry.path().string());
        }
    }

    std::sort(files.begin(), files.end());

    const int THREAD_COUNT = 7;
    std::vector<std::vector<sf::Texture>> partialTextures(THREAD_COUNT);
    std::vector<std::thread> threads;

    int chunkSize = (files.size() + THREAD_COUNT - 1) / THREAD_COUNT;

    for (int t = 0; t < THREAD_COUNT; ++t) {
        threads.emplace_back([t, chunkSize, &files, &partialTextures]() {
            int start = t * chunkSize;
            int end = std::min(start + chunkSize, (int)files.size());

            for (int i = start; i < end; i += FRAME_STEP) {
                sf::Texture tex;
                if (tex.loadFromFile(files[i])) {
                    partialTextures[t].push_back(std::move(tex));
                }
                else {
                    std::cerr << "Failed to load: " << files[i] << "\n";
                }
            }
            });
    }

    for (auto& th : threads)
        th.join();

    for (auto& vec : partialTextures)
        for (auto& tex : vec)
            bgFrames.push_back(std::move(tex));

    if (bgFrames.empty()) {
        std::cerr << "Warning: No background frames found\n";
        return;
    }

    bg.setTexture(bgFrames[0], true);
    bg.setScale(
        WIDTH / bgFrames[0].getSize().x,
        HEIGHT / bgFrames[0].getSize().y
    );
}


void Menu::updateBackground()
{
    if (bgFrames.size() <= 1) return;

    if (frameClock.getElapsedTime().asSeconds() < frameTime)
        return;

    frameClock.restart();

    currentFrame += frameDirection;

    if (currentFrame == bgFrames.size() - 1)
        frameDirection = -1;
    else if (currentFrame == 0)
        frameDirection = 1;

    bg.setTexture(bgFrames[currentFrame], true);
}

int Menu::update(RenderWindow& window)
{
    updateBackground();

    Vector2i mousePos = Mouse::getPosition(window);

    if (btnStart.contains(mousePos)) {
        if (tStartHover.getSize().x)
            btnStart.rect.setTexture(&tStartHover);
    }
    else if (tStart.getSize().x)
        btnStart.rect.setTexture(&tStart);

    if (btnOptions.contains(mousePos)) {
        if (tOptionsHover.getSize().x)
            btnOptions.rect.setTexture(&tOptionsHover);
    }
    else if (tOptions.getSize().x)
        btnOptions.rect.setTexture(&tOptions);

    if (btnExit.contains(mousePos)) {
        if (tExitHover.getSize().x)
            btnExit.rect.setTexture(&tExitHover);
    }
    else if (tExit.getSize().x)
        btnExit.rect.setTexture(&tExit);

    if (Mouse::isButtonPressed(Mouse::Left)) {
        if (btnStart.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 1;
        }
        if (btnOptions.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 2;
        }
        if (btnExit.contains(mousePos)) {
            if (soundMgr) soundMgr->playSFX("button_click");
            return 3;
        }
    }

    return 0;
}

void Menu::draw(RenderWindow& window)
{
    if (!bgFrames.empty())
        window.draw(bg);

    btnStart.draw(window);
    btnOptions.draw(window);
    btnExit.draw(window);
}
