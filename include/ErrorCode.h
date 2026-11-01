// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //
#pragma once

enum class ErrorCode : int {
    Success = 0,
    ParentIdEmpty,
    ElementIdEmpty,
    DuplicateElement,
    ParentNotDefined,
    CannotDeleteRoot,
    CannotChangeElementId
};