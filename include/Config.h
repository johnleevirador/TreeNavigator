// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //

#pragma once

#include <string>

struct Metadata {
    // ============================ //
    // -- REQUIRED DO NOT MODIFY -- //
    // ============================ //
    std::string parentId;
    std::string elementId;
    bool wrap;

    // ============================ //
    // -- OPTIONAL USER METADATA -- //
    // ============================ //
    int xPos = 0;
    int yPos = 0;
    int width = 0;
    int height = 0;
};
