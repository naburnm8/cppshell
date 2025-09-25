//
// Created by Артём Линт on 13.08.2025.
//

#include "execution_effect.h"

#include <iostream>

void Effect::Redirect::commenceEffect() {
    std::cout << "Test" << std::endl;
}

void Effect::Redirect::cleanup() {
    std::cout << "Test cleanup" << std::endl;
}


