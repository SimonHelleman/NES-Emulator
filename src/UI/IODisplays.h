#pragma once
#include <IO/IOPort.h>
#include <IO/StandardJoypad.h>

void RenderIOPort(const IOPort& port, const char* title);
void RenderStdJoypad(const StandardJoypad& controller, const char* title);