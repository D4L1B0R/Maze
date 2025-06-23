#ifndef DISPLAY_HEADER
#define DISPLAY_HEADER

#include <iostream>

void printGameLegend() {
    std::cout << "\n==================[ GAME LEGEND – PRESS 'N' TO VIEW MAP ]===================\n\n";

    std::cout << "**R – THE CLOCKWORK ROBOT ROBERT: THE EXPLORER OF UNKNOWN**\n";
    std::cout << "A lone automaton cast into the Maze’s depths.\n";
    std::cout << "Moves one step at a time, leaving a faint trail of light (.) behind.\n";
    std::cout << "Your goal: guide R to freedom without falling prey to the Minotaur.\n\n";

    std::cout << "**M – THE MINOTAUR OF THE LABYRINTH**\n";
    std::cout << "A relentless hunter, stalking one square per turn.\n";
    std::cout << "Can trample and destroy any Power-Up (P) it crosses.\n";
    std::cout << "If it reaches R, the game is over—unless R wields a power worthy of legend.\n\n";

    std::cout << "**# – ANCIENT STONE WALLS**\n";
    std::cout << "Impassable barriers forged eons ago.\n";
    std::cout << "Some may be shattered by the Hammer of Earth’s Wrath, if you dare use it.\n\n";

    std::cout << "**. – LIGHT TRAIL**\n";
    std::cout << "Both R and M leave a single dot behind as they move—\n";
    std::cout << "marking their path through the twisting corridors.\n\n";

    std::cout << "**U – THE ENCHANTED ENTRANCE**\n";
    std::cout << "The only way in; once you pass through, the Maze’s magic binds you inside.\n\n";

    std::cout << "**I – THE FLICKERING EXIT**\n";
    std::cout << "Your one hope of escape. Find it before the Minotaur finds you.\n\n";

    std::cout << "**P – ANCIENT POWER-UPS**\n";
    std::cout << "Relics scattered in hidden alcoves.\n";
    std::cout << "R may collect and activate them ('+P') to turn the tides of battle.\n";
    std::cout << "But beware—M will crush them if it crosses their path.\n\n";

    std::cout << "=============================================================================\n\n";
}

void revealAllPowerUps() {
    std::cout << "\n==================[ MYSTICAL POWER-UPS ]===================\n\n";

    std::cout << "**SWORD OF THE LOST WARRIOR OF RUNETERRA**\n";
    std::cout << "A legendary blade once wielded by the ancient protector of the Maze.\n";
    std::cout << "With this sword, you can slay the fearsome Minotaur instead of dying.\n\n";

    std::cout << "**ANGELIC SHIELD OF AEONS**\n";
    std::cout << "Forged by forgotten sentinels, this shield grants you a second chance.\n";
    std::cout << "When the Minotaur attacks, the shield absorbs the fatal blow — but only once.\n\n";

    std::cout << "**HAMMER OF EARTH'S WRATH**\n";
    std::cout << "A mighty relic capable of smashing through walls.\n";
    std::cout << "Use it to destroy one wall blocking your path and open new ways forward.\n\n";

    std::cout << "**FOG OF THE DAMNED**\n";
    std::cout << "A cursed mist that reduces your vision to a 3x3 grid for several moves.\n";
    std::cout << "This makes escaping and navigating the Maze even more challenging.\n\n";

    std::cout << "============================================================\n";
}

#endif