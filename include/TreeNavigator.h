// ================================== //
// --   CREATED BY: John Virador   -- //
// --  MIT License Software @2026  -- //
// ================================== //

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Config.h"

class TreeNavigator {
    private:
        std::string root;

        std::string parentId;
        bool wrap;
        std::size_t currentIndex = 1;

        std::unordered_map<
            std::string,
            std::vector<Metadata>
        > directory;

        void validateElement(const Metadata& element) const;

        void registerElement(const Metadata& element);

    public:
        TreeNavigator(const std::vector<Metadata>& config);

        struct CursorState {
            std::string containerId;
            std::size_t index;

            bool operator==(const CursorState& other) const;

            bool operator!=(const CursorState& other) const;
        };

        Metadata current() const;

        // Get the metadata of the currently selected container or its parent
        Metadata getParentMetadata() const;

        void next();

        void prev();

        void enter();

        void back();

        bool go(std::string id);

        std::size_t getSiblingCount() const;

        std::vector<Metadata> getSiblings() const;

        const std::vector<Metadata>& getSiblingsRef() const;

        std::size_t getChildrenCount() const;

        std::vector<Metadata> getChildren() const;

        const std::vector<Metadata>& getChildrenRef() const;

        bool isRoot() const;

        CursorState getCursorState() const;
};