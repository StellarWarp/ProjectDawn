//
// Created by Estelle on 2023-03-09.
//

#pragma once

#include "Core/GameObject.h"

namespace ModelLoader {
	GameObject* LoadModelFromFile(const std::wstring_view path);

};
