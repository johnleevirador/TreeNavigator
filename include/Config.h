// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //
#pragma once

#include <string>
#include <utility>

// ============================================================
// USER CUSTOMIZATION AREA
// ============================================================
//
// Hey! Looking for a way to customize your Payload?
// YOU'RE IN THE RIGHT PLACE!
//
// Want to add a field? Remove one? Change its type or default
// value? Go ahead! You can do all of that here.
//
// PLEASE DO NOT GO LOOKING FOR ANOTHER PLACE TO CUSTOMIZE.
// THIS. IS. THE. PLACE. RIGHT HERE. 😭
//
// A few rules to keep everything working correctly:
//   1. Only modify the declarations inside 
//      TREENAV_CUSTOM_PAYLOAD below.
//   2. Every member declaration must end with a semicolon.
//   3. Do not modify DEFINE_CUSTOM_PAYLOAD or remove its 
//      invocations inside Payload.
//   4. Do not modify the required members of Payload
//      and Metadata.
//
// Follow these rules, and your custom members will compiled
// with no issues.
//
// ============================================================

// ============================================================
// NOTE TO DEVELOPERS
// ============================================================
// 
// If you're a developer maintaining or contributing to this
// library, first of all, THANK YOU!
// 
// TreeNavigator wouldn't get very far without people like you
// taking the time to understand it, improve it, and help it
// grow. Your time and effort are genuinely appreciated.
// 
// You probably already know what you're doing, so I'll leave
// the technical decisions in your capable hands. Best of luck,
// and thank you for helping this project move forward!
// 
// If anything about the library leaves you confused, don't
// hesitate to reach out to the author. There's no shame in
// asking questions. We're all learning something.
// 
// Happy coding, and thank you for being part of TreeNavigator!
//
// - John Virador
// 
// ============================================================
#define TREENAV_CUSTOM_PAYLOAD \
    int xPos = 0;              \
    int yPos = 0;              \
    int width = 0;             \
    int height = 0;

// ==========================================
// Reusable injection macro
// - DO NOT MODIFY
// ==========================================
#define DEFINE_CUSTOM_PAYLOAD(fields) \
    static constexpr bool _treenav_custom_payload_defined = true; \
    fields

// ==========================================
// Payload
// ==========================================
struct Payload {
    // Required members
    std::string elementId;
    bool wrap;

    DEFINE_CUSTOM_PAYLOAD(
        TREENAV_CUSTOM_PAYLOAD
    )
};

// ==========================================
// Metadata
// ==========================================
struct Metadata : Payload {
    // Required members
    std::string parentId;

    // The constructor forwards the entire Payload part of itself automatically
    Metadata(std::string pId, const Payload& payload) 
        : Payload(payload), parentId(std::move(pId)) {}
};

// ==========================================
// Verify customization exists
// - DO NOT MODIFY.
// ==========================================
static_assert(
    Payload::_treenav_custom_payload_defined,
    "TreeNavigator: Do not remove DEFINE_CUSTOM_PAYLOAD."
);