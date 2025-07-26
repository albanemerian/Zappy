/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Help
*/

#include "Help.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include "../../../Utils/Constants.hpp"

Help::Help(std::shared_ptr<IDisplay> display, std::shared_ptr<IAudio> audio)
    : _display(display), _audio(audio), _helpContainer(nullptr), _visible(false)
{
    initHelpContainer();
}

void Help::show()
{
    _visible = true;
}

void Help::hide()
{
    _visible = false;
}

bool Help::isVisible() const
{
    return _visible;
}

void Help::update()
{
    if (this->_display->isKeyReleased(this->_display->getKeyId(C)))
        _visible = !_visible;

    if (!_visible || !_helpContainer)
        return;

    _helpContainer->update();
}

void Help::draw()
{
    if (!_visible || !_helpContainer)
        return;

    _helpContainer->draw();
}

void Help::handleResize(int oldWidth, int oldHeight, int newWidth, int newHeight)
{
    if (!_helpContainer)
        return;

    _helpContainer->handleResize(oldWidth, oldHeight, newWidth, newHeight);
}

void Help::initHelpContainer()
{
    if (!_display)
        return;

    Vector2i screenSize = this->_display->getScreenSize();
    int screenWidth = screenSize.x;
    int screenHeight = screenSize.y;

    float width = screenWidth * 0.50f;
    float height = screenHeight * 0.30f;
    float x = (screenWidth - width) / 2;
    float y = (screenHeight - height) / 2;

    _helpContainer = std::make_shared<Containers>(
        _display,
        _audio,
        x, y,
        width, height,
        Color32{50, 50, 50, 240}
    );

    _helpContainer->setRelativePosition(
        15.f, 20.0f,
        85.0f, 60.0f
    );

    _helpContainer->addTextPercent(
        "help_title",
        5.0f, 2.0f,
        zappy::constants::HELP_TITLE,
        6.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addButtonPercent(
        "help_close_button",
        92.0f, 2.0f,
        6.0f, 6.0f,
        "X",
        [this]() {
            this->hide();
        },
        {200, 60, 60, 255},
        {255, 100, 100, 255},
        {180, 40, 40, 255},
        {255, 255, 255, 255}
    );

    _helpContainer->addTextPercent(
        "help_separator",
        0.0f, 8.0f,
        std::string(500, '-'),
        1.0f,
        {150, 150, 150, 200}
    );

    float yPos = 12.0f;

    _helpContainer->addTextPercent(
        "help_section1_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_1,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section1_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_1_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 10.0f;

    _helpContainer->addTextPercent(
        "help_section2_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_2,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section2_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_2_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 28.0f;

    _helpContainer->addTextPercent(
        "help_section3_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_3,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section3_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_3_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );
    yPos += 12.0f;

    _helpContainer->addTextPercent(
        "help_section4_title",
        5.0f, yPos,
        zappy::constants::HELP_SECTION_4,
        3.5f,
        {230, 230, 120, 255}
    );
    yPos += 5.0f;

    _helpContainer->addTextPercent(
        "help_section4_content",
        7.0f, yPos,
        zappy::constants::HELP_SECTION_4_CONTENT,
        2.5f,
        {220, 220, 220, 255}
    );

    yPos = 12.f;
    _helpContainer->addTextPercent(
        "help_section5_title",
        50.0f, yPos,
        zappy::constants::HELP_SECTION_5,
        3.5f,
        {230, 230, 120, 255}
    );


    yPos = 54.f;
    _helpContainer->addTextPercent(
        "help_section6_title",
        50.0f, yPos,
        zappy::constants::HELP_SECTION_6,
        3.5f,
        {230, 230, 120, 255}
    );

    const float posYImageLevel = 25.0f;
    const float posYTextLevel = 42.0f;

    _helpContainer->addImagePercent(
        "help_model_lvl1",
        50.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl1.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl1",
        51.50f, posYTextLevel,
        "Lvl 1" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl2",
        56.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl2.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl2",
        57.5f, posYTextLevel,
        "Lvl 2" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl3",
        62.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl3.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl3",
        63.5f, posYTextLevel,
        "Lvl 3" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl4",
        68.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl4.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl4",
        69.5f, posYTextLevel,
        "Lvl 4" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl5",
        74.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl5.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl5",
        75.5f, posYTextLevel,
        "Lvl 5" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl6",
        80.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl6.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl6",
        81.5f, posYTextLevel,
        "Lvl 6" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl7",
        86.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl7.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl7",
        87.5f, posYTextLevel,
        "Lvl 7" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_model_lvl8",
        92.0f, posYImageLevel,
        5.0f, 15.0f,
        "gui/assets/img/skinLvl8.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_lvl8",
        93.5f, posYTextLevel,
        "Lvl 8" , 2.0f,
        {255, 255, 255, 255}
    );

    const float posYImageItem = 65.0f;
    const float posYTextItem = 82.0f;

    _helpContainer->addImagePercent(
        "help_item_food",
        50.0f, posYImageItem,
        5.0f, 13.5f,
        "gui/assets/img/food.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_food",
        51.5f, posYTextItem,
        "Food" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_linemate",
        56.0f, posYImageItem+0.1,
        5.0f, 12.25f,
        "gui/assets/img/linemate.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_linemate",
        56.8f, posYTextItem,
        "Linemate" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_deraumere",
        62.6f, posYImageItem,
        5.0f, 12.25f,
        "gui/assets/img/deraumere.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_deraumere",
        62.8f, posYTextItem,
        "Deraumere" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_sibur",
        68.9f, posYImageItem,
        5.0f, 12.25f,
        "gui/assets/img/sibur.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_sibur",
        70.4f, posYTextItem,
        "Sibur" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_mendiane",
        74.9f, posYImageItem,
        5.0f, 12.25f,
        "gui/assets/img/mendiane.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_mendiane",
        75.6f, posYTextItem,
        "Mendiane" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_phiras",
        80.9f, posYImageItem,
        5.0f, 12.25f,
        "gui/assets/img/phiras.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_phiras",
        82.f, posYTextItem,
        "Phiras" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_thystame",
        86.9f, posYImageItem,
        5.0f, 12.25f,
        "gui/assets/img/thystame.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_thystame",
        87.4f, posYTextItem,
        "Thystame" , 2.0f,
        {255, 255, 255, 255}
    );

    _helpContainer->addImagePercent(
        "help_item_egg",
        93.2f, posYImageItem,
        4.5f, 12.25f,
        "gui/assets/img/egg.png"
    );

    _helpContainer->addTextPercent(
        "help_model_txt_egg",
        94.8f, posYTextItem,
        "Egg" , 2.0f,
        {255, 255, 255, 255}
    );
    _visible = false;
}

bool Help::containsPoint(float x, float y) const
{
    if (!_helpContainer || !_visible)
        return false;

    return _helpContainer->contains(x, y);
}
